#ifndef VEC_H 
#define VEC_H 
// if the token doesnt exist, it defines the token
// the token represents the below code
// defining the token if it doesnt exist

#include<cmath>
#include<iostream>
#include<cstdlib>
#include "raytracer.h"

using namespace std;

class vec3{

    // class members
    public:
        double e[3];

    // class methods
    public:
        // defining constructors

        // zero arg constructor
        vec3() : e{0,0,0} {} // creates a member called e containing the magnitudes 0 for i, j, k

        // full vector constructor
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {} // creates a member called e which contains the magnutudes of i, j, k.

        // functions to return mangitude of a component of the vector
        double x() const { return e[0]; } // method to return the i component( x-axis magnitude )
        double y() const { return e[1]; } // method to return the j component ( y-axis magnitude )
        double z() const { return e[2]; } // method to return the k component ( z-axis magnitude )

        // vector specific operations
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); } // overloading the - operator to get the inverse of the vector
        double operator[](int i) const { return e[i]; } // allows for subscribing, reading value at the specified index
        double& operator[](int i) { return e[i]; } // allows for subscribing, writing value at the specified index

        // operator overloading to allow for easier vector operations
        vec3& operator+=(const vec3 &v)
        {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this; // returning a pointer to itself
        }

        vec3& operator*=(const double d)
        {
            e[0] *= d;
            e[1] *= d;
            e[2] *= d;
            return *this; // returning a pointer to itself
        }

        vec3& operator/=(const double d)
        {
            return *this *= 1/d; // returning a pointer to itself
        }

        double sq() const
        {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        double length() const
        {
            return sqrt(sq()); // len of a vector = sqrt(|i|^2+|j|^2 + |k|^2)
        }

        inline static vec3 random() // creating a random vec3 
        {
            auto x = rand() / (RAND_MAX + 1.0);
            auto y = rand() / (RAND_MAX + 1.0);
            auto z = rand() / (RAND_MAX + 1.0);
            return vec3(x, y, z);
        }

        inline static vec3 random(double min, double max) // creating a random vec3 within specific bounds
        {
            auto x = rand() / (RAND_MAX + 1.0);
            auto y = rand() / (RAND_MAX + 1.0);
            auto z = rand() / (RAND_MAX + 1.0);
            x = min + (max - min) * x;
            y = min + (max - min) * y;
            z = min + (max - min) * z;
            return vec3(x, y, z);
        }

        bool near_zero() const // to find if any component of the vector is close to zero
        {
            const auto s = 1e-8;
            return (fabs(e[0] < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s)); 
        }
   
};

// type aliases for vec3 class
using point3 = vec3; // 3D point
using color = vec3; // rgb color

// utility functions/helper functions
// inline: reduces the call time for the function, improving the time taken by the program
// ostream directly outputs the result on the screen

// directly prints the vector on the screen
inline ostream& operator<<(ostream &out, const vec3 &v) // directly prints the values of the components 
    {
        return out << v.e[0] << " " << v.e[1] << " " << v.e[2];  
    }

// adding two vectors together
inline vec3 operator+(const vec3 &a, const vec3 &b)
{ 
    return vec3(a.e[0] + b.e[0], a.e[1] + b.e[1], a.e[2] + b.e[2]);
}

// subtracting two vectors
inline vec3 operator-(const vec3 &a, const vec3 &b)
{ 
    return vec3(a.e[0] - b.e[0], a.e[1] - b.e[1], a.e[2] - b.e[2]);
}

// vector multiplication (vec3 x vec3)
inline vec3 operator*(const vec3 &a, const vec3 &b)
{
    return vec3(a.e[0] * b.e[0], a.e[1] * b.e[1], a.e[2] * b.e[2]);
}

// vector multiplication (constval x vec3)
inline vec3 operator*(const vec3 &a, double b)
{
    return vec3(a.e[0] * b, a.e[1] * b, a.e[2] * b);
}

// to maintain associative property 
inline vec3 operator*(double b, const vec3 &a)
{
    return a * b;
}

//vector division (constval x vec3)
inline vec3 operator/(vec3 a, double b)
{
    return (1/b) * a;
}

// to maintain associative property 
inline vec3 operator/(double b, vec3 a)
{
    return (1/b) * a;
}

// dot product
inline double dot(const vec3 &u, const vec3 &v)
{
    return u.e[0] * v.e[0] 
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

// cross product
inline vec3 cross(const vec3 &u, const vec3 &v) 
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// making the vector a  unit vector
inline vec3 unit_vector(vec3 v)
{
    return v / v.length();// we do this by dividing the vector with its length, making the total mangnitude one
}

inline vec3 random_in_unit_sphere()
{
    while(true)
    {
        auto p = vec3::random(-1,1);
        if (p.sq() >= 1)
        {
            continue;
        }
        return p;
    }
}

vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
    auto cosx = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cosx * n);
    vec3 r_out_para = -sqrt(fabs(1.0 - r_out_perp.sq())) * n;
    return r_out_perp + r_out_para;

}

inline double random_d()
{   
    // returns a random real in [0, 1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_d(double min, double max)
{
    // returns a randon real in [min, max]
    return min + (max - min) * random_d();
}

vec3 random_in_unit_disk()
{
    while(true)
    {
        auto p = vec3(random_d(-1, 1), random_d(-1, 1), 0);
        
        if (p.sq() >= 1) 
        {
            continue;
        }
        return p;
    }
}
#endif // ending the original token checker