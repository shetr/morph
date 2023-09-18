#include "Image.hpp"

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

    std::vector<vec3> image;
    image.resize(count);

    size_t kk = fread(data.data(), (size_t)4, count, fp);

    const bool flipY = true;

    for (int i = 0; i < count; ++i)
    {
        const RGBE& rgbe = data[i];
        int x = i % width;
        int y = flipY ? ((height - 1) - (i / width)) : (i / width);
        vec3& col = image[x + y * width];

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

void SaveHDR(const char* filename, const vec3* inImage, int width, int height, bool psf = false)
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
      vec3 vv;
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
          double w = weight[y * width + x];
          if (showBargraph && (x > 0.98 * width))
            w = (double)y / height; // thin bar on the right showing the mapping
          if (rainbowPSC)
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