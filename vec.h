#ifndef SENSEFX_VEC_H
#define SENSEFX_VEC_H

#include <vector>
#include <ostream>

class vec {

    protected:
        std::vector<double> coordinates;

    public:
        vec(int dim);

        int dim();

        static double norm(vec *v);
        double norm();

        /* Skalarni proizvod
         * smatra da su "bazni"
         * vektori ortonormirani
         * kao i ostali metodi. */
        double operator*(vec &v);

        bool operator==(vec &v);

        bool operator!=(vec &v);

        friend std::ostream &operator<<(std::ostream &os, vec &v);

        bool operator<(vec &v);

        bool operator>(vec &v);

        bool operator<=(vec &v);

        bool operator>=(vec &v);
};

class vec2 : public vec {

    public:
        double &x, &y;

        vec2(double x, double y);
        vec2 operator+(vec2 &v);
        vec2 operator-(vec2 &v);
        double operatorX(vec2 &v);
};

class vec3 : public vec {

    public:

        double &x, &y, &z;

        vec3();
        vec3(double x, double y, double z);

        vec3 operator+(vec3 &v);
        vec3 operator-(vec3 &v);
        vec3 operatorX(vec3 &v);

};

#endif //SENSEFX_VEC_H
