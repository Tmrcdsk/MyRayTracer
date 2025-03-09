#include "Global.h"

#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"

#include "Camera.h"

int main()
{
	HittableList world;

	float R = std::cosf(PI / 4);

	auto materialLeft = std::make_shared<Lambertian>(color(0.0f, 0.0f, 1.0f));
	auto materialRight = std::make_shared<Lambertian>(color(1.0f, 0.0f, 0.0f));

	world.add(std::make_shared<Sphere>(vec3(-R, 0.0f, -1.0f), R, materialLeft));
	world.add(std::make_shared<Sphere>(vec3(R, 0.0f, -1.0f), R, materialRight));

	Camera camera;
	camera.Width = 400;
	camera.Height = 225;
	camera.SPP = 100;
	camera.maxDepth = 60;

	camera.vfov = 90.0f;

	camera.render(world);
	return 0;
}