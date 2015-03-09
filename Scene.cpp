#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <limits>
#include <cfloat>

#include "Vector3.h"
#include "Vector4.h"
#include "Sample.h"
#include "Point.h"
#include "Normal.h"
#include "Matrix4.h"
#include "Transformation.h"
#include "Ray.h"
#include "DifferentialGeometry.h"
#include "Color.h"
#include "BRDFCoefficients.h"
#include "Material.h"
#include "Shape.h"
#include "Primitive.h"
#include "Intersection.h"
#include "AggregatePrimitive.h"
#include "Film.h"
#include "Bucket.h"
#include "Camera.h"
#include "Light.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <bitset>
#include <algorithm>

using namespace std;

inline float sqr(float x) { return x*x; }


//****************************************************
// Some Classes
//****************************************************

class Viewport;

class Viewport {
public:
	int w, h;

};

// NOTE: in-class initialization of non-static data member is a C++11 extension (might have to remove)
class Ka {
  public:
    float r, g, b;
};

class Kd {
  public:
    float r, g, b;
};

class Ks {
  public:
    float r, g, b;
};

class Sp {
  public:
    float v = 1;
};

class Pl {
  public:
    float x, y, z, r, g, b;

  Pl(float x, float y, float z, float r, float g, float b) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->g = g;
    this->b = b;
  }
};

class Dl {
  public:
    float x, y, z, r, g, b;

  Dl(float x, float y, float z, float r, float g, float b) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->g = g;
    this->b = b;
  }
};


//****************************************************
// Global Variables
//****************************************************


// REAL GLOBAL VARIABLES
std::vector<Bucket> listOfBuckets;
Film film;
Camera camera;
int recursionDepth;
std::vector<Light> lights;
AggregatePrimitive aggregatePrimitive;

class RayTracer {
	public:

		void trace(Ray& ray, int depth, Color* color) {
			if (depth > recursionDepth) {
				color->r = 0;
				color->g = 0;
				color->b =0;
				return;
			}
			float tHit;
			Intersection intersection;
			// This method will populate tHit and intersection if there is an intersection with this ray and any primitive.
			if (!aggregatePrimitive.intersect(ray, &tHit, &intersection) ) {
				color->r = 0;
				color->g = 0;
				color->b =0;
				return;
			}
			BRDFCoefficients brdf;
			// This method will populate the brdf variable with the brdf values of the intersection primitive.
			intersection.primitive->getBRDF(intersection.differentialGeometry, &brdf);

			//There is an intersection, so we have to loop through all the light sources

		}
};

RayTracer rayTracer;

Viewport viewport;











// TODO: Move these into appropriate classes
//***** The following are set by command line options *****//
// Ambient term
Ka ka;
// Diffuse term
Kd kd;
// Specular term
Ks ks;
// Phong exponent
Sp sp;
// List of point lights
std::vector<Pl> point_lights;
// List of directional lights
std::vector<Dl> directional_lights;

bool debug;







// TODO: Remove this!
void setPixel(int x, int y, int r, int g, int b) {

}

//****************************************************
// Draw a filled circle.
//****************************************************

