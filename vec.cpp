#include <cmath>
#include "vec.h"
#include "ast.h"

vec::vec(int dim) {
    coordinates.resize(dim);
}

int vec::dim() {
        return (int) coordinates.size();
}

double vec::norm(vec *v) {

    double sum = 0;
    int dim = v->dim();

    for (int i=0; i<dim; i++)
        sum += v->coordinates[i]*v->coordinates[i];

    return std::sqrt(sum);
}

double vec::norm() {
   return vec::norm(this);
}

double vec::operator*(vec &v) {
    int dim = this->dim();
    ast(dim == v.dim(), "Vector dimensions must be same");
    double sum = 0;

    for (int i=0; i<dim; i++)
        sum+=coordinates[i]*v.coordinates[i];

    return sum;
}

std::ostream &operator<<(std::ostream &os,vec &v) {

    int dim = v.dim();

    os << "[";
    for (int i=0; i<dim; i++) {

        if (i > 0)
            os << " ";

        os << v.coordinates[i];
    }
    os << "]";
    return os;
}

bool vec::operator==(vec &v) {

    if (this->dim() != v.dim())
        return false;

    for (int i=0; i < v.dim(); i++)
        if (coordinates[i] != v.coordinates[i])
            return false;
    return true;
}

bool vec::operator!=(vec &v) {
    return (!(operator==(v)));
}

bool vec::operator<(vec &v) {
    return norm() < v.norm();
}

bool vec::operator>(vec &v) {
    return norm() > v.norm();
}

bool vec::operator<=(vec &v) {
    return norm() <= v.norm();
}

bool vec::operator>=(vec &v) {
    return norm() >= v.norm();
}


vec2::vec2(double x, double y) : vec(2),
x(coordinates[0]), y(coordinates[1]) {
    this->x = x;
    this->y = y;
}

vec2 vec2::operator+(vec2 &v) {
    return vec2(x-v.x,y-v.y);
}

vec2 vec2::operator-(vec2 &v) {
    return vec2(x-v.x,y-v.y);
}

double vec2::operatorX(vec2 &v) {
    return x*v.y - v.x*y;
}

vec3::vec3() : vec(3),
x(coordinates[0]), y(coordinates[1]), z(coordinates[2]) {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

vec3::vec3(double x, double y, double z) : vec(3),
x(coordinates[0]), y(coordinates[1]), z(coordinates[2]) {
    this->x = x;
    this->y = y;
    this->z = z;
}

vec3 vec3::operator+(vec3 &v) {
    return vec3(x+v.x,y+v.y,z+v.z);
}

vec3 vec3::operator-(vec3 &v) {
    return vec3(x-v.x,y-v.y,z-v.z);
}

vec3 vec3::operatorX(vec3 &v) {
    return vec3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}
