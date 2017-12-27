#ifndef SENSEFX_MAT_H
#define SENSEFX_MAT_H

#include <ostream>
#include "vec.h"
#include <vector>

class mat {

	protected:
		std::vector <float> elementsVec;

	public:
		float * elements;
		float ** matrix;

		int n, m;

		friend std::ostream &operator<<(std::ostream &os, mat &mat1);

		mat(int n, int m);
		mat(int n, int m, float ** matr);
		mat(int n, int m, float elOnDiagonal);

		mat operator=(mat m);
		mat operator*(mat m);
		mat operator*=(mat m);
		float*& operator[] (int x);

		void makeIdentity();
		mat transpose();
};

class mat4 : public mat {
	public:
		static bool inverse(const float m[16], float invOut[16]);

		mat4();
		mat4(float matr[][4]);
		mat4 transpose();
		mat4 inverse();
		mat4 operator=(const mat rop);
		mat4 operator*(mat4 m);
		mat4 operator*=(mat4 m);
};

#endif //SENSEFX_MAT_H
