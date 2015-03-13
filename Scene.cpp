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
#include "Camera.h"
#include "Light.h"

#include "lodepng.h"

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


//****************************************************
// Forward Declarations
//****************************************************
static void printRay(Ray ray);
static void printSample(Sample sample);
static void printColor(Color color);
void printCommandLineOptionVariables();
void printSamples();
Color applyShadingModel(DifferentialGeometry differentialGeometry, BRDFCoefficients brdf, Ray lightRay, Color lightColor);
class RayTracer;


//****************************************************
// Global Variables
//****************************************************
bool debug;
std::vector<Sample> samples;
Film film;
Camera camera;
int recursionDepth;
AggregatePrimitive aggregatePrimitive;
std::vector<PointLight> point_lights;
std::vector<DirectionalLight> directional_lights;

class RayTracer {
	public:
		void trace(Ray& ray, int depth, Color* color) {
			if (depth > recursionDepth) {
				color->r = 0;
				color->g = 0;
				color->b = 0;
				return;
			}

//			// For testing only: shade coordinate red if ray intersects it, else shade black
//			if (!aggregatePrimitive.intersectP(ray)) {
//				color->r = 0;
//				color->g = 0;
//				color->b = 0;
//				return;
//			} else {
//				//For testing purposes, simply shade red if the ray intersects the point
//				color->r = 255;
//				color->g = 0;
//				color->b = 0;
//			}

			// TODO: this may be wrong
			float tHit = FLT_MIN;
			Intersection intersection;

			// This method will populate tHit and intersection if there is an intersection with this ray and any primitive.
			if (!aggregatePrimitive.intersect(ray, &tHit, &intersection) ) {
				// If no intersection, then make the color black and return
				color->r = 0;
				color->g = 0;
				color->b = 0;
				return;
			}

			BRDFCoefficients brdf;
			// This method will populate the brdf variable with the brdf values of the intersection primitive.
			brdf = intersection.primitive->getBRDF(intersection.differentialGeometry, &brdf);

			// Initialize a new Color with R = G = B = 0.0
			// This color will be appended to with our shading model
			Color colorOfPixel;
			Ray lightRay;
			Color lightColor;

			float intersectionXCoor = intersection.differentialGeometry.position.x;
			float intersectionYCoor = intersection.differentialGeometry.position.y;
			float intersectionZCoor = intersection.differentialGeometry.position.z;

			// There is an intersection, so we have to loop through all the light source
			// and consider their contributions to the intersection pixel
			for (std::vector<DirectionalLight>::size_type i = 0; i < directional_lights.size(); i++) {

				// Generate light ray from the intersection point to the light position
				// For directional lights, this is generated by subtracting the position of the intersection point
				// from the position of the light (IN WORLD COORDINATES)
				directional_lights[i].generateLightRay(intersection.differentialGeometry, &lightRay, &lightColor);

				// if this light ray is not blocked, do:
				//     *color += shading(intersection.differentialGeometry, brdf, lightRay, lightColor);

				// For now, we just ignore shadows and reflections and just apply our shading model
				Color colorToAdd = applyShadingModel(
						intersection.differentialGeometry,
						brdf,
						lightRay,
						Color(directional_lights[i].r, directional_lights[i].g, directional_lights[i].b));

				color->r += colorToAdd.r;
				color->g += colorToAdd.g;
				color->b += colorToAdd.b;

			}

			for (std::vector<PointLight>::size_type i = 0; i < point_lights.size(); i++) {
				// TODO:
				// Generate light ray from the intersection point to the light position
				// point_lights[i].generateLightRay(intersection.differentialGeometry, &lightRay, &lightColor);
				// if this light ray is not blocked, do:
				//     *color += shading(intersection.differentialGeometry, brdf, lightRay, lightColor);

				// For now, we just ignore shadows and reflections and just apply our shading model
				// i.e. just call:
				Color colorToAdd = applyShadingModel(intersection.differentialGeometry, brdf, lightRay, lightColor);
				color->r += colorToAdd.r;
				color->g += colorToAdd.g;
				color->b += colorToAdd.b;
			}

			// TODO: Handle reflection rays

		}
};

// (Declaration of RayTracer global variable)
RayTracer rayTracer;


