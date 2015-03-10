#ifndef CAMERA_H_
#define CAMERA_H_

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
			ray->position = Point(0, 0, 0);

			float horizontalSampleDistance = 2.0 / viewingPlaneWidth;
			float verticalSampleDistance = 2.0 / viewingPlaneHeight;

			float imagePlaneX = -1.0 + ((2 * (sample.x + 0.5)) / viewingPlaneWidth);
			float imagePlaneY = -1.0 + ((2 * (sample.y + 0.5)) / viewingPlaneHeight);

			ray->direction = Vector3(imagePlaneX, imagePlaneY, -1);

			ray->t_min = LDBL_EPSILON;
			ray->t_max = FLT_MAX;
		}
};

#endif /* CAMERA_H_ */