// For viewport of [400 x 400], center X = 200, center Y = 200
void circle(float centerX, float centerY, float radius) {

  // We could eliminate wasted work by only looping over the pixels
  // inside the sphere's radius.  But the example is more clear this
  // way.  In general drawing an object by loopig over the whole
  // screen is wasteful.

  int i,j;  // Pixel indices

  /*
  Example:

  centerX = 200
  centerY = 200
  radius = 180

  -->

  minI = max(0, 20) = 20
  maxI = min(399, 380) = 380
  minJ = max(0, 20) = 20
  maxJ = min(399, 380) = 380
  */

  int minI = max(0,(int)floor(centerX-radius));
  int maxI = min(viewport.w-1,(int)ceil(centerX+radius));

  int minJ = max(0,(int)floor(centerY-radius));
  int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));

  for (i=0;i<viewport.w;i++) {
    for (j=0;j<viewport.h;j++) {

      // Location of the center of pixel relative to center of sphere
      float x = (i+0.5-centerX);
      float y = (j+0.5-centerY);

      float dist = sqrt(sqr(x) + sqr(y));

      // if current pixel (i, j) is inside the bounds of the circle
      if (dist<=radius) {

        // This is the front-facing Z coordinate
        float z = sqrt(radius*radius-dist*dist);

        // printf("(%f, %f, %f)\n", x, y, z);

        // (x, y, z) represents location of current pixel relative to very center of sphere (3-space)
        // i.e. n = xi + yj + zk

        // ***** BEGIN COMPUTATION OF PHONG SHADING MODEL ***** //

        // resultant_rgb_sum_of_pixel = Vector3(0, 0, 0);

        float resultant_rgb_sum_of_pixel_r = 0;
        float resultant_rgb_sum_of_pixel_g = 0;
        float resultant_rgb_sum_of_pixel_b = 0;

        // Set viewer vector to constant (0, 0, 1)
        Vector3 viewer_vector = Vector3(0, 0, 1);

        // ***** BEGIN CONSIDERING DIRECTIONAL LIGHTS ***** //
        // Iterate through each directional light...
        // Calculate ambient, diffuse, specular contributions for this light
        // Sum all 3 contributions together
        // Add contribution to resultant_rgb_sum_of_pixel
        for (std::vector<Dl>::size_type i = 0; i < directional_lights.size(); i++)
        {
          // current directional light given by directional_lights[i]

          // Calculate ambient term
          float directional_ambient_r = ka.r * directional_lights[i].r;
          float directional_ambient_g = ka.g * directional_lights[i].g;
          float directional_ambient_b = ka.b * directional_lights[i].b;

          // Calculate diffuse term
          Vector3 prenormalized_directional_light_vector = Vector3(directional_lights[i].x, directional_lights[i].y, directional_lights[i].z);

          // Change orientation of light vector to point outwards to sphere
          Vector3 directional_light_vector = Vector3::normalizeVector(prenormalized_directional_light_vector.scaleVector(-1));

          Vector3 prenormalized_directional_normal_vector = Vector3(x, y, z);
          Vector3 directional_normal_vector = Vector3::normalizeVector(prenormalized_directional_normal_vector);


          float directional_diffuse_dot_product = fmax(directional_light_vector.dotProduct(directional_normal_vector), 0);
          float directional_diffuse_r = kd.r * directional_lights[i].r * directional_diffuse_dot_product;
          float directional_diffuse_g = kd.g * directional_lights[i].g * directional_diffuse_dot_product;
          float directional_diffuse_b = kd.b * directional_lights[i].b * directional_diffuse_dot_product;

          // Calculate specular term
          Vector3 directional_reflective_vector = directional_normal_vector.scaleVector(directional_light_vector.dotProduct(directional_normal_vector) * 2).subtractVector(directional_light_vector);
          float directional_specular_dot_product_term = pow(fmax(directional_reflective_vector.dotProduct(viewer_vector), 0), sp.v);
          float directional_specular_r = ks.r * directional_lights[i].r * directional_specular_dot_product_term;
          float directional_specular_g = ks.g * directional_lights[i].g * directional_specular_dot_product_term;
          float directional_specular_b = ks.b * directional_lights[i].b * directional_specular_dot_product_term;

          // Combine three contributions together
          resultant_rgb_sum_of_pixel_r += (directional_ambient_r + directional_diffuse_r + directional_specular_r);
          resultant_rgb_sum_of_pixel_g += (directional_ambient_g + directional_diffuse_g + directional_specular_g);
          resultant_rgb_sum_of_pixel_b += (directional_ambient_b + directional_diffuse_b + directional_specular_b);
        }
        // ***** FINISH CONSIDERING DIRECTIONAL LIGHTS ***** //

        // ----------

        // ***** BEGIN CONSIDERING POINT LIGHTS ***** //
        for (std::vector<Pl>::size_type i = 0; i < point_lights.size(); i++)
        {
          // current directional light given by directional_lights[i]

          // Calculate ambient term
          float point_ambient_r = ka.r * point_lights[i].r;
          float point_ambient_g = ka.g * point_lights[i].g;
          float point_ambient_b = ka.b * point_lights[i].b;

          // Calculate diffuse term

          // Location of point light given by command line options (i.e. x, y, z)
          Vector3 normalized_point_light_location = Vector3(point_lights[i].x, point_lights[i].y, point_lights[i].z);

          Vector3 prenormalized_point_normal_vector = Vector3(x, y, z);
          Vector3 point_normal_vector = Vector3::normalizeVector(prenormalized_point_normal_vector);

          Vector3 prenormalized_point_light_vector = normalized_point_light_location.subtractVector(point_normal_vector).scaleVector(1);
          Vector3 point_light_vector = Vector3::normalizeVector(prenormalized_point_light_vector);

          float point_diffuse_dot_product = fmax(point_light_vector.dotProduct(point_normal_vector), 0);
          float point_diffuse_r = kd.r * point_lights[i].r * point_diffuse_dot_product;
          float point_diffuse_g = kd.g * point_lights[i].g * point_diffuse_dot_product;
          float point_diffuse_b = kd.b * point_lights[i].b * point_diffuse_dot_product;

          // Calculate specular term
          Vector3 point_reflective_vector = point_normal_vector.scaleVector(point_light_vector.dotProduct(point_normal_vector) * 2).subtractVector(point_light_vector);
          float point_specular_dot_product_term = pow(fmax(point_reflective_vector.dotProduct(viewer_vector), 0), sp.v);
          float point_specular_r = ks.r * point_lights[i].r * point_specular_dot_product_term;
          float point_specular_g = ks.g * point_lights[i].g * point_specular_dot_product_term;
          float point_specular_b = ks.b * point_lights[i].b * point_specular_dot_product_term;

          // Combine three contributions together
          resultant_rgb_sum_of_pixel_r += (point_ambient_r + point_diffuse_r + point_specular_r);
          resultant_rgb_sum_of_pixel_g += (point_ambient_g + point_diffuse_g + point_specular_g);
          resultant_rgb_sum_of_pixel_b += (point_ambient_b + point_diffuse_b + point_specular_b);
        }
        // ***** FINISH CONSIDERING POINT LIGHTS ***** //
        Vector3 resultant_rgb = Vector3(resultant_rgb_sum_of_pixel_r, resultant_rgb_sum_of_pixel_g, resultant_rgb_sum_of_pixel_b);
        setPixel(i, j, resultant_rgb.x, resultant_rgb.y, resultant_rgb.z);
      }
    }
  }
}



