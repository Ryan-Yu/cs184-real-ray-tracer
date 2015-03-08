#ifndef BRDFCOEFFICIENTS_H_
#define BRDFCOEFFICIENTS_H_

// This is a wrapper class for the 4 BRDF coefficients.
// NOTE: a BRDFCoefficient is an instance variable of a Material
class BRDFCoefficients {
	public:
		Color kd, ks, ka, kr;

	BRDFCoefficients() {

	}

	BRDFCoefficients(Color kd, Color ks, Color ka, Color kr) {
		this->kd = kd;
		this->ks = ks;
		this->ka = ka;
		this->kr = kr;
	}
};

#endif /* BRDFCOEFFICIENTS_H_ */
