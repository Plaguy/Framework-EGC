#include "Objects2D.h"

#include <Core/Engine.h>

Mesh* Objects2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Objects2D::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float lengthX, float lengthY, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(lengthX, 0, 0), color),
		VertexFormat(corner + glm::vec3(lengthX, lengthY, 0), color),
		VertexFormat(corner + glm::vec3(0, lengthY, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

Mesh* Objects2D::CreateStationaryPlatform(std::string name, glm::vec3 leftBottomCorner, float lengthX)
{
	glm::vec3 corner = leftBottomCorner;
	float lengthY = 0.2f;
	glm::vec3 customColor1 = glm::vec3(0, 0.6f, 1);
	glm::vec3 customColor2 = glm::vec3(1, 0.6f, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, customColor1),
		VertexFormat(corner + glm::vec3(lengthX, 0, 0), customColor1),
		VertexFormat(corner + glm::vec3(lengthX, lengthY, 0), customColor2),
		VertexFormat(corner + glm::vec3(0, lengthY, 0), customColor2)
	};

	Mesh* platform = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	indices.push_back(0);
	indices.push_back(2);

	platform->InitFromData(vertices, indices);
	return platform;
}

Mesh* Objects2D::CreateReflectivePlatform(std::string name, glm::vec3 leftBottomCorner, float lengthX)
{
	glm::vec3 corner = leftBottomCorner;
	float lengthY = 0.2f;
	glm::vec3 customColor1 = glm::vec3(1, 0, 1);
	glm::vec3 customColor2 = glm::vec3(1, 0.6f, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, customColor1),
		VertexFormat(corner + glm::vec3(lengthX, 0, 0), customColor1),
		VertexFormat(corner + glm::vec3(lengthX, lengthY, 0), customColor2),
		VertexFormat(corner + glm::vec3(0, lengthY, 0), customColor2)
	};

	Mesh* platform = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	indices.push_back(0);
	indices.push_back(2);

	platform->InitFromData(vertices, indices);
	return platform;
}

Mesh* Objects2D::CreateFinishPlatform(std::string name, glm::vec3 leftBottomCorner)
{
	glm::vec3 corner = leftBottomCorner;
	float lengthY = 0.1f;
	float lengthX = 1.3f;
	glm::vec3 customColor1 = glm::vec3(0, 1, 1);
	glm::vec3 customColor2 = glm::vec3(1, 1, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, customColor1),
		VertexFormat(corner + glm::vec3(lengthX, 0, 0), customColor1),
		VertexFormat(corner + glm::vec3(lengthX, lengthY, 0), customColor2),
		VertexFormat(corner + glm::vec3(0, lengthY, 0), customColor2)
	};

	Mesh* platform = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	indices.push_back(0);
	indices.push_back(2);

	platform->InitFromData(vertices, indices);
	return platform;
}

Mesh* Objects2D::CreatePlayer(std::string name, glm::vec3 leftBottomCorner)
{
	glm::vec3 corner = leftBottomCorner;
	glm::vec3 blue = glm::vec3(0, 0, 1);
	glm::vec3 green = glm::vec3(0, 1, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, green),
		VertexFormat(corner + glm::vec3(0.2f, 0.3f, 0), blue),
		VertexFormat(corner + glm::vec3(0.4f, 0, 0), green)
	};
	Mesh* player = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 };
	 
	player->InitFromData(vertices, indices);
	return player;
}

Mesh* Objects2D::CreateAsteroid1(std::string name, glm::vec3 leftBottomCorner)
{
	glm::vec3 corner = leftBottomCorner;
	glm::vec3 customColor1 = glm::vec3(1.0f, 0, 0);
	glm::vec3 customColor2 = glm::vec3(0, 0.6f, 1.0f);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0.1f, 0, 0), customColor1),
		VertexFormat(corner + glm::vec3(0, 0.2f, 0), customColor1),
		VertexFormat(corner + glm::vec3(0.1f, 0.4f, 0), customColor1),
		VertexFormat(corner + glm::vec3(0.3f, 0.4f, 0), customColor2),
		VertexFormat(corner + glm::vec3(0.4f, 0.2f, 0), customColor2),
		VertexFormat(corner + glm::vec3(0.3f, 0, 0), customColor2)
	};
	Mesh* asteroid = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5 };

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(5);

	asteroid->InitFromData(vertices, indices);
	return asteroid;
}

Mesh* Objects2D::CreateAsteroid2(std::string name, glm::vec3 leftBottomCorner)
{
	glm::vec3 corner = leftBottomCorner;
	glm::vec3 customColor1 = glm::vec3(0.5f, 0.5f, 0);
	glm::vec3 customColor2 = glm::vec3(0, 0.5f, 0.5f);
	glm::vec3 customColor3 = glm::vec3(0.5f, 0, 0.5f);
	glm::vec3 customColor4 = glm::vec3(0.5f, 0.5f, 0.5f);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0.1f, 0, 0), customColor4),
		VertexFormat(corner + glm::vec3(0, 0.1f, 0), customColor1),
		VertexFormat(corner + glm::vec3(0, 0.2f, 0), customColor1),
		VertexFormat(corner + glm::vec3(0.1f, 0.3f, 0), customColor2),
		VertexFormat(corner + glm::vec3(0.2f, 0.3f, 0), customColor2),
		VertexFormat(corner + glm::vec3(0.3f, 0.2f, 0), customColor3),
		VertexFormat(corner + glm::vec3(0.3f, 0.1f, 0), customColor3),
		VertexFormat(corner + glm::vec3(0.2f, 0, 0), customColor4)
	};
	Mesh* asteroid = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 2, 3, 4, 0, 2, 4, 0, 4, 6, 4, 5, 6, 0, 6, 7 };

	asteroid->InitFromData(vertices, indices);
	return asteroid;
}
