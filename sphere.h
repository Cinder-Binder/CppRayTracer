#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public hittable
{
    // class methods
    public:
        Sphere() {}
        Sphere(point3 cen_pos, double rad, shared_ptr<material> mat) : center(cen_pos), radius(rad), mat_ptr(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    // a sphere has two properties, the position of the sphere and the radius
    // class members
    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
};

bool Sphere::hit(const ray& paul, double t_min, double t_max, hit_record& rec) const
{
    vec3 oc = paul.ray_origin() - center; 
    auto a = paul.ray_direction().sq();
    auto half_b = dot(oc, paul.ray_direction());
    auto c = oc.sq() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return false; // the paul doesnt touch the sphere
    }
    auto sqrtd = sqrt(discriminant);

    // finding the nearest root that lies in the given range
    auto root = (-half_b - sqrtd) / a; // finding the root
    if(root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a; // finding the root 
        if(root < t_min || t_max < root)
        {
            return false; // if the root lies outside the range
        }
    }

    rec.t = root; // recording the hit
    rec.p = paul.at(rec.t); // the point of interception
    rec.normal = (rec.p - center) / radius; // normal at the point of interception
    vec3 outward_normal = (rec.p - center) / radius; // normal
    rec.set_face_normal(paul, outward_normal); // setting the outward normal
    rec.mat_ptr = mat_ptr;
    return true; // the paul is touching the sphere
}


#endif