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
  Matrix4 createTranslationMatrix(float x, float y, float z) {
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
  Matrix4 createScalingMatrix(float x, float y, float z) {
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
};

#endif /* MATRIX_H_ */
