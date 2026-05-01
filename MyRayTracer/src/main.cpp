#include "Global.h"

#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Quad.h"
#include "Material.h"
#include "BVH.h"
#include "Texture.h"

#include "Camera.h"
#include "ConstantMedium.h"

#include <chrono>

#define TIMER(render)                                                          \
do {                                                                           \
    auto _t0 = std::chrono::steady_clock::now();                               \
    render;                                                                    \
    auto _t1 = std::chrono::steady_clock::now();                               \
    auto _dt = _t1 - _t0;                                                      \
    std::cout << "Render complete:\n";                                         \
    std::cout << "Time taken: "                                                \
              << std::chrono::duration_cast<std::chrono::hours>(_dt).count()   \
              << " hours\n";                                                   \
    std::cout << "          : "                                                \
              << std::chrono::duration_cast<std::chrono::minutes>(_dt).count() \
              << " minutes\n";                                                 \
    std::cout << "          : "                                                \
              << std::chrono::duration_cast<std::chrono::seconds>(_dt).count() \
              << " seconds\n";                                                 \
} while (0)

void bouncingSpheres()
{
	HittableList world;

	auto checker = std::make_shared<CheckerTexture>(0.32f, color(0.2f, 0.3f, 0.1f), color(0.9f, 0.9f, 0.9f));
	world.add(std::make_shared<Sphere>(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<Lambertian>(checker)));

	for (int a = -11; a < 11; ++a) {
		for (int b = -11; b < 11; ++b) {
			auto chooseMat = randomFloat();
			vec3 center(a + 0.9f * randomFloat(), 0.2f, b + 0.9f * randomFloat());

			if ((center - vec3(4.0f, 0.2f, 0.0f)).norm() > 0.9f) {
				std::shared_ptr<Material> sphereMaterial;

				if (chooseMat < 0.8f) {
					// diffuse
					vec3 albedo = color::Random() * color::Random();
					sphereMaterial = std::make_shared<Lambertian>(albedo);
					vec3 center2 = center + vec3(0, randomFloat(0.0f, 0.5f), 0);
					world.add(std::make_shared<Sphere>(center, center2, 0.2f, sphereMaterial));
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

	world = HittableList(std::make_shared<BVHNode>(world));

	Camera camera;
	camera.Width = 400;
	camera.Height = 225;
	camera.SPP = 100;
	camera.maxDepth = 50;
	camera.background = color(0.70f, 0.80f, 1.00f);

	camera.vfov = 20.0f;
	camera.lookFrom = vec3(13.0f, 2.0f, 3.0f);
	camera.lookAt = vec3(0.0f, 0.0f, 0.0f);
	camera.vUp = vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.6f;
	camera.focusDist = 10.0f;

	TIMER(camera.render(world));
}

void checkeredSpheres()
{
	HittableList world;

	auto checker = std::make_shared<CheckerTexture>(0.32f, color(0.2f, 0.3f, 0.1f), color(0.9f, 0.9f, 0.9f));

	world.add(std::make_shared<Sphere>(vec3(0.0f, -10.0f, 0.0f), 10.0f, std::make_shared<Lambertian>(checker)));
	world.add(std::make_shared<Sphere>(vec3(0.0f,  10.0f, 0.0f), 10.0f, std::make_shared<Lambertian>(checker)));

	Camera camera;
	camera.Width = 400;
	camera.Height = 225;
	camera.SPP = 100;
	camera.maxDepth = 50;
	camera.background = color(0.70f, 0.80f, 1.00f);

	camera.vfov = 20.0f;
	camera.lookFrom = vec3(13.0f, 2.0f, 3.0f);
	camera.lookAt = vec3(0.0f, 0.0f, 0.0f);
	camera.vUp = vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.0f;

	TIMER(camera.render(world));
}

void earth() {
	auto earthTexture = std::make_shared<ImageTexture>("image/earthmap.jpg");
	auto earthSurface = std::make_shared<Lambertian>(earthTexture);
	auto globe = std::make_shared<Sphere>(vec3(0, 0, 0), 2.0f, earthSurface);

	Camera camera;
	camera.Width = 400;
	camera.Height = 225;
	camera.SPP = 100;
	camera.maxDepth = 50;
	camera.background = color(0.70f, 0.80f, 1.00f);

	camera.vfov = 20.0f;
	camera.lookFrom = vec3(0.0f, 0.0f, 12.0f);
	camera.lookAt = vec3(0.0f, 0.0f, 0.0f);
	camera.vUp = vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.0f;

	TIMER(camera.render(HittableList(globe)));
}

void perlinSpheres() {
	HittableList world;

	auto pertext = std::make_shared<NoiseTexture>(4);
	world.add(std::make_shared<Sphere>(vec3(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
	world.add(std::make_shared<Sphere>(vec3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

	Camera camera;
	camera.Width = 400;
	camera.Height = 225;
	camera.SPP = 100;
	camera.maxDepth = 50;
	camera.background = color(0.70f, 0.80f, 1.00f);

	camera.vfov = 20.0f;
	camera.lookFrom = vec3(13.0f, 2.0f, 3.0f);
	camera.lookAt = vec3(0.0f, 0.0f, 0.0f);
	camera.vUp = vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.0f;

	TIMER(camera.render(HittableList(world)));
}

void quads() {
	HittableList world;

	// Materials
	auto left_red     = std::make_shared<Lambertian>(color(1.0f, 0.2f, 0.2f));
    auto back_green   = std::make_shared<Lambertian>(color(0.2f, 1.0f, 0.2f));
    auto right_blue   = std::make_shared<Lambertian>(color(0.2f, 0.2f, 1.0f));
    auto upper_orange = std::make_shared<Lambertian>(color(1.0f, 0.5f, 0.0f));
    auto lower_teal   = std::make_shared<Lambertian>(color(0.2f, 0.8f, 0.8f));

	// Quads
	world.add(std::make_shared<Quad>(vec3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
	world.add(std::make_shared<Quad>(vec3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(std::make_shared<Quad>(vec3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(std::make_shared<Quad>(vec3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(std::make_shared<Quad>(vec3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

	Camera camera;
	camera.Width = 400;
	camera.Height = 225;
	camera.SPP = 100;
	camera.maxDepth = 50;
	camera.background = color(0.70f, 0.80f, 1.00f);

	camera.vfov = 80.0f;
	camera.lookFrom = vec3(0.0f, 0.0f, 9.0f);
	camera.lookAt = vec3(0.0f, 0.0f, 0.0f);
	camera.vUp = vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.0f;

	TIMER(camera.render(HittableList(world)));
}

void simpleLight() {
	HittableList world;

	auto pertext = std::make_shared<NoiseTexture>(4);
	world.add(std::make_shared<Sphere>(vec3(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
	world.add(std::make_shared<Sphere>(vec3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

	auto difflight = std::make_shared<DiffuseLight>(color(4, 4, 4));
	world.add(std::make_shared<Sphere>(vec3(0, 7, 0), 2, difflight));
	world.add(std::make_shared<Quad>(vec3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));

	Camera camera;
	camera.Width = 400;
	camera.Height = 225;
	camera.SPP = 100;
	camera.maxDepth = 50;
	camera.background = color(0, 0, 0);

	camera.vfov = 20.0f;
	camera.lookFrom = vec3(26.0f, 3.0f, 6.0f);
	camera.lookAt = vec3(0.0f, 2.0f, 0.0f);
	camera.vUp = vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.0f;

	TIMER(camera.render(HittableList(world)));
}

void cornellBox() {
	HittableList world;

	auto red = std::make_shared<Lambertian>(color(0.65f, 0.05f, 0.05f));
	auto white = std::make_shared<Lambertian>(color(0.73f, 0.73f, 0.73f));
	auto green = std::make_shared<Lambertian>(color(0.12f, 0.45f, 0.15f));
	auto light = std::make_shared<DiffuseLight>(color(15, 15, 15));

	world.add(std::make_shared<Quad>(vec3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
	world.add(std::make_shared<Quad>(vec3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
	world.add(std::make_shared<Quad>(vec3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light));
	world.add(std::make_shared<Quad>(vec3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
	world.add(std::make_shared<Quad>(vec3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
	world.add(std::make_shared<Quad>(vec3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

	std::shared_ptr<Hittable> box1 = box(vec3(0, 0, 0), vec3(165, 330, 165), white);
	box1 = std::make_shared<RotateY>(box1, 15);
	box1 = std::make_shared<Translate>(box1, vec3(265, 0, 295));
	world.add(box1);

	std::shared_ptr<Hittable> box2 = box(vec3(0, 0, 0), vec3(165, 165, 165), white);
	box2 = std::make_shared<RotateY>(box2, -18);
	box2 = std::make_shared<Translate>(box2, vec3(130, 0, 65));
	world.add(box2);

	Camera camera;
	camera.Width = 600;
	camera.Height = 600;
	camera.SPP = 200;
	camera.maxDepth = 50;
	camera.background = color(0, 0, 0);

	camera.vfov = 40.0f;
	camera.lookFrom = vec3(278.0f, 278.0f, -800.0f);
	camera.lookAt = vec3(278.0f, 278.0f, 0.0f);
	camera.vUp = vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.0f;

	TIMER(camera.render(HittableList(world)));
}

void cornellSmoke() {
	HittableList world;

	auto red = std::make_shared<Lambertian>(color(0.65f, 0.05f, 0.05f));
	auto white = std::make_shared<Lambertian>(color(0.73f, 0.73f, 0.73f));
	auto green = std::make_shared<Lambertian>(color(0.12f, 0.45f, 0.15f));
	auto light = std::make_shared<DiffuseLight>(color(7, 7, 7));

	world.add(std::make_shared<Quad>(vec3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
	world.add(std::make_shared<Quad>(vec3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
	world.add(std::make_shared<Quad>(vec3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
	world.add(std::make_shared<Quad>(vec3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
	world.add(std::make_shared<Quad>(vec3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
	world.add(std::make_shared<Quad>(vec3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

	std::shared_ptr<Hittable> box1 = box(vec3(0, 0, 0), vec3(165, 330, 165), white);
	box1 = std::make_shared<RotateY>(box1, 15);
	box1 = std::make_shared<Translate>(box1, vec3(265, 0, 295));
	
	std::shared_ptr<Hittable> box2 = box(vec3(0, 0, 0), vec3(165, 165, 165), white);
	box2 = std::make_shared<RotateY>(box2, -18);
	box2 = std::make_shared<Translate>(box2, vec3(130, 0, 65));
	
	world.add(std::make_shared<ConstantMedium>(box1, 0.01f, color(0, 0, 0)));
	world.add(std::make_shared<ConstantMedium>(box2, 0.01f, color(1, 1, 1)));

	Camera camera;
	camera.Width = 600;
	camera.Height = 600;
	camera.SPP = 200;
	camera.maxDepth = 50;
	camera.background = color(0, 0, 0);

	camera.vfov = 40.0f;
	camera.lookFrom = vec3(278.0f, 278.0f, -800.0f);
	camera.lookAt = vec3(278.0f, 278.0f, 0.0f);
	camera.vUp = vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.0f;

	TIMER(camera.render(HittableList(world)));
}

void finalScene(int imageWidth, int imageHeight, int SPP, int maxDepth) {
	HittableList boxes1;
	auto ground = std::make_shared<Lambertian>(color(0.48f, 0.83f, 0.53f));

	int boxesPerSide = 20;
	for (int i = 0; i < boxesPerSide; ++i) {
		for (int j = 0; j < boxesPerSide; ++j) {
			float w = 100.0f;
			float x0 = -1000.0f + i * w;
			float z0 = -1000.0f + j * w;
			float y0 = 0.0f;
			float x1 = x0 + w;
			float y1 = randomFloat(1, 101);
			float z1 = z0 + w;

			boxes1.add(box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground));
		}
	}

	HittableList world;

	world.add(std::make_shared<BVHNode>(boxes1));

	auto light = std::make_shared<DiffuseLight>(color(7, 7, 7));
	world.add(std::make_shared<Quad>(vec3(123, 554, 147), vec3(300, 0, 0), vec3(0, 0, 265), light));

	auto center1 = vec3(400, 400, 200);
	auto center2 = center1 + vec3(30, 0, 0);
	auto sphereMaterial = std::make_shared<Lambertian>(color(0.7f, 0.3f, 0.1f));
	world.add(std::make_shared<Sphere>(center1, center2, 50, sphereMaterial));

	world.add(std::make_shared<Sphere>(vec3(260, 150, 45), 50, std::make_shared<Dielectric>(1.5f)));
	world.add(std::make_shared<Sphere>(
		vec3(0, 150, 145), 50, std::make_shared<Metal>(color(0.8f, 0.8f, 0.9f), 1.0f)
	));

	auto boundary = std::make_shared<Sphere>(vec3(360, 150, 145), 70, std::make_shared<Dielectric>(1.5f));
	world.add(boundary);
	world.add(std::make_shared<ConstantMedium>(boundary, 0.2f, color(0.2f, 0.4f, 0.9f)));
	boundary = std::make_shared<Sphere>(vec3(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5f));
	world.add(std::make_shared<ConstantMedium>(boundary, 0.0001f, color(1, 1, 1)));

	auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("image/earthmap.jpg"));
	world.add(std::make_shared<Sphere>(vec3(400, 200, 400), 100, emat));
	auto pertext = std::make_shared<NoiseTexture>(0.2f);
    world.add(std::make_shared<Sphere>(vec3(220, 280, 300), 80, std::make_shared<Lambertian>(pertext)));

	HittableList boxes2;
	auto white = std::make_shared<Lambertian>(color(0.73f, 0.73f, 0.73f));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
        boxes2.add(std::make_shared<Sphere>(vec3::Random(0, 165), 10, white));
    }

	world.add(std::make_shared<Translate>(
		std::make_shared<RotateY>(
			std::make_shared<BVHNode>(boxes2), 15
		),
		vec3(-100, 270, 395)
	));

	Camera camera;

	camera.Width = imageWidth;
	camera.Height = imageHeight;
	camera.SPP = SPP;
	camera.maxDepth = maxDepth;
	camera.background = color(0, 0, 0);

	camera.vfov = 40.0f;
	camera.lookFrom = vec3(478.0f, 278.0f, -600.0f);
	camera.lookAt = vec3(278.0f, 278.0f, 0.0f);
	camera.vUp = vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.0f;

	TIMER(camera.render(world));
}

int main()
{
	switch (10) {
		case 1: bouncingSpheres(); break;
		case 2: checkeredSpheres(); break;
		case 3: earth(); break;
		case 4: perlinSpheres(); break;
		case 5: quads(); break;
		case 6: simpleLight(); break;
		case 7: cornellBox(); break;
		case 8: cornellSmoke(); break;
		case 9: finalScene(800, 800, 10000, 40); break;
		default: finalScene(400, 400, 250, 4); break;
	}
}