/*
 * Material.h
 *
 *  Created on: Mar 7, 2015
 *      Author: ryanyu
 */

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
