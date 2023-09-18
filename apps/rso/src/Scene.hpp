

// The scene definition with main rendering method
class Scene
{
  std::vector<Intersectable *> objects;
  double totalPower;
  int nLightSamples, nBRDFSamples;

public:
  Camera camera;

  void buildHw1_3Test();

  void buildHw2Test(const char* hdrFilename);

  void buildHw4Test(const char* hdrFilename);

  void build(const char* hdrFilename = "raw013.hdr");

  // Set the weight for the sampling method
  void setWeight(double wval);

  // Render the scene
  void render();

  // Compute intersection between a rady and primitive
  Hit firstIntersect(const Ray &ray, Intersectable *skip);

  // Sample the light source from all the light sources in the scene
  LightSource sampleLightSource(const vec3 &illuminatedPoint);

  vec3 pathTrace(const Ray &primaryRay);

  // Trace a primary ray towards the scene
  vec3 trace(const Ray &r);

  // Only testing routine for debugging
  void testRay(int X, int Y);
};