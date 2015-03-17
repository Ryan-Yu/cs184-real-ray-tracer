#ifndef MATRIX_H_
#define MATRIX_H_

// Class that represents homogenized 3D matrices
class Matrix4 {
  public:

  Matrix4() {
  }


  // TODO: Rotation matrix
  // TODO: SVD?


  // Creates an identity matrix
  static Eigen::Matrix4f createIdentityMatrix() {
	  Eigen::Matrix4f m;
	  m << 1, 0, 0, 0,
		   0, 1, 0, 0,
		   0, 0, 1, 0,
		   0, 0, 0, 1;
	  return m;
  }

  // Creates a homogenized 3D translation matrix that translates by (x, y, z)
  static Eigen::Matrix4f createTranslationMatrix(float x, float y, float z) {
	  Eigen::Matrix4f m;
	  m << 1, 0, 0, x,
		   0, 1, 0, y,
		   0, 0, 1, z,
		   0, 0, 0, 1;
	  return m;
  }

  // Creates a homogenized 3D scaling matrix that scales by (x, y, z)
  static Eigen::Matrix4f createScalingMatrix(float x, float y, float z) {
	  Eigen::Matrix4f m;
	  m << x, 0, 0, 0,
		   0, y, 0, 0,
		   0, 0, z, 0,
		   0, 0, 0, 1;
	  return m;
  }

  static Eigen::Matrix4f composeMatrices(Eigen::Matrix4f matrix1, Eigen::Matrix4f matrix2) {
	  return matrix1*matrix2;
  }
  // TODO: Might need to change homogenized coordinate from a 0 to a 1.
  static Vector3 multiplyMatrixByVector(Eigen::Matrix4f matrix, Vector3 vector) {
	  Eigen::Vector4f vector4(vector.x, vector.y, vector.z, 0.0);
	  Eigen::Vector4f resultVector4 = matrix*vector4;
	  Vector3 resultVector3 = Vector3(resultVector4(0, 0), resultVector4(1, 0), resultVector4(2, 0));

	  return resultVector3;
  }
  // TODO: Might need to change homogenized coordinate from a 1 to a 0.
  static Point multiplyMatrixByPoint(Eigen::Matrix4f matrix, Point point) {
	  Eigen::Vector4f vector4(point.x, point.y, point.z, 1.0);
	  Eigen::Vector4f resultVector4 = matrix*vector4;
	  Point resultPoint = Point(resultVector4[0], resultVector4[1], resultVector4[2]);
	  return resultPoint;
  }

	bool isIdentity(Eigen::Matrix4f matrix) {
		return matrix == Matrix4::createIdentityMatrix();
	}
};

#endif /* MATRIX_H_ */
