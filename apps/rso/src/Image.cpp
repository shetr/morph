#include "Image.hpp"

#include <iostream>

namespace Morph {

double average(dvec3 v)
{
  return dot(v, dvec3(1)) / 3.0;
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

void SaveHDR(const char* filename, const vector2d<vec3>& inImage)
{
  FILE *fp;
  fp = fopen(filename, "wb");
  if (fp)
  {
    size_t nmemb = inImage.size();
    RGBE *data = new RGBE[nmemb];
    for (int ii = 0; ii < nmemb; ii++)
    {
      RGBE &rgbe = data[ii];
      int x = (ii % inImage.dim().x);
      int y = inImage.dim().y - (ii / inImage.dim().x) - 1;
      dvec3 vv;
      vv = inImage(x, y);
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
    if (fprintf(fp, "-Y %d +X %d\n", inImage.dim().y, inImage.dim().x) < 0)
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

void SaveTGA(const char* filename, const vector2d<vec3>& inImage)
{
  // Save TGA file for the image, simple format
  FILE *ofile = 0;
  ofile = fopen(filename, "wb");
  if (!ofile)
    return;

  fputc(0, ofile);
  fputc(0, ofile);
  fputc(2, ofile);
  for (int i = 3; i < 12; i++)
  {
    fputc(0, ofile);
  }
  fputc(inImage.dim().x % 256, ofile);
  fputc(inImage.dim().x / 256, ofile);
  fputc(inImage.dim().y % 256, ofile);
  fputc(inImage.dim().y / 256, ofile);
  fputc(24, ofile);
  fputc(32, ofile);

  for (int y = inImage.dim().y - 1; y >= 0; y--)
  {
    for (int x = 0; x < inImage.dim().x; x++)
    {
      double r, g, b;
      r = inImage(x, y).x;
      g = inImage(x, y).y;
      b = inImage(x, y).z;
      int R = fmax(fmin(r * 255.5, 255), 0);
      int G = fmax(fmin(g * 255.5, 255), 0);
      int B = fmax(fmin(b * 255.5, 255), 0);
      fputc(B, ofile);
      fputc(G, ofile);
      fputc(R, ofile);
    }
  }
  fclose(ofile);
}

}