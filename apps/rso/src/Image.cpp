#include "Image.hpp"

double average(dvec3 v)
{
  return dot(v, dvec3(0)) / 3.0;
}

void getPseudocolorRainbow(double val, double minVal, double maxVal, double &r, double &g, double &b)
{
  if (isnan(val) || isinf(val))
  {
    r = g = b = 0; // black ... exception
    return;
  }
  if (val < minVal)
    val = minVal;
  if (val > maxVal)
    val = maxVal;
  double ratio = (val - minVal) / (maxVal - minVal);
  double value = 1.0f - ratio;
  float val4 = value * 4.0f;
  value = val4 - (int)val4;
  switch ((int)(val4))
  {
  case 0:
    r = 1.0;
    g = value;
    b = 0.f;
    break;
  case 1:
    r = 1.0 - value;
    g = 1.0;
    b = 0.f;
    break;
  case 2:
    r = 0.f;
    g = 1.0;
    b = value;
    break;
  case 3:
    r = 0.f;
    g = 1.0 - value;
    b = 1.0;
    break;
  default:
    r = value * 1.0;
    g = 0.f;
    b = 1.0;
    break;
  }
  return;
}

void getPseudocolorCoolWarm(double val, double minVal, double maxVal, double &r, double &g, double &b)
{
  if (isnan(val) || isinf(val))
  {
    r = g = b = 0; // black ... exception
    return;
  }
  if (val < minVal)
    val = minVal;
  if (val > maxVal)
    val = maxVal;
  double ratio = (val - minVal) / (maxVal - minVal);
  int i = int(ratio * 31.999);
  assert(i < 33);
  assert(i >= 0);
  float alpha = i + 1.0 - (ratio * 31.999);
  r = Globals::pscols[4 * i + 1] * alpha + Globals::pscols[4 * (i + 1) + 1] * (1.0 - alpha);
  g = Globals::pscols[4 * i + 2] * alpha + Globals::pscols[4 * (i + 1) + 2] * (1.0 - alpha);
  b = Globals::pscols[4 * i + 3] * alpha + Globals::pscols[4 * (i + 1) + 3] * (1.0 - alpha);
  // printf("rgb=%f %f %f index=%d a=%g\n",r,g,b,i, alpha);
}

Image ReadHDR(const char* filename)
{
    FILE *fp;
    fp = fopen(filename, "rb");

    if (!fp) {
        std::cout << "failed to open: " << filename << std::endl;
        return {};
    }

    char programtype[20];
    float gamma;
    float exposure;
    char format[20];
    int height;
    int width;
    if (fscanf(fp, "#?%s\n", programtype) < 0)
        abort();
    if (fscanf(fp, "GAMMA=%g\n", &gamma) < 0)
        abort();
    if (fscanf(fp, "EXPOSURE=%g\n", &exposure) < 0)
        abort();
    if (fscanf(fp, "FORMAT=%s\n\n", format) < 0)
        abort();
    if (fscanf(fp, "-Y %d +X %d\n", &height, &width) < 0)
        abort();

    size_t count = width * height;
    std::vector<RGBE> data;
    data.resize(count);

    std::vector<dvec3> image;
    image.resize(count);

    size_t kk = fread(data.data(), (size_t)4, count, fp);

    const bool flipY = true;

    for (int i = 0; i < count; ++i)
    {
        const RGBE& rgbe = data[i];
        int x = i % width;
        int y = flipY ? ((height - 1) - (i / width)) : (i / width);
        dvec3& col = image[x + y * width];

        int e = (int)rgbe.e - 128;
        float maxRGB = std::max(rgbe.r, std::max(rgbe.g, rgbe.b));
        // maxRGB = maxV * v = maxV * frexp(maxV, &e) * 256 / maxV = frexp(maxV, &e) * 256
        // frexp(maxV, &e) = maxRGB / 256
        float frexp_maxV_e = maxRGB / 256.0f;
        // maxV = frexp_maxV_e * 2^e
        float maxV = frexp_maxV_e * powf(2, e);
        // maxRGB = maxV * v
        float v = maxRGB / maxV;

        if (maxV != 0) {
            col.x = rgbe.r / v;
            col.y = rgbe.g / v;
            col.z = rgbe.b / v;
        } else {
            col.x = 0;
            col.y = 0;
            col.z = 0;
        }
    }

    fclose(fp);

    return {image, width, height};
}

void SaveHDR(const char* filename, const dvec3* inImage, int width, int height, bool psf)
{
  FILE *fp;
  fp = fopen(filename, "wb");
  if (psf)
  {
    width *= 2;
  }
  if (fp)
  {
    size_t nmemb = width * height;
    RGBE *data = new RGBE[nmemb];
    for (int ii = 0; ii < nmemb; ii++)
    {
      RGBE &rgbe = data[ii];
      int x = (ii % width);
      int y = height - (ii / width) - 1;
      dvec3 vv;
      vv = inImage[y * width + x];
      if (psf)
      {
        if (x < width)
        {
          vv = inImage[y * width + x];
        }
        else
        {
          x -= width;
          double w = Globals::weight.data()[y * width + x];
          if (Globals::showBargraph && (x > 0.98 * width))
            w = (double)y / height; // thin bar on the right showing the mapping
          if (Globals::rainbowPSC)
            getPseudocolorRainbow(w, 0.0, 1.0, vv.x, vv.y, vv.z); // is more common but wrong perceptually
          else
            getPseudocolorCoolWarm(w, 0.0, 1.0, vv.x, vv.y, vv.z); // is perceptually better
        }
      }
      float v;
      int e;
      v = vv.x;
      if (vv.y > v)
        v = vv.y;
      if (vv.z > v)
        v = vv.z;
      if (v < 1e-32)
      {
        rgbe.r = rgbe.g = rgbe.b = rgbe.e = 0x0;
      }
      else
      {
        v = (float)(frexp(v, &e) * 256.0 / v);
        rgbe.r = (unsigned char)(vv.x * v);
        rgbe.g = (unsigned char)(vv.y * v);
        rgbe.b = (unsigned char)(vv.z * v);
        rgbe.e = (unsigned char)(e + 128);
      }
    }
    fflush(stdout);
    const char *programtype = "RADIANCE";
    if (fprintf(fp, "#?%s\n", programtype) < 0)
    {
      abort();
    }
    float gamma = 2.2;
    float exposure = 1.0;
    if (fprintf(fp, "GAMMA=%g\n", gamma) < 0)
    {
      abort();
    }
    if (fprintf(fp, "EXPOSURE=%g\n", exposure) < 0)
    {
      abort();
    }
    if (fprintf(fp, "FORMAT=32-bit_rle_rgbe\n\n") < 0)
    {
      abort();
    }
    if (fprintf(fp, "-Y %d +X %d\n", height, width) < 0)
    {
      abort();
    }
    // Write data
    size_t kk = fwrite(data, (size_t)4, nmemb, fp);
    fclose(fp);
    if (kk != nmemb)
    {
      printf("ERROR - was not able to save all kk=%d entries to file, exiting\n",
              (int)nmemb);
      fflush(stdout);
      abort(); // error
    }
  }
}