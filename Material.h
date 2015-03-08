#ifndef MATERIAL_H_
#define MATERIAL_H_

class Material {
	public:
		BRDFCoefficients constantBRDF;

	Material() {

	}

	BRDFCoefficients getBRDF(DifferentialGeometry& differentialGeometry, BRDFCoefficients* brdf) {
		return constantBRDF;
	}
};



#endif /* MATERIAL_H_ */
