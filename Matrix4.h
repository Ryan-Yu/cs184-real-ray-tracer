#ifndef MATRIX_H_
#define MATRIX_H_

// Class that represents homogenized 3D matrices
class Matrix4 {
  public:
    float matrix[4][4];

  Matrix4() {
  }

  // TODO: Rotation matrix
  // TODO: SVD?

  // Creates a homogenized 3D translation matrix that translates by (x, y, z)
  static Matrix4 createTranslationMatrix(float x, float y, float z) {
	  Matrix4 matrixToReturn = Matrix4();
	  int i, j;
	  for (i = 0; i < 4; i++) {
		  for (j = 0; j < 4; j++) {
			  if (i == j) {
				  matrixToReturn.matrix[i][j] = 1.0;
			  } else {
				  matrixToReturn.matrix[i][j] = 0;
			  }
		  }
	  }
	  matrixToReturn.matrix[0][3] = x;
	  matrixToReturn.matrix[1][3] = y;
	  matrixToReturn.matrix[2][3] = z;
	  return matrixToReturn;
  }

  // Creates a homogenized 3D scaling matrix that scales by (x, y, z)
  static Matrix4 createScalingMatrix(float x, float y, float z) {
	  Matrix4 matrixToReturn = Matrix4();
	  int i, j;
	  for (i = 0; i < 4; i++) {
		  for (j = 0; j < 4; j++) {
			  matrixToReturn.matrix[i][j] = 0;
		  }
	  }
	  matrixToReturn.matrix[0][0] = x;
	  matrixToReturn.matrix[1][1] = y;
	  matrixToReturn.matrix[2][2] = z;
	  matrixToReturn.matrix[3][3] = 1.0;
	  return matrixToReturn;
  }

  static Matrix4 multiplyMatrices(Matrix4 matrix1, Matrix4 matrix2) {

  }

  static Vector3 multiplyMatrixByVector(Matrix4 matrix, Vector3 vector) {
	  Vector3 vectorToReturn = Vector3();
	  int i;
	  for (i = 0; i < 4; i++) {
		  float currentSum = 0.0;
		  currentSum += matrix.matrix[i][0] * vector.x;
		  currentSum += matrix.matrix[i][1] * vector.y;
		  currentSum += matrix.matrix[i][2] * vector.z;
		  currentSum += matrix.matrix[i][3] * 1.0;
		  if (i == 0) {
			  vectorToReturn.x = currentSum;
		  }
		  else if (i == 1) {
			  vectorToReturn.y = currentSum;
		  }
		  else if (i == 2) {
			  vectorToReturn.z = currentSum;
		  }
	  }
	  return vectorToReturn;
  }

  static Point multiplyMatrixByPoint(Matrix4 matrix, Point point) {
	  Point pointToReturn = Point();
	  int i;
	  for (i = 0; i < 4; i++) {
		  float currentSum = 0.0;
		  currentSum += matrix.matrix[i][0] * point.x;
		  currentSum += matrix.matrix[i][1] * point.y;
		  currentSum += matrix.matrix[i][2] * point.z;
		  currentSum += matrix.matrix[i][3] * 1.0;
		  if (i == 0) {
			  pointToReturn.x = currentSum;
		  }
		  else if (i == 1) {
			  pointToReturn.y = currentSum;
		  }
		  else if (i == 2) {
			  pointToReturn.z = currentSum;
		  }
	  }
	  return pointToReturn;
  }
};

#endif /* MATRIX_H_ */
