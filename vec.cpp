#include "vec.h"

vec::vec(int dim) {
    coordinates.resize(dim);
}

int vec::dim() const {
        return coordinates.size();
}

std::ostream &operator<<(std::ostream &os, const vec &vector) {

    int dim = vector.dim();

    os << "[";
    for (int i=0; i<dim; i++) {

        if (i > 0)
            os << " ";

        os << vector.coordinates[i];
    }
    os << "]";
    return os;
}

vec2::vec2(double x, double y) : vec(2),
x(coordinates[0]), y(coordinates[1]) {
    this->x = x;
    this->y = y;
}

vec3::vec3(double x, double y, double z) : vec(3),
x(coordinates[0]), y(coordinates[1]), z(coordinates[2]) {
    this->x = x;
    this->y = y;
    this->z = z;
}
