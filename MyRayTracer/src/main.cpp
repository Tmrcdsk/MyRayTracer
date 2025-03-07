#include "Global.h"

#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"

#include "Camera.h"

int main()
{
	HittableList world;
	world.add(std::make_shared<Sphere>(vec3(0.0f, 0.0f, -1.0f), 0.5f));
	world.add(std::make_shared<Sphere>(vec3(0.0f, -100.5f, -1.0f), 100.0f));

	Camera camera;
	camera.Width = 400;
	camera.Height = 225;
	camera.SPP = 100;

	camera.render(world);
	return 0;
}