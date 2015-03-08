#ifndef BRDFCOEFFICIENTS_H_
#define BRDFCOEFFICIENTS_H_

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
