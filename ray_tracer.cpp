
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <limits>

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

#include "vector3.h"

inline float sqr(float x) { return x*x; }

using namespace std;

//****************************************************
// Some Classes
//****************************************************

class Viewport;

class Viewport {
  public:
    int w, h; // width and height
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
Viewport	viewport;

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
vector<Pl> point_lights;
// List of directional lights
vector<Dl> directional_lights;

bool debug;



//****************************************************
// helper function that normalizes a vector and returns the normalized vector
//****************************************************
Vector3 normalizeVector(Vector3 vector_to_normalize) {

  float magnitude = sqrt(pow(vector_to_normalize.x, 2) + pow(vector_to_normalize.y, 2) + pow(vector_to_normalize.z, 2));
  // printf("The vector that we are trying to normalize is (%f, %f, %f) and its magnitud e is: %f\n", vector_to_normalize.x, vector_to_normalize.y, vector_to_normalize.z, magnitude);
  return Vector3(vector_to_normalize.x / magnitude, vector_to_normalize.y / magnitude, vector_to_normalize.z / magnitude);
}

// Integer to bits representation
// (via http://stackoverflow.com/questions/2686542/converting-integer-to-a-bit-representation)
vector<int> convert_to_bits(int x) {
  vector<int> ret;
  while(x) {
    if (x&1)
      ret.push_back(1);
    else
      ret.push_back(0);
    x>>=1;  
  }
  reverse(ret.begin(),ret.end());
  return ret;
}

// Bits to integer representation
// (via http://stackoverflow.com/questions/18938831/convert-vector-of-binary-integers-to-vector-of-digits)
int convert_to_int(const vector<int>& x) {
  int result = 0;
  int base = 1;
  for (unsigned int i = x.size()-1; i > 0; --i)
  {
    result += x[i]*base;
    base *= 2;
  }
  result += x[0] * base;
  return result;
}


//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************

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
        for (vector<Dl>::size_type i = 0; i < directional_lights.size(); i++)
        {
          // current directional light given by directional_lights[i]

          // Calculate ambient term
          float directional_ambient_r = ka.r * directional_lights[i].r;
          float directional_ambient_g = ka.g * directional_lights[i].g;
          float directional_ambient_b = ka.b * directional_lights[i].b;

          // Calculate diffuse term
          Vector3 prenormalized_directional_light_vector = Vector3(directional_lights[i].x, directional_lights[i].y, directional_lights[i].z);

          // Change orientation of light vector to point outwards to sphere
          Vector3 directional_light_vector = normalizeVector(prenormalized_directional_light_vector.scaleVector(-1));

          Vector3 prenormalized_directional_normal_vector = Vector3(x, y, z);
          Vector3 directional_normal_vector = normalizeVector(prenormalized_directional_normal_vector);


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
        for (vector<Pl>::size_type i = 0; i < point_lights.size(); i++)
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
          Vector3 point_normal_vector = normalizeVector(prenormalized_point_normal_vector);

          Vector3 prenormalized_point_light_vector = normalized_point_light_location.subtractVector(point_normal_vector).scaleVector(1);
          Vector3 point_light_vector = normalizeVector(prenormalized_point_light_vector);

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
        // setPixel(i,j, resultant_rgb_sum_of_pixel_r, resultant_rgb_sum_of_pixel_g, resultant_rgb_sum_of_pixel_b);
      }

    }
  }
}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

  // Start drawing
  circle(viewport.w / 2.0 , viewport.h / 2.0 , min(viewport.w, viewport.h) * 0.45);
}



//****************************************************
// function that assists with exit of program on space bar press
//***************************************************
void exitOnSpaceBarPress( unsigned char key, int x, int y )
{
    switch ( key ) 
    {
    // Space bar
    case ' ':
        exit(1);
    }
}


//****************************************************
// function that prints all of our command line option variables
//***************************************************
void printCommandLineOptionVariables( )
{
  if (debug)
  {
    cout << "\n***** BEGIN PRINTING COMMAND LINE OPTION VARIABLES *****\n";

    cout << "Directional Lights:\n";
    if (directional_lights.size() == 0)
    {
      cout << " (none)\n";
    }
    for (vector<Dl>::size_type i = 0; i < directional_lights.size(); i++)
    {
      cout << "  " << "Light " << (i + 1) << " (index " << i << " of list):\n";
      cout << "     " << "x: " << directional_lights[i].x << " y: " << directional_lights[i].x << " z: " << directional_lights[i].x << "\n";
      cout << "     " << "r: " << directional_lights[i].r << " g: " << directional_lights[i].g << " b: " << directional_lights[i].b << "\n";
    }

    cout << "\nPoint Lights:\n";
    if (point_lights.size() == 0)
    {
      cout << " (none)\n";
    }
    for (vector<Dl>::size_type i = 0; i < point_lights.size(); i++)
    {
      cout << "  " << "Light " << (i + 1) << " (index " << i << " of list):\n";
      cout << "     " << "x: " << point_lights[i].x << " y: " << point_lights[i].x << " z: " << point_lights[i].x << "\n";
      cout << "     " << "r: " << point_lights[i].r << " g: " << point_lights[i].g << " b: " << point_lights[i].b << "\n";
    }
    cout << "***** FINISH PRINTING COMMAND LINE OPTION VARIABLES *****\n\n";
  }
}


//****************************************************
// function that parses command line options,
// given number of command line arguments (argc)
// and the argument array (argv)
//***************************************************
void parseCommandLineOptions(int argc, char *argv[])
{
  string flag;

  int i = 1;
  while (i <= argc - 1) {

    flag = argv[i];
    if (flag == "-dl")
    {
      // Check that -dl has enough option parameters
      if ((i + 6) > (argc - 1))
      {
        cout << "Invalid number of parameters for -dl.";
        exit(1);
      }

      // Add directional lights to our directional lights list
      if (directional_lights.size() < 5)
      {
        directional_lights.push_back( Dl( stof(argv[i+1]), stof(argv[i+2]), stof(argv[i+3]), stof(argv[i+4]), stof(argv[i+5]), stof(argv[i+6]) ) );
      }
      else
      {
        // We already have 5 directional lights, so we can't add any more
        cout << "Too many directional lights.";
        exit(1);
      }

      i += 6;
    }
    else if (flag == "-pl")
    {
      // Check that -pl has enough option parameters
      if ((i + 6) > (argc - 1))
      {
        cout << "Invalid number of parameters for -pl.";
        exit(1);
      }
      
      // Add point lights to our point lights list
      if (point_lights.size() < 5)
      {
        point_lights.push_back( Pl( stof(argv[i+1]), stof(argv[i+2]), stof(argv[i+3]), stof(argv[i+4]), stof(argv[i+5]), stof(argv[i+6]) ) );
      }
      else
      {
        // We already have 5 point lights, so we can't add any more
        cout << "Too many point lights.";
        exit(1);
      }

      i += 6;
    }
    else
    {
      cout << "Extra parameters in command line options; terminating program.";
      exit(1);
    }

    // Advance to next flag, if one exists
    i++;
  }
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

  // Turns debug mode ON or OFF
  debug = true;

  // Parse command line options
  parseCommandLineOptions(argc, argv);
  printCommandLineOptionVariables();

  // Initalize theviewport size
  viewport.w = 400;
  viewport.h = 400;

  Vector3 vect3 = Vector3(2, 2, 2);

  return 0;
}