//****************************************************
// function that prints all of our command line option variables
//***************************************************
void printCommandLineOptionVariables( )
{
  if (debug)
  {
    std::cout << "\n***** BEGIN PRINTING COMMAND LINE OPTION VARIABLES *****\n";
    std::cout << "  " << "Film Width: " << film.width << "\n";
    std::cout << "  " << "Film Height: " << film.height << "\n\n";
    std::cout << "  Recursion Depth: " << recursionDepth << "\n\n";
    std::cout << "Directional Lights:\n";
    if (directional_lights.size() == 0)
    {
      std::cout << " (none)\n";
    }
    for (std::vector<Dl>::size_type i = 0; i < directional_lights.size(); i++)
    {
      std::cout << "  " << "Light " << (i + 1) << " (index " << i << " of list):\n";
      std::cout << "     " << "x: " << directional_lights[i].x << " y: " << directional_lights[i].x << " z: " << directional_lights[i].x << "\n";
      std::cout << "     " << "r: " << directional_lights[i].r << " g: " << directional_lights[i].g << " b: " << directional_lights[i].b << "\n";
    }

    std::cout << "\nPoint Lights:\n";
    if (point_lights.size() == 0)
    {
      std::cout << " (none)\n";
    }
    for (std::vector<Dl>::size_type i = 0; i < point_lights.size(); i++)
    {
      std::cout << "  " << "Light " << (i + 1) << " (index " << i << " of list):\n";
      std::cout << "     " << "x: " << point_lights[i].x << " y: " << point_lights[i].x << " z: " << point_lights[i].x << "\n";
      std::cout << "     " << "r: " << point_lights[i].r << " g: " << point_lights[i].g << " b: " << point_lights[i].b << "\n";
    }
    std::cout << "***** FINISH PRINTING COMMAND LINE OPTION VARIABLES *****\n\n";
  }
}