//****************************************************
// applies Phong shading model to differentialGeometry.position
//****************************************************
Color applyShadingModel(DifferentialGeometry differentialGeometry, BRDFCoefficients brdf, Ray lightRay, Color lightColor) {

	// ***** BEGIN COMPUTATION OF PHONG SHADING MODEL ***** //

	float resultant_rgb_sum_of_pixel_r = 0;
	float resultant_rgb_sum_of_pixel_g = 0;
	float resultant_rgb_sum_of_pixel_b = 0;

	// ****(?)****  Set viewer vector
	// (?) is it (0, 0, 0) or
	Vector3 viewer_vector = Vector3(0, 0, 0);
	// (?) -1 * incoming_ray's_vector?
	// Vector3 viewer_vector = Vector3(-1.0 * differentialGeometry.position.x, -1.0 * differentialGeometry.position.y, -1.0 * differentialGeometry.position.z);

	// **************************************
    // For directional light
	// **************************************
	if (lightRay.t_max == FLT_MAX) {
		// Calculate ambient term -- UNCHANGED
		float directional_ambient_r = brdf.ka.r * lightColor.r;
		float directional_ambient_g = brdf.ka.r * lightColor.g;
		float directional_ambient_b = brdf.ka.r * lightColor.b;

		// Light vector is no longer this...
		// ****(?)****
		Vector3 prenormalized_directional_light_vector = lightRay.direction;

		// ****(?)**** Change orientation of light vector to point inwards to sphere
		Vector3 directional_light_vector = Vector3::normalizeVector(prenormalized_directional_light_vector.scaleVector(-1));

		// TODO: Need to change -- (x, y, z) is in world coordinates now, not relative to center of sphere
		// NOTE: we should defer this logic to Sphere/Triangle, as follows:
		Vector3 directional_normal_vector = Vector3(differentialGeometry.normal.x, differentialGeometry.normal.y, differentialGeometry.normal.z);
		//	  Vector3 prenormalized_directional_normal_vector = Vector3(x, y, z);
		//	  Vector3 directional_normal_vector = Vector3::normalizeVector(prenormalized_directional_normal_vector);

		float directional_diffuse_dot_product = fmax(directional_light_vector.dotProduct(directional_normal_vector), 0);
		float directional_diffuse_r = brdf.kd.r * lightColor.r * directional_diffuse_dot_product;
		float directional_diffuse_g = brdf.kd.g * lightColor.g * directional_diffuse_dot_product;
		float directional_diffuse_b = brdf.kd.b * lightColor.b * directional_diffuse_dot_product;

		// Calculate specular term
		Vector3 directional_reflective_vector = directional_normal_vector.scaleVector(directional_light_vector.dotProduct(directional_normal_vector) * 2).subtractVector(directional_light_vector);
		float directional_specular_dot_product_term = pow(fmax(directional_reflective_vector.dotProduct(viewer_vector), 0), brdf.sp);
		float directional_specular_r = brdf.ks.r * lightColor.r * directional_specular_dot_product_term;
		float directional_specular_g = brdf.ks.g * lightColor.g * directional_specular_dot_product_term;
		float directional_specular_b = brdf.ks.b * lightColor.b * directional_specular_dot_product_term;

		// Combine three contributions together
		resultant_rgb_sum_of_pixel_r += (directional_ambient_r + directional_diffuse_r + directional_specular_r);
		resultant_rgb_sum_of_pixel_g += (directional_ambient_g + directional_diffuse_g + directional_specular_g);
		resultant_rgb_sum_of_pixel_b += (directional_ambient_b + directional_diffuse_b + directional_specular_b);
	} else {
		// **************************************
		// For point light
		// **************************************

		// Calculate ambient term
		float point_ambient_r = brdf.ka.r * lightColor.r;
		float point_ambient_g = brdf.ka.g * lightColor.g;
		float point_ambient_b = brdf.ka.b * lightColor.b;

		// Calculate diffuse term

		// Location of point light given by command line options (i.e. x, y, z)
		Vector3 normalized_point_light_location = lightRay.direction;

		Vector3 point_normal_vector = Vector3(differentialGeometry.normal.x, differentialGeometry.normal.y, differentialGeometry.normal.z);

		Vector3 prenormalized_point_light_vector = normalized_point_light_location.subtractVector(point_normal_vector).scaleVector(1);
		Vector3 point_light_vector = Vector3::normalizeVector(prenormalized_point_light_vector);

		float point_diffuse_dot_product = fmax(point_light_vector.dotProduct(point_normal_vector), 0);
		float point_diffuse_r = brdf.kd.r * lightColor.r * point_diffuse_dot_product;
		float point_diffuse_g = brdf.kd.g * lightColor.g * point_diffuse_dot_product;
		float point_diffuse_b = brdf.kd.b * lightColor.b * point_diffuse_dot_product;

		// Calculate specular term
		Vector3 point_reflective_vector = point_normal_vector.scaleVector(point_light_vector.dotProduct(point_normal_vector) * 2).subtractVector(point_light_vector);
		float point_specular_dot_product_term = pow(fmax(point_reflective_vector.dotProduct(viewer_vector), 0), brdf.sp);
		float point_specular_r = brdf.ks.r * lightColor.r * point_specular_dot_product_term;
		float point_specular_g = brdf.ks.g * lightColor.g * point_specular_dot_product_term;
		float point_specular_b = brdf.ks.b * lightColor.b * point_specular_dot_product_term;

		// Combine three contributions together
		resultant_rgb_sum_of_pixel_r += (point_ambient_r + point_diffuse_r + point_specular_r);
		resultant_rgb_sum_of_pixel_g += (point_ambient_g + point_diffuse_g + point_specular_g);
		resultant_rgb_sum_of_pixel_b += (point_ambient_b + point_diffuse_b + point_specular_b);
	}

	return Color(resultant_rgb_sum_of_pixel_r, resultant_rgb_sum_of_pixel_g, resultant_rgb_sum_of_pixel_b);
}


