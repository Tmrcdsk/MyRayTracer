#include "Global.h"

#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"

#include "Camera.h"

int main()
{
	HittableList world;

	auto materialGround = std::make_shared<Lambertian>(color(0.8f, 0.8f, 0.0f));
	auto materialCenter = std::make_shared<Lambertian>(color(0.1f, 0.2f, 0.5f));
	auto materialLeft = std::make_shared<Metal>(color(0.8f, 0.8f, 0.8f));
	auto materialRight = std::make_shared<Metal>(color(0.8f, 0.6f, 0.2f));

	world.add(std::make_shared<Sphere>(vec3(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
	world.add(std::make_shared<Sphere>(vec3(0.0f, 0.0f, -1.2f), 0.5f, materialCenter));
	world.add(std::make_shared<Sphere>(vec3(-1.0f, 0.0f, -1.0f), 0.5f, materialLeft));
	world.add(std::make_shared<Sphere>(vec3(1.0f, 0.0f, -1.0f), 0.5f, materialRight));

	Camera camera;
	camera.Width = 400;
	camera.Height = 225;
	camera.SPP = 100;
	camera.maxDepth = 50;

	camera.render(world);
	return 0;
}