
struct RGBE
{
  unsigned char r, g, b, e;
};

struct Image
{
  std::vector<vec3> data;
  int width;
  int height;
};

Image ReadHDR(const char* filename);

void SaveHDR(const char* filename, const vec3* inImage, int width, int height, bool psf = false);