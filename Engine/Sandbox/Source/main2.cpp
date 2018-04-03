#include <XYZ/Engine.hpp>

#include <random>

#include <glm/ext.hpp>
#include <fstream>
#include <sys/param.h>
#include <zconf.h>
#include <XYZ/Terrain/Manager/Quadtree/TerrainQuadtree.hpp>
#include <XYZ/Terrain/Manager/Quadtree/QuadtreeTerrainManager.hpp>

using namespace XYZ;

glm::vec3 reverseProjection(const glm::mat4& transform, const glm::vec3& position) {
	auto inversed = inverse(transform) * glm::vec4(position, 1.0);
	return glm::vec3(inversed / inversed.w);
}

int main() {
	auto camera = std::make_shared<Scene::Camera>();
	camera->setPosition(glm::vec3(100.0f, 0.0, 0.0));
	camera->Yaw = -30.0;
	camera->updateCameraVectors();

	Terrain::Manager::Quadtree::QuadtreeTerrainManager manager(nullptr, camera);

	glm::vec3 cameraPosition = camera->getPosition();
//	glm::vec3 cameraLookAt = glm::vec3(10.0, 0.0, 0.0);

	auto projection = glm::perspective(glm::radians(camera->getFieldOfView()), 1.0f, camera->getZNear(), camera->getZFar());
	auto view = glm::lookAt(cameraPosition, cameraPosition + camera->getFront(), glm::vec3(0.0, 1.0, 0.0));
	auto transform = projection * view;
	auto inverseTransform = inverse(transform);

	glm::vec3 bounds[2][2][2];
	for(int i = -1; i <= 1; i = i + 2) {
		for(int j = -1; j <= 1; j = j + 2) {
			for(int k = -1; k <= 1; k = k + 2) {
				auto inversed = inverse(transform) * glm::vec4(glm::vec3(i, j, k), 1.0);
				bounds[(i + 1) / 2][(j + 1) / 2][(k + 1) / 2] = glm::vec3(inversed / inversed.w);
			}
		}
	}

	glm::vec3 max, min;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			for(int k = 0; k < 2; k++) {
				max.x = std::max(max.x, bounds[i][j][k].x);
				max.y = std::max(max.y, bounds[i][j][k].y);
				max.z = std::max(max.z, bounds[i][j][k].z);

				min.x = std::min(min.x, bounds[i][j][k].x);
				min.y = std::min(min.y, bounds[i][j][k].y);
				min.z = std::min(min.z, bounds[i][j][k].z);
				
				std::cout << "[" << i << "]" << "[" << j << "]" << "[" << k << "] = " << to_string(bounds[i][j][k])
						  << std::endl;
			}
		}
	}

	float size = glm::ceilMultiple(std::max(
			std::max(
					std::max(bounds[0][0][1].x, bounds[0][0][1].z),
					std::max(bounds[1][0][1].x, bounds[1][0][1].z)
			),
			std::max(
					std::max(bounds[1][1][1].x, bounds[1][1][1].z),
					std::max(bounds[0][1][1].x, bounds[0][1][1].z)
			)
	), 16.0f);
	std::cout << size << std::endl;

	Terrain::Manager::Quadtree::TerrainQuadtree quadtree(
			0, {{0,    0},
				{size, size}}
	);

	quadtree.update(cameraPosition);

	// print a quadtree debug
	std::ofstream os("debug.svg");
	os << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl
	   << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">" << std::endl;
	os << quadtree.toSVG();
	glm::vec2 frustumViewer = glm::vec2(cameraPosition.x, cameraPosition.z) + size;
	os << "<circle cx=\"" << frustumViewer.x << "\" cy=\"" << frustumViewer.y
	   << "\" r=\"3.0\" fill=\"red\" fill-opacity=\"0.5\" />" << std::endl;

	std::string origin1 = std::to_string(bounds[1][0][0].x + size) + " " + std::to_string(bounds[1][0][0].z + size);
	std::string origin2 = std::to_string(bounds[0][0][0].x + size) + " " + std::to_string(bounds[0][0][0].z + size);
	std::string corner1 = std::to_string(bounds[0][0][1].x + size) + " " + std::to_string(bounds[0][0][1].z + size);
	std::string corner2 = std::to_string(bounds[1][0][1].x + size) + " " + std::to_string(bounds[1][0][1].z + size);

	std::string points = origin1 + ", " + origin2 + ", " + corner1 + ", " + corner2;

	os << "<polygon points=\"" << points << "\"\n"
			"      fill=\"red\" stroke-width=\"0.5\" fill-opacity=\"0.2\"/>" << std::endl;

	origin1 = std::to_string(bounds[0][1][0].x + size) + " " + std::to_string(bounds[0][1][0].z + size);
	origin2 = std::to_string(bounds[1][1][0].x + size) + " " + std::to_string(bounds[1][1][0].z + size);
	corner1 = std::to_string(bounds[1][1][1].x + size) + " " + std::to_string(bounds[1][1][1].z + size);
	corner2 = std::to_string(bounds[0][1][1].x + size) + " " + std::to_string(bounds[0][1][1].z + size);

	points = origin1 + ", " + origin2 + ", " + corner1 + ", " + corner2;
	
	// culling box
	os << "<polygon points=\"" << points << "\"\n"
			"      fill=\"red\" stroke-width=\"0.5\" fill-opacity=\"0.2\"/>" << std::endl;

	origin1 = std::to_string(min.x + size) + " " + std::to_string(min.z + size);
	origin2 = std::to_string(min.x + size) + " " + std::to_string(max.z + size);
	corner1 = std::to_string(max.x + size) + " " + std::to_string(max.z + size);
	corner2 = std::to_string(max.x + size) + " " + std::to_string(min.z + size);

	points = origin1 + ", " + origin2 + ", " + corner1 + ", " + corner2;

	glm::vec2 boxMax = glm::vec2(max.x, max.z);
	glm::vec2 boxMin = glm::vec2(min.x, min.z);

	glm::vec2 centre = (boxMax + boxMin) / 2.0f;
	glm::vec2 halfSize = (boxMax - boxMin) / 2.0f;

	std::cout << to_string(centre) << std::endl;
	std::cout << to_string(halfSize) << std::endl;

//	auto xy = centre - halfSize + size;
//	auto wh = halfSize * 2.0f;
//
//	os << "<rect x=\"" << xy.x << "\" y=\"" << xy.y
//	   << "\" width=\"" << wh.x << "\" height=\"" << wh.y
//	   << "\" fill=\"orange\" stroke-width=\"0.5\" stroke=\"black\" fill-opacity=\"0.5\" />" << std::endl;

	for(auto& box : quadtree.queryRange({centre, halfSize})) {
		auto xy = box.first.centre - box.first.halfSize + size;
		auto wh = box.first.halfSize * 2.0f;

		os << "<rect x=\"" << xy.x << "\" y=\"" << xy.y
		   << "\" width=\"" << wh.x << "\" height=\"" << wh.y
		   << "\" fill=\"orange\" stroke-width=\"0.5\" stroke=\"black\" fill-opacity=\"0.5\" />" << std::endl;
	}

	os << "<polygon points=\"" << points << "\"\n"
			"      fill=\"blue\" stroke-width=\"0.5\" fill-opacity=\"0.2\"/>" << std::endl;

	os << "</svg>" << std::endl;

}
