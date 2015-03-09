#ifndef CAMERA_H_
#define CAMERA_H_

class Camera {
	public:
		Camera() {

		}

		void generateRay(Sample& sample, Ray* ray) {
			ray->position = Point(0, 0, 0);
			ray->direction = Vector3(sample.x, sample.y, -1);
			ray->t_min = LDBL_EPSILON;
			ray->t_max = FLT_MAX;
		}
};

#endif /* CAMERA_H_ */
