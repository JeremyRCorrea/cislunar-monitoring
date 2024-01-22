#pragma once

#include <boost/array.hpp>

typedef boost::array<double, 3> double3;
typedef boost::array<double, 6> State;


double dot(double3 v, double3 w) {
    return v[0] * w[0] + v[1] * w[1] + v[2] * w[2];
}

double magnitude(const double3& r) {
    return sqrt(dot(r, r));
}

double distance(const double3& v, const double3& w) {
    return sqrt( std::pow(v[0] - w[0], 2) + std::pow(v[1] - w[1], 2) + std::pow(v[2] - w[2], 2));
}

double3 cross(const double3& a, const double3& b)
{
    return double3{ a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0] };
}

// Addition
double3 operator+(const double3& v, const double3& w) { return double3{ v[0] + w[0], v[1] + w[1], v[2] + w[2] }; }
double3 operator+(const double3& v, double scalar) { return double3{ v[0] + scalar, v[1] + scalar, v[2] + scalar }; }
double3 operator+(double scalar, const double3& v) { return double3{ v[0] + scalar, v[1] + scalar, v[2] + scalar }; }

// Subtraction
double3 operator-(const double3& v, const double3& w) { return double3{ v[0] - w[0], v[1] - w[1], v[2] - w[2] }; }
double3 operator-(const double3& v, double scalar) { return double3{ v[0] - scalar, v[1] - scalar, v[2] - scalar }; }
double3 operator-(double scalar, const double3& v) { return double3{ scalar - v[0], scalar - v[1], scalar - v[2] }; }

// Scalar multiply
double3 operator*(double scalar, const double3& v) { return double3{ v[0] * scalar, v[1] * scalar, v[2] * scalar }; }
double3 operator*(const double3& v, double scalar) { return double3{ v[0] * scalar, v[1] * scalar, v[2] * scalar }; }

// Scalar divide
double3 operator/(const double3& v, double scalar) { return double3{ v[0] / scalar, v[1] / scalar, v[2] / scalar }; }
double3 operator/(double scalar, const double3& v) { return double3{ scalar / v[0], scalar / v[1], scalar / v[2] }; }



