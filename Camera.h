#ifndef CAMERA_H_
#define CAMERA_H_

static void printRay(Ray ray);

static void printSample(Sample sample);

static void printVector(Vector3 vector);

class Camera {
	public:
		// Note: Camera needs Film width and height to correctly compute viewing ray
		int viewingPlaneWidth, viewingPlaneHeight;

		Camera() {

		}

		Camera(int widthOfFilm, int heightOfFilm) {
			this->viewingPlaneWidth = widthOfFilm;
			this->viewingPlaneHeight = heightOfFilm;
		}

		// Given a sample in FILM coordinates, this method generates a ray from the eye (0, 0, 0)
		// to the sample in IMAGE PLANE coordinates (i.e. [-1, 1])
		void generateRay(Sample& sample, Ray* ray) {

			Point eye = Point(0.0, 0.0, 0.0);

			// z-coordinates must be the same
			// Left coordinates must align vertically
			// Right coordinates must align vertically
			// Top coordinates must align horizontally
			// Bottom coordinates must align horizontally
			Point imagePlaneTopLeft = Point(-1.0, 1.0, -3.0);
			Point imagePlaneBottomLeft = Point(-1.0, -1.0, -3.0);
			Point imagePlaneTopRight = Point(1.0, 1.0, -3.0);
			Point imagePlaneBottomRight = Point(1.0, -1.0, -3.0);

			// Textbook page 75
			ray->position = eye;

			float rectangleWidth = imagePlaneTopRight.x - imagePlaneTopLeft.x;
			float rectangleHeight = imagePlaneTopRight.y - imagePlaneBottomRight.y;

			float horizontalSampleDistance = rectangleWidth / viewingPlaneWidth;
			float verticalSampleDistance = rectangleHeight / viewingPlaneHeight;

			float imagePlaneX = imagePlaneTopLeft.x + ((rectangleWidth * (sample.x + 0.5)) / viewingPlaneWidth);
			float imagePlaneY = imagePlaneBottomLeft.y + ((rectangleHeight * (sample.y + 0.5)) / viewingPlaneHeight);

			ray->direction = Vector3(imagePlaneX, imagePlaneY, imagePlaneTopLeft.z);

			ray->t_min = FLT_EPSILON;
			ray->t_max = FLT_MAX;
		}
};

#endif /* CAMERA_H_ */
