#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Objects2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float lengthX, float lengthY, glm::vec3 color, bool fill = false);
	Mesh* CreatePlatform(std::string name, glm::vec3 leftBottomCorner, float lengthY);
	Mesh* CreatePlayer(std::string name, glm::vec3 leftBottomCorner);
	Mesh* CreateAsteriod1(std::string name, glm::vec3 leftBottomCorner);
	//Mesh* CreateAsteroid2(std::string name, glm::vec3 leftBottomCorner);

}

