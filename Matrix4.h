#ifndef MATRIX_H_
#define MATRIX_H_

class Matrix4 {
  public:
    float matrix[4][4];

  Matrix4() {
  }

  // TODO: fill this in
  // TODO: Rotation matrix
  // TODO: SVD?
  Matrix4 createTranslationMatrix(float x, float y, float z) {
	  int i, j;
	  for (i = 0; i < 4; i++) {
		  for (j = 0; j < 4; j++) {
			  if (i == j) {
				  matrix[i][j] = 1.0;
			  } else {
			  matrix[i][j] = 0;
			  }
		  }
	  }
	  matrix[0][3] = x;
	  matrix[1][3] = y;
	  matrix[2][3] = z;
	  return matrix;
  }

  Matrix4 createScalingMatrix(float x, float y, float z) {
	  Matrix4 toReturn = Matrix4();
	  int i, j;
	  for (i = 0; i < 4; i++) {
		  for (j = 0; j < 4; j++) {
			  matrix[i][j] = 0;
		  }
	  }
	  matrix[0][0] = x;
	  matrix[1][1] = y;
	  matrix[2][2] = z;
	  matrix[3][3] = 1.0;
	  toReturn.matrix = matrix;
	  return toReturn;
  }
};

#endif /* MATRIX_H_ */