//****************************************************
// Debug printing functions
//****************************************************

static void printRay(Ray ray) {
	if (debug) {
		printf("Ray: (%f, %f, %f) + t(%f, %f, %f)\n", ray.position.x, ray.position.y, ray.position.z, ray.direction.x, ray.direction.y, ray.direction.z);
	}
}

static void printSample(Sample sample) {
	if (debug) {
		std::cout << "Sample: x = " << sample.x << "; y = " << sample.y << "\n";
	}
}

static void printColor(Color color) {
	if (debug) {
		printf("Color: (r, g, b) = (%f, %f, %f)\n", color.r, color.g, color.b);
	}
}

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
    for (std::vector<DirectionalLight>::size_type i = 0; i < directional_lights.size(); i++)
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
    for (std::vector<PointLight>::size_type i = 0; i < point_lights.size(); i++)
    {
      std::cout << "  " << "Light " << (i + 1) << " (index " << i << " of list):\n";
      std::cout << "     " << "x: " << point_lights[i].x << " y: " << point_lights[i].x << " z: " << point_lights[i].x << "\n";
      std::cout << "     " << "r: " << point_lights[i].r << " g: " << point_lights[i].g << " b: " << point_lights[i].b << "\n";
    }

    std::cout << "***** FINISH PRINTING COMMAND LINE OPTION VARIABLES *****\n\n";
  }
}

// Prints contents of samples and buckets for debug purposes
void printSamples() {
	if (debug) {
		for (vector<Sample>::size_type i = 0; i < samples.size(); i++) {
			printSample(samples[i]);
		}
	}
}


//****************************************************
// Parsing of command line options, with options:
// (1) -dimensions width height
//     adds viewport width and height attributes to Viewport global variable
// (2) -depth n
//     sets recursion depth
// (3) -dl x y z r g b
//     (directional lights)
// (4) -pl x y z r g b
//     (point lights)
//
// NOTE: also performs proper initialization of Film and Camera global variables
//****************************************************
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
	  film = Film(widthOfFilm, heightOfFilm);
	  camera = Camera(widthOfFilm, heightOfFilm);
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
      directional_lights.push_back( DirectionalLight( stof(argv[i+1]), stof(argv[i+2]), stof(argv[i+3]), stof(argv[i+4]), stof(argv[i+5]), stof(argv[i+6]) ) );
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

      point_lights.push_back( PointLight( stof(argv[i+1]), stof(argv[i+2]), stof(argv[i+3]), stof(argv[i+4]), stof(argv[i+5]), stof(argv[i+6]) ) );
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


//****************************************************
// Main rendering loop
//
// Loops through samples, and does the following per sample:
// (1) generates a ray from the eye through the sample
// (2) traces this ray with the ray tracer
// (3) commits the color returns by the ray tracer to the film
//****************************************************
void render() {
	// Loop through all of the samples...
	for (vector<Sample>::size_type i = 0; i < samples.size(); i++) {

		// For each sample, generate a ray from the eye to the sample location
		Ray currentRay;
		Color currentSampleColor;

		// Given the sample in Film-coordinates, tell the camera to generate a viewing ray in IMAGE PLANE [-1, 1] coordinates
		camera.generateRay(samples[i], &currentRay);

		// Call the trace method to try to populate currentSampleColor for the currentSample
		rayTracer.trace(currentRay, recursionDepth, &currentSampleColor);

		// Commit the currentSampleColor for the currentSample onto our Film
		film.commitColor(samples[i], currentSampleColor);
	}

}


//****************************************************
// Populates a list of samples based on the film height and width
//****************************************************
void initializeSampler() {
	float x, y;

	float widthOfBucket = 2.0 / film.width;
	float heightOfBucket = 2.0 / film.height;

	// Generates samples in the FILM's coordinates
	for (y = 0; y < film.height; y++) {
		for (x = 0; x < film.width; x++) {
			samples.push_back(Sample(x, y));
		}
	}

}


//****************************************************
// Initializes primitives for our scene
//****************************************************
void initializePrimitives() {
	// Must use 'new', as need to store pointers to variables that will live outside of the scope of this function
	Sphere *sphere1 = new Sphere(0, 0, -2, 1);
	Material *material1 = new Material();
	BRDFCoefficients *brdf = new BRDFCoefficients();

	// ka
	Color *color1 = new Color(11, 11, 60);

	// kd
	Color *color2 = new Color(255, 185, 255);

	// ks
	Color *color3 = new Color(255, 255, 255);

	brdf->ka = *color1;
	brdf->kd = *color2;
	brdf->ks = *color3;
	brdf->sp = 4.0;
	material1->constantBRDF = *brdf;

	GeometricPrimitive *primitive1 = new GeometricPrimitive();

	primitive1->shape = sphere1;
	primitive1->material = material1;

	aggregatePrimitive.addPrimitive(primitive1);
}


//****************************************************
// Main function
//****************************************************
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

  film.writeImage("ray_tracer_output.png");
  return 0;
};

