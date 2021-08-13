#ifndef HITTABLE_LIST_H
#define HITTALBE_LIST_H

#include "hittable.h"

#include<memory>
#include<vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable
{   
    // class members
    public:
        std::vector<shared_ptr<hittable>> objects;
    // class methods
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
 
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max; // hits infinity at the start, and inches towards the closest obj in each call

    for(const auto& object : objects) // for each obj in objs
    {
        if(object->hit(r, t_min, closest_so_far, temp_rec)) // hittable class method "hit" returns 0
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }    

    return hit_anything;

}

#endif