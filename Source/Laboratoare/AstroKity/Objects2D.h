#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Objects2D
{
	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float lengthX, float lengthY, glm::vec3 color, bool fill = false);
	Mesh* CreateStationaryPlatform(std::string name, glm::vec3 leftBottomCorner, float lengthY);
	Mesh* CreateReflectivePlatform(std::string name, glm::vec3 leftBottomCorner, float lengthY);
	Mesh* CreateFinishPlatform(std::string name, glm::vec3 leftBottomCorner);
	Mesh* CreatePlayer(std::string name, glm::vec3 leftBottomCorner);
	Mesh* CreateAsteroid1(std::string name, glm::vec3 leftBottomCorner);
	Mesh* CreateAsteroid2(std::string name, glm::vec3 leftBottomCorner);

	//vectors
	float CalculateVectorLength(glm::vec3 vector);
	glm::vec3 NormalizeVector(glm::vec3 vector, float length);
}

