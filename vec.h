#ifndef SENSEFX_VEC_H
#define SENSEFX_VEC_H

#include <vector>
#include <ostream>

class vec {

    protected:
        std::vector<float> coordinates;

    public:
        vec(int dim);

        int dim();

        float normSquared();
        float norm();

        /* Skalarni proizvod
         * smatra da su "bazni"
         * vektori ortonormirani
         * kao i ostali metodi. */
        float operator*(vec &v);

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
        float &x, &y;

        vec2();
        vec2(float x, float y);

        vec2 operator+(vec2 &v);
        vec2 operator-(vec2 &v);
        vec2 operator*(float scalar);
        float operator*(vec2 &v);
        float operatorX(vec2 &v);

        vec2 normalize();
};

class vec3 : public vec {

    public:
        float &x, &y, &z;

        vec3();
        vec3(float x, float y, float z);

        vec3 operator+(vec3 &v);
        vec3 operator-(vec3 &v);
        vec3 operator*(float scalar);
        float operator*(vec3 &v);
        vec3 operatorX(vec3 &v);
        float operatorTriple(vec3 &v, vec3 &u);

        vec3 normalize();
};

#endif //SENSEFX_VEC_H
