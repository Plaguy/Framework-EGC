/* Tema 1 - AstroKity
Source: astrokity.cpp
Copyright 2017-2018: Diana Dumitrescu (333CB) */


#include <vector>
#include <iostream>

#include <Core/Engine.h>

#include "AstroKity.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Objects2D.h"

using namespace std;

AstroKity::AstroKity()
{
}

AstroKity::~AstroKity()
{
}

void AstroKity::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	logicSpace.x = 0;		// logic x
	logicSpace.y = 0;		// logic y
	logicSpace.width = 4;	// logic width
	logicSpace.height = 4;	// logic height

	glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
	length = 0.99f;
	lengthX = 0.99f;
	lengthY = 1.98f;
	bool fill = true;

	Mesh* square1 = Objects2D::CreateSquare("square1", corner, length, glm::vec3(1, 0, 0));
	Mesh* rect1 = Objects2D::CreatePlatform("rect1", corner, lengthY);
	Mesh* player = Objects2D::CreatePlayer("player", corner);
	Mesh* asteroid1 = Objects2D::CreateAsteriod1("asteroid1", corner);
	AddMeshToList(square1);
	AddMeshToList(rect1);
	AddMeshToList(player);
	AddMeshToList(asteroid1);
}

// 2D visualization matrix
glm::mat3 AstroKity::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
}

// uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 AstroKity::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

void AstroKity::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}

void AstroKity::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void AstroKity::Update(float deltaTimeSeconds)
{

	glm::ivec2 resolution = window->GetResolution();

	// Sets the screen area where to draw - the left half of the window
	viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	SetViewportArea(viewSpace, glm::vec3(0), true);

	// Compute the 2D visualization matrix
	visMatrix = glm::mat3(1);
	//visMatrix *= VisualizationTransf2D(logicSpace, viewSpace);

	//DrawScene(visMatrix);

	// The viewport is now the right half of the window

	//viewSpace = ViewportSpace(resolution.x / 2, 0, resolution.x / 2, resolution.y);
	//SetViewportArea(viewSpace, glm::vec3(0.5f), true);

	// Compute uniform 2D visualization matrix
	//visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
	DrawScene(visMatrix);
}

void AstroKity::FrameEnd()
{

}

void AstroKity::DrawScene(glm::mat3 visMatrix)
{
	modelMatrix1 = visMatrix * Transform2D::Translate(0, 0);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix1);

	modelMatrix1 = visMatrix * Transform2D::Translate(3, 0);
	RenderMesh2D(meshes["rect1"], shaders["VertexColor"], modelMatrix1);

	modelMatrix1 = visMatrix * Transform2D::Translate(1.5, 1.5);
	RenderMesh2D(meshes["player"], shaders["VertexColor"], modelMatrix1);

	
	modelMatrix1 = visMatrix * Transform2D::Translate(0, 3);
	RenderMesh2D(meshes["asteroid1"], shaders["VertexColor"], modelMatrix1);
	/*
	modelMatrix1 = visMatrix * Transform2D::Translate(3, 3);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix1);*/
}

void AstroKity::OnInputUpdate(float deltaTime, int mods)
{
	//TODO move the logic window with W, A, S, D (up, left, down, right)
	if (window->KeyHold(GLFW_KEY_W)) {
		logicSpace.y -= deltaTime * 10;
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		logicSpace.y += deltaTime * 10;
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		logicSpace.x += deltaTime * 10;
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		logicSpace.x -= deltaTime * 10;
	}
	//TODO zoom in and zoom out logic window with Z and X
	if (window->KeyHold(GLFW_KEY_Z)) {
		logicSpace.width -= deltaTime * float(1);
		logicSpace.height -= deltaTime * float(1);
	}
	if (window->KeyHold(GLFW_KEY_X)) {
		logicSpace.width += deltaTime * float(1);
		logicSpace.height += deltaTime * float(1);
	}
}

void AstroKity::OnKeyPress(int key, int mods)
{

}

void AstroKity::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void AstroKity::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void AstroKity::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void AstroKity::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void AstroKity::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}