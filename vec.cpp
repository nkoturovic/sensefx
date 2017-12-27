#include <cmath>
#include "vec.h"
#include "ast.h"

vec::vec(int dim) {
    coordinates.resize(dim);
}

int vec::dim()const {
        return (int) coordinates.size();
}

float vec::normSquared() const {
    float sum = 0;
    int dim = this->dim();

    for (int i=0; i<dim; i++)
        sum += coordinates[i]*coordinates[i];

    return sum;
}

float vec::norm() const{
   return std::sqrt(this->normSquared());
}

float vec::operator*(vec &v)const{
    int dim = this->dim();
    ast(dim == v.dim(), "Vector dimensions must be same");
    float sum = 0;

    for (int i=0; i<dim; i++)
        sum+=coordinates[i]*v.coordinates[i];

    return sum;
}

float vec::angle(vec v) {
	float scalarProduct = (*this)*v;
	return scalarProduct/(this->norm()*v.norm());
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

vec2::vec2() : vec(2),
x(coordinates[0]), y(coordinates[1]) {
    this->x = 0;
    this->y = 0;
}

vec2::vec2(float x, float y) : vec(2),
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

vec2 vec2::operator*(float scalar) {
    return vec2(scalar*x,scalar*y);
}

float vec2::operator*(vec2 &v) {
    return vec::operator*(v);
}

float vec2::operatorX(vec2 &v) {
    return x*v.y - v.x*y;
}

vec2 vec2::normalize() {
    float norm = this->norm();
    return vec2(x/norm, y/norm);
}

vec3::vec3() : vec(3),
x(coordinates[0]), y(coordinates[1]), z(coordinates[2]) {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

vec3::vec3(float x, float y, float z) : vec(3),
x(coordinates[0]), y(coordinates[1]), z(coordinates[2]) {
    this->x = x;
    this->y = y;
    this->z = z;
}

vec3::vec3(float *v) : vec3(v[0], v[1], v[2]) {}

vec3 vec3::operator+(const vec3 &v) const{
    return vec3(x+v.x,y+v.y,z+v.z);
}

vec3 vec3::operator+=(const vec3 &v) const{
	vec3 sum = *this + v;
	this->x = sum.x;
	this->y = sum.y;
	this->z = sum.z;
	return *this;
}

vec3 vec3::operator-(vec3 &v) const{
    return vec3(x-v.x,y-v.y,z-v.z);
}

vec3 vec3::operator*(float scalar)const {
    return vec3(scalar*x,scalar*y,scalar*z);
}

float vec3::operator*(vec3 &v)const {
    return vec::operator*(v);
}

vec3 vec3::operatorX(vec3 &v) {
    return vec3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

float vec3::operatorTriple(vec3 &v, vec3 &u) {
    return this->operatorX(v).operator*(u);
}

vec3 vec3::normalize() const {
    float norm = this->norm();
    return vec3(x/norm, y/norm, z/norm);
}

void vec3::normalizeSelf() const {
	vec3 normV = this->normalize();
	this->x = normV.x;
	this->y = normV.y;
	this->z = normV.z;
}
