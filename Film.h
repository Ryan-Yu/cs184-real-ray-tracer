#ifndef FILM_H_
#define FILM_H_

#include "lodepng.h"

// Class that represents the output of our ray tracing
// The output is represented by a 2D array of Colors.
class Film {

	static void printSample(Sample sample) {
		std::cout << "Sample: x = " << sample.x << "; y = " << sample.y << "\n";
	}

	public:
		std::vector< Color > filmOutput;
		int width, height;
		std::vector<unsigned char> rawImage;

	Film() {

	}

	Film(int width, int height) {
		this->width = width;
		this->height = height;
	}

	void commitColor(Sample& sample, Color& color) {
		// Assuming sample.x and sample.y are coordinates in the image plane (i.e. [-1, 1]) coordinate system,
		// we must convert sample.x and sample.y to the coordinate system of the film before we populate
		// the Film's corresponding coordinate in our 2D array

		// Convert [-1, 1] coordinate system sample to [width, height] coordinate systems sample
		// NOTE: This only works for one sample per bucket, centered in the middle of the bucket index;
		// NOTE: we might need to convert Sample coordinates to image plane coordinates to include
		// more than one sample per bucket

		// First, grab width and height of each bucket...
		float widthOfBucket = 2.0 / width;
		float heightOfBucket = 2.0 / height;

		// Given this information, calculate the bottom left index of the bucket that the sample is in...
		Sample bottomLeftIndexOfSample = Sample(sample.x - (widthOfBucket / 2.0), sample.y - (heightOfBucket / 2.0));

		// Scale the sample appropriately to [width, height] coordinate system
		Sample scaledFinalSample = Sample(ceil((bottomLeftIndexOfSample.x + 1) * (width / 2.0)), ceil((bottomLeftIndexOfSample.y + 1) * (height / 2.0)));

		filmOutput.push_back(Color(color.r, color.g, color.b));

		std::cout << "In film, final point is: ";
		printSample(scaledFinalSample);

	}


	void convertToRawData() {
		rawImage.resize(4 * width * height);
		for(unsigned y = 0; y < height; y++) {
			for(unsigned x = 0; x < width; x++) {
				rawImage[4 * width * y + 4 * x + 0] = 255;
				rawImage[4 * width * y + 4 * x + 1] = 255;
				rawImage[4 * width * y + 4 * x + 2] = 255;
				rawImage[4 * width * y + 4 * x + 3] = 255;
			}

		}

//		for (std::vector<Color>::size_type i = 0; i < filmOutput.size(); i++) {
//			rawImage[(width * height * i) + 0] = 50;
//			rawImage[(width * height * i) + 1] = 50;
//			rawImage[(width * height * i) + 2] = 50;
//			rawImage[(width * height * i) + 3] = 255;
//			rawImage[(width * height * i) + 0] = filmOutput[i].r;
//			rawImage[(width * height * i) + 1] = filmOutput[i].g;
//			rawImage[(width * height * i) + 2] = filmOutput[i].b;
//			rawImage[(width * height * i) + 3] = 255.0;
//		}
	}


	// First converts our 2D array of color into raw data so LodePNG can use it
	// Then encodes the image to a file with LodePNG
	// Method signature of lodepng::encode:
	// void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height)
	void writeImage() {
		convertToRawData();
		const char* filename = "test.png";
		// Encode the image so LodePNG can play with it
		unsigned error = lodepng::encode(filename, rawImage, width, height);
//		//if there's an error, display it
	    if (error) {
	    	std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
	    }
	    std::cout << "Done writing.";
	}
};

#endif /* FILM_H_ */
