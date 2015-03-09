#ifndef FILM_H_
#define FILM_H_

// Class that represents the output of our ray tracing
// The output is represented by a 2D array of Colors.
class Film {

	public:
		std::vector< std::vector< Color > > filmOutput;
		int width, height;

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

	}

	// Writes our film to an image file
	void writeImage() {

	}
};

#endif /* FILM_H_ */
