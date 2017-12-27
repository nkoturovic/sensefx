#include "mat.h"
#include "ast.h"
#include <iostream>
#include <algorithm>

mat mat::operator*(mat rop) {
	
	ast(m == rop.n, "Invalid dimension");

	float sum=0;
	mat result(n, rop.m);

	int i,j,k;

	for (i = 0; i < n; i++) {
		for (j = 0; j < rop.m; j++) {
			for (k = 0; k < m; k++) {
				sum = sum + this->matrix[i][k]*rop[k][j];
			}

			result[i][j] = sum;
			sum = 0;
		}
}

return result;
}

mat mat::operator=(mat rop) {

	ast(n == rop.n, "Invalid matrix size");
	ast(m == rop.m, "Invalid matrix size");

	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++) 
			this->matrix[i][j] = rop[i][j];
	}

	return *this;
}

mat mat::operator*=(mat rop) {
	ast(m == rop.n, "Invalid dimension");
	ast(n == rop.m, "Invalid matrix size");
	mat result = *this * rop;

	for (int i=0; i<n; i++) {
		for (int j=0; j<rop.m; j++)
			this->matrix[i][j] = result[i][j];
	}

	return *this;
}

std::ostream &operator<<(std::ostream &os, mat &mat1) {

	for (int i=0; i<mat1.n; i++) {
		for (int j=0; j<mat1.m; j++)
			os << mat1[i][j] << " ";
		os << std::endl;
	}

    return os;
}


float*& mat::operator[] (int x) {
	return matrix[x];
}

mat::mat(int n, int m, float ** matr) : n(n), m(m) {
	
	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++)  {
			elementsVec.push_back(matr[i][j]);
		}
		std::cout << "uspesno" << std::endl;
		std::cout.flush();

	}

		elements = elementsVec.data();
	matrix = new float*[n];

	for (int i=0; i<n; i++) 
		matrix[i] = &elements[i*m];
}

mat::mat(int n, int m, float elOnDiagonal) : n(n), m(m) {

	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++) {
			if (i != j)
				elementsVec.push_back(0.0f);
			else 
				elementsVec.push_back(elOnDiagonal);
		}
	}

	elements = elementsVec.data();
	matrix = new float*[n];

	for (int i=0; i<n; i++) 
		matrix[i] = &elements[i*m];
}


mat::mat(int n, int m) : mat(n, m, 1.0f) {
};

mat mat::transpose() {

	mat retval(m,n);

	for (int j=0; j<m; j++) {
		for (int i=0; i<n; i++) {
			retval[j][i] = this->matrix[i][j];
		}
	}
	return retval;
};


mat4::mat4() : mat(4,4) {}

mat4::mat4(float matr[][4]) : mat(4,4) {
	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++)
			this->matrix[i][j] = matr[i][j];
	}
}

mat4 mat4::transpose() {

	mat4 retval;

	for (int j=0; j<m; j++) {
		for (int i=0; i<n; i++) {
			retval[j][i] = this->matrix[i][j];
		}
	}
	return retval;
}

mat4 mat4::operator=(mat rop) {
	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++)
			this->matrix[i][j] = rop.elements[i*m+j];
	}

	return *this;
}

mat4 mat4::inverse() {
	mat4 retval;
	mat4::inverse(this->elements, retval.elements);
	return retval;
}

/* Copied from MESA Implementation!! */
bool mat4::inverse(const float m[16], float invOut[16])
{
    float inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return true;
}

mat4 mat4::operator*(mat4 m) {
	mat l = *this;
	mat r = m;
	mat product = l*r;
	mat4 product4;
	product4 = product;

	return product4;
}

mat4 mat4::operator*=(mat4 m) {
	mat4 result = *this*m;
	*this = result;
	return *this;
}
