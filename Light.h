#ifndef LIGHT_H_
#define LIGHT_H_

class Light {
	public:
		virtual void generateLightRay(DifferentialGeometry& differentialGeometry, Ray* lightRay, Color* lightColor) {

		}
};

class PointLight : public Light {
	public:
		float x, y, z, r, g, b;

	PointLight() {

	}

	PointLight(float x, float y, float z, float r, float g, float b) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->r = r;
		this->g = g;
		this->b = b;
	}

	void generateLightRay(DifferentialGeometry& differentialGeometry, Ray* lightRay, Color* lightColor) {
		lightRay->position = Point(differentialGeometry.position.x, differentialGeometry.position.y, differentialGeometry.position.z);
		lightRay->direction = Vector3(this->x - differentialGeometry.position.x, this->y - differentialGeometry.position.y, this->z - differentialGeometry.position.z);
		lightRay->t_min = LDBL_EPSILON;

		// Determine intersection point of our light ray and (this->x, this->y, this->z)
		lightRay->t_max = (this->x - lightRay->position.x)/(lightRay->direction.x);
		lightColor->r = this->r;
		lightColor->g = this->g;
		lightColor->b = this->b;
	}
};


class DirectionalLight : public Light {
	public:
		float x, y, z, r, g, b;

	DirectionalLight() {

	}

	DirectionalLight(float x, float y, float z, float r, float g, float b) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->r = r;
		this->g = g;
		this->b = b;
	}

	void generateLightRay(DifferentialGeometry& differentialGeometry, Ray* lightRay, Color* lightColor) {
		lightRay->position = Point(differentialGeometry.position.x, differentialGeometry.position.y, differentialGeometry.position.z);
		// Directional lights have constant direction.
		lightRay->direction = Vector3(this->x, this->y, this->z);
		lightRay->t_min = LDBL_EPSILON;
		lightRay->t_max = FLT_MAX;
		lightColor->r = this->r;
		lightColor->g = this->g;
		lightColor->b = this->b;
	}
};

#endif /* LIGHT_H_ */