// (1) -dimensions width height
//     adds viewport width and height attributes to Viewport global variable
// (2)
void parseCommandLineOptions(int argc, char *argv[])
{
  string flag;

  int i = 1;
  while (i <= argc - 1) {
    flag = argv[i];

    if (flag == "-dimensions")
	{
	  // Check that -dimensions has enough option parameters
	  if ((i + 2) > (argc - 1))
	  {
		std::cout << "Invalid number of parameters for -dimensions.";
		exit(1);
	  }

	  int widthOfFilm = stoi(argv[i+1]);
	  int heightOfFilm = stoi(argv[i+2]);
//	  if (widthOfFilm < 1000 || heightOfFilm < 500 || widthOfFilm > 3000 || heightOfFilm > 3000) {
//		  std::cout << "Dimensions of output file must be at least 1000x500 and no more than 3000x3000.";
//		  exit(1);
//	  }
	  film.width = widthOfFilm;
	  film.height = heightOfFilm;
	  i += 2;
	}
    else if (flag == "-depth")
	{
	  // Check that -depth has enough option parameters
	  if ((i + 1) > (argc - 1))
	  {
		std::cout << "Invalid number of parameters for -depth.";
		exit(1);
	  }
	  recursionDepth = stoi(argv[i+1]);
	  i += 1;
	}
    else if (flag == "-dl")
    {
      // Check that -dl has enough option parameters
      if ((i + 6) > (argc - 1))
      {
        std::cout << "Invalid number of parameters for -dl.";
        exit(1);
      }

      // Add directional lights to our directional lights list
      lights.push_back( DirectionalLight( stof(argv[i+1]), stof(argv[i+2]), stof(argv[i+3]), stof(argv[i+4]), stof(argv[i+5]), stof(argv[i+6]) ) );
      i += 6;
    }
    else if (flag == "-pl")
    {
      // Check that -pl has enough option parameters
      if ((i + 6) > (argc - 1))
      {
        std::cout << "Invalid number of parameters for -pl.";
        exit(1);
      }

      lights.push_back( PointLight( stof(argv[i+1]), stof(argv[i+2]), stof(argv[i+3]), stof(argv[i+4]), stof(argv[i+5]), stof(argv[i+6]) ) );
      i += 6;
    }
    else
    {
      std::cout << "Extra parameters in command line options; terminating program.";
      exit(1);
    }

    // Advance to next flag, if one exists
    i++;
  }
}

// Prints contents of samples and buckets for debug purposes
void printContentsOfBuckets() {
	if (debug) {
		for (vector<Bucket>::size_type i = 0; i < listOfBuckets.size(); i++) {
			std::cout << "  Samples for bucket anchored at (" << listOfBuckets[i].anchorPoint.x << ", " << listOfBuckets[i].anchorPoint.y << ")\n";
			for (vector<Sample>::size_type j = 0; j < listOfBuckets[i].samples.size(); j++) {
				std::cout << "    " << "Sample: x = " << listOfBuckets[i].samples[j].x << "; y = " << listOfBuckets[i].samples[j].y << "\n";
			}
		}
	}
}

// Main rendering loop
void render() {
	// Loop through all of the samples in each bucket...
	// Current Bucket given by 'listOfBuckets[i]'
	for (vector<Bucket>::size_type i = 0; i < listOfBuckets.size(); i++) {
		// Current Sample given by 'listOfBuckets[i].samples[j]
	  	for (vector<Sample>::size_type j = 0; j < listOfBuckets[i].samples.size(); j++) {

	  		Sample currentSample = listOfBuckets[i].samples[j];

	  		// For each sample, generate a ray from the eye to the sample location
	  		Ray* currentRay;
	  		Color* currentSampleColor;
	  		camera.generateRay(currentSample, currentRay);

	  		rayTracer.trace(*currentRay, recursionDepth, currentSampleColor);

	  		film.commitColor(currentSample, *currentSampleColor);
	  	}
	}
}


// initializes list of samples
void initializeSampler() {

	// Image plane = (-1, -1) (-1, 1) (1, -1) (1, 1) with z coordinate -1
	// NOTE: Image plane width = height = 2

	float i, j;
	int numberOfBuckets = film.width * film.height;

	float widthOfBucket = 2.0 / film.width;
	float heightOfBucket = 2.0 / film.height;

	for (i = -1.0; i < 1.0; i += widthOfBucket) {
		for (j = -1.0; j < 1.0; j += heightOfBucket) {
			std::vector<Sample> currentSamples;
			currentSamples.push_back(Sample(i + (widthOfBucket / 2.0), j + (heightOfBucket / 2.0)));
			listOfBuckets.push_back(Bucket(currentSamples, Point(i, j, -1)));
		}
	}
}

void initializePrimitives() {
	Sphere sphere1 = Sphere(0, 0, -2, 1);
	GeometricPrimitive primitive1;
	primitive1.shape = &sphere1;
	aggregatePrimitive.addPrimitive(&primitive1);
}


int main(int argc, char *argv[]) {

  // Turns debug mode ON or OFF
  debug = true;

  // Parse command line options
  parseCommandLineOptions(argc, argv);
  printCommandLineOptionVariables();
  // Initializes list of buckets; Buckets have a list of samples
  initializeSampler();
  initializePrimitives();
  render();


//  printContentsOfBuckets();
  return 0;
};








