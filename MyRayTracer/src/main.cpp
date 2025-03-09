#include "Global.h"

#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"

#include "Camera.h"

#include <chrono>

int main()
{
	HittableList world;

	auto materialGround = std::make_shared<Lambertian>(color(0.5f, 0.5f, 0.5f));
	world.add(std::make_shared<Sphere>(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, materialGround));

	for (int a = -11; a < 11; ++a) {
		for (int b = -11; b < 11; ++b) {
			auto chooseMat = randomFloat();
			vec3 center(a + 0.9f * randomFloat(), 0.2f, b + 0.9 * randomFloat());

			if ((center - vec3(4.0f, 0.2f, 0.0f)).norm() > 0.9f) {
				std::shared_ptr<Material> sphereMaterial;

				if (chooseMat < 0.8f) {
					// diffuse
					vec3 albedo = color::Random() * color::Random();
					sphereMaterial = std::make_shared<Lambertian>(albedo);
					world.add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
				else if (chooseMat < 0.95f) {
					// metal
					vec3 albedo = color::Random(0.5f, 1.0f);
					float fuzz = randomFloat(0.0f, 0.5f);
					sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
					world.add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
				else {
					// glass
					sphereMaterial = std::make_shared<Dielectric>(1.5f);
					world.add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(1.5f);
	world.add(std::make_shared<Sphere>(vec3(0.0f, 1.0f, 0.0f), 1.0f, material1));

	auto material2 = std::make_shared<Lambertian>(color(0.4f, 0.2f, 0.1f));
	world.add(std::make_shared<Sphere>(vec3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

	auto material3 = std::make_shared<Metal>(color(0.7f, 0.6f, 0.5f), 0.0f);
	world.add(std::make_shared<Sphere>(vec3(4.0f, 1.0f, 0.0f), 1.0f, material3));

	Camera camera;
	camera.Width = 1280;
	camera.Height = 720;
	camera.SPP = 500;
	camera.maxDepth = 50;

	camera.vfov = 20.0f;
	camera.lookFrom = vec3(13.0f, 2.0f, 3.0f);
	camera.lookAt = vec3(0.0f, 0.0f, 0.0f);
	camera.vUp = vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.6f;
	camera.focusDist = 10.0f;

	auto start = std::chrono::system_clock::now();
	camera.render(world);
	auto stop = std::chrono::system_clock::now();

	std::cout << "Render complete: \n";
	std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
	std::cout << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " minutes\n";
	std::cout << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds\n";

	return 0;
}