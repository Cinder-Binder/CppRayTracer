#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {

    // class members
    public:
        vec3 direction; // direction vector
        point3 origin; // vector at origin
    
    //class methods
    public:
        ray() {}
        ray(const point3& orig, const vec3& dir)
            : origin(orig), direction(dir) // initializing the ray with origin vector and the directional vector
        {}

        point3 ray_origin() const { return origin; } // method to return the origin vector
        vec3 ray_direction() const { return direction; } // method to return the direction vector

        point3 at(double t) const {
            return origin + t*direction; // the position on the ray
            // P(t) = A + tb
        }

};

#endif