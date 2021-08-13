#include<iostream>
#include "raytracer.h"
#include "camera.h"
#include "hittable_list.h"
#include "color.h"
#include "sphere.h"
#include "material.h"

using namespace std;

// double hit_sphere(const point3& center, double radius, const ray& paul)
// {
//     // t2b⋅b+2tb⋅(A−C)+(A−C)⋅(A−C)−r2=0
//     vec3 oc = paul.ray_origin() - center; //normal of the point of intercept
//     auto a = paul.ray_direction().sq();
//     auto half_b = dot(oc, paul.ray_direction());
//     auto c = oc.sq() - radius * radius;
//     double discriminant = half_b * half_b - a * c;
    
//     if (discriminant > 0)
//     {
//         return (-half_b - sqrt(discriminant)) / a;
//     }
//     else
//     {
//         return -1.0;
//     }
//     // b^2 - 4ac
//     // if roots = 0, paul doesnt touch the spear
//     // if roots = 1, paul touchs the spear once
//     // if roots = 2, paul touchs the spear twice
// }

color ray_color(const ray& tracing_ray, const hittable& world, int depth)
{
    hit_record rec;

    if(depth <= 0)
    {
        return color(0, 0, 0);
    }
    
    if (world.hit(tracing_ray, 0.001, infinity, rec))
    {
        ray scattered;
        color attenuation;
        if(rec.mat_ptr->scatter(tracing_ray, rec, attenuation, scattered))
        {
            return attenuation * ray_color(scattered, world, depth - 1); 
        }
        return color(0, 0, 0);
    }
    // if doesnt hit, its background
    vec3 unit_direction = unit_vector(tracing_ray.ray_direction()); // finding the unit vector of the direction of the ray
    auto t = 0.5 * (unit_direction.y() +  1.0); // scaling t to 0 <= t <= 1
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
    // blendedValue=(1−t)⋅startValue+t⋅endValue
}

hittable_list randomscene()
{ 
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8)
                {   
                    // diffuse 
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    //metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);

                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else 
                {
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main()
{

    // image window properities 
    const double ASPECT_RATIO = 3.0 / 2.0;
    const int SCREEN_WIDTH = 400;
    const int SCREEN_HEIGHT = static_cast<int>(SCREEN_WIDTH / ASPECT_RATIO); // scaling the height according to the aspect ratio
    const int SAMPLES_PER_IMAGE = 100;
    const int MAX_DEPTH = 50;
    // creating an image of ppm format
    // FIRST LINE must be the image format, p3 indicates the color is in ascii
    // Second Line has to contain the image's width followed by its height, i.e. no of pixels in one column one row
    // Third Line contains the max value for r, g, b

    point3 lookfrom(3,2,13);
    point3 lookat(0,0,-1);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, ASPECT_RATIO, aperture, dist_to_focus);
    
    // Camera cam(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), 90, ASPECT_RATIO);

    // world
    // auto world = randomscene();
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.6, 0.3, 0.3));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.3, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center ));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5 , material_left ));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    cout << "P3\n" << SCREEN_WIDTH << ' ' << SCREEN_HEIGHT << "\n255\n"; // printing the image properities

    // creating the color value for each pixel in the given image window

    // rendering the image
    for(int j = SCREEN_HEIGHT-1; j >= 0; --j) // we are starting from the top of the window
    {
        cerr << "ScanLines remaining: " << j << "\n" << flush;  // flush forcefully writes the temp saved data into permanent data, we ar flushing j so that if the program ever gets stuck we'll know in which line its stuck at.

        for(int i = 0; i < SCREEN_WIDTH; ++i) // we are starting the the left of the window
        {
            color pixel_color(0, 0, 0);

            // // auto: automatically detects the result's data type and creates an object of that type
            // auto r = double(i) / (SCREEN_WIDTH-1); // getting a value between 0.0 and 1.0, this means that the starting of the screen will have the red color completely turned off and vice versa
            // auto g = double(j) / (SCREEN_HEIGHT-1); // getting a value between 0.0 and 1.0, this means that the top of the screen will have the green color completely on and vice-versa
            // auto b = 0.25; 

            // int ir = static_cast<int>(255.999 * r); // scaling the red value to an integer value between 0 and 255
            // int ig = static_cast<int>(255.999 * g); // scaling the green value to an integer value between 0 and 255
            // int ib = static_cast<int>(255.999 * b); // scaling the blue value to an integer value between 0 and 255
            // cout << ir << ' ' << ig << ' ' << ib << "\n" ;

            // averaging the run aka antialiasing
            for(int s = 0; s < SAMPLES_PER_IMAGE; ++s)
            {   
                auto u = (i + random_double()) / (SCREEN_WIDTH - 1); 
                auto v = (j + random_double()) / (SCREEN_HEIGHT - 1); // x,y cordinate of a ray in the image
                ray r = cam.get_ray(u,v); // creating a ray passing from the origin to the center of the pixel
                pixel_color += ray_color(r, world, MAX_DEPTH); // creating a color vector with the r,g,b compnents
            }
            
            write_color(cout, pixel_color, SAMPLES_PER_IMAGE); // print the values of the components

        }
    }

    cerr << "\nDone\n";
}