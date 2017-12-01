#ifndef SENSEFX_VEC_H
#define SENSEFX_VEC_H

#include <vector>
#include <ostream>

class vec {

    protected:
        std::vector<double> coordinates;

    public:

        vec(int dim);

        int dim()const;

    friend std::ostream &operator<<(std::ostream &os, const vec &vec1);
};

class vec2 : public vec {

    public:
        double &x, &y;

        vec2(double x, double y);
};

class vec3 : public vec {

    public:
        double &x, &y, &z;

        vec3(double x, double y, double z);
};


#endif //SENSEFX_VEC_H
