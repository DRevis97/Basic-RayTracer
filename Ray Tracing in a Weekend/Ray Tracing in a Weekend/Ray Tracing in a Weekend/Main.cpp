#include <iostream>
#include <cstdlib>
#include <cmath>
#include <random>

#include "Sphere.h"
#include "HitableList.h"
#include "Material.h"
#include "Camera.h"
#include "float.h"




vec3 color(const ray& r, Hitable* world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->Scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}

}

Hitable* randomScene()
{
	int n = 500;
	Hitable** list = new Hitable * [n + 1];
	list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = (rand() / (RAND_MAX + 1.0));
			vec3 center(a + 0.9 * (rand() / (RAND_MAX + 1.0)), 0.2, b + 0.9 * (rand() / (RAND_MAX + 1.0)));
			if ((center - vec3(4, 0, 2.0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
				{
					/*float L1 = ((rand() / (RAND_MAX + 1.0)) * (rand() / (RAND_MAX + 1.0)));
					float L2 = ((rand() / (RAND_MAX + 1.0)) * (rand() / (RAND_MAX + 1.0)));
					float L3 = ((rand() / (RAND_MAX + 1.0)) * (rand() / (RAND_MAX + 1.0)));
					vec3 lambert(L1, L2, L3);*/
					list[i++] = new Sphere(center, 0.2, new Lambertian(vec3(((rand() / (RAND_MAX + 1.0)) * (rand() / (RAND_MAX + 1.0))), ((rand() / (RAND_MAX + 1.0)) * (rand() / (RAND_MAX + 1.0))), ((rand() / (RAND_MAX + 1.0)) * (rand() / (RAND_MAX + 1.0))))));
				}
				else if (choose_mat < 0.95)
				{
					/*float M1 = 0.5 * (1 + (rand() / (RAND_MAX + 1.0)));
					float M2 = 0.5 * (1 + (rand() / (RAND_MAX + 1.0)));
					float M3 = 0.5 * (1 + (rand() / (RAND_MAX + 1.0)));*/
					list[i++] = new Sphere(center, 0.2, new Metal(vec3(0.5 * (1 + (rand() / (RAND_MAX + 1.0))), 0.5 * (1 + (rand() / (RAND_MAX + 1.0))), 0.5 * (1 + (rand() / (RAND_MAX + 1.0)))), 0.5 * (rand() / (RAND_MAX + 1.0))));
				}
				else
				{
					list[i++] = new Sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
		
	}

	list[i++] = new Sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new Sphere(vec3(4, 1, 0), 1.0, new Metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new HitableList(list, i);
	
}

int main()
{
	float pi = 3.14159265358979323846;
	int nx = 1200;
	int ny = 800;
	int ns = 10;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	Hitable* list[5];
	
	list[0] = new Sphere(vec3(0, 0, -1), 0.5, new Lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new Sphere(vec3(0, -100.5, -1), 100, new Lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new Sphere(vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new Sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new Sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
	Hitable* world = new HitableList(list, 5);
	world = randomScene();

	vec3 lookFrom(13, 2, 3);
	vec3 lookAt(0, 0, 0);
	float distance_to_focus = 10.0;
	float aperture = 0.1;
	Camera camera(lookFrom, lookAt, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, distance_to_focus);
	for (int i = ny - 1; i >= 0; i--)
	{
		for (int j = 0; j < nx; j++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(j + (rand() / (RAND_MAX + 1.0))) / float(nx);
				float v = float(i + (rand() / (RAND_MAX + 1.0))) / float(ny);
				ray r = camera.getRay(u, v);
				

				col += color(r, world,0);
			}
			
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}