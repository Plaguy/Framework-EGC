/* Tema 1 - AstroKity
Source: astrokity.cpp
Copyright 2017-2018: Diana Dumitrescu (333CB) */


#include <vector>
#include <iostream>

#include <Core/Engine.h>

#include "AstroKity.h"

#include <vector>
#include <iostream>
#include <unordered_map>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Objects2D.h"
#include "UtilsKity.h"

using namespace std;

AstroKity::AstroKity()
{
	p = new Animation2D(1.8f, 0.01f, 0, 0, 0, 0);
	obstacle1 = new Animation2D(2.7f, 1.4f, 0, 0, 0, 0);
	obstacle2 = new Animation2D(0.5f, 2.1f, 0, 0, 0, 0);
	obstacle3 = new Animation2D(0.5f, 2.6f, 0, 0, 0, 0);
	obstacle4 = new Animation2D(2.7f, 4.2f, 0, 0, 0, 0);
	obstacle5 = new Animation2D(2.4f, 4.7f, 0, 0, 0, 0);
	obstacle6 = new Animation2D(3.1f, 4.7f, 0, 0, 0, 0);
	obstacle7 = new Animation2D(2.7f, 5.1f, 0, 0, 0, 0);
}

AstroKity::~AstroKity()
{
	delete(p);
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

	//Create the arena; add it to the meshes vector
	Mesh* arena = Objects2D::CreateRectangle("arena", corner, 4, 6.2f, glm::vec3(0, 0.1f, 0.1f));
	AddMeshToList(arena);

	//Create the player; add it to the meshes vector
	Mesh* player = Objects2D::CreatePlayer("player", corner);
	AddMeshToList(player);
	
	//Create reflective platforms; add them to the meshes vector
	Mesh* reflective1 = Objects2D::CreateReflectivePlatform("reflective1", corner, 1.5f);
	AddMeshToList(reflective1);
	Mesh* reflective2 = Objects2D::CreateReflectivePlatform("reflective2", corner, 1.2f);
	AddMeshToList(reflective2);

	//Create stationary platforms; add them to the meshes vector
	Mesh* stationary1 = Objects2D::CreateStationaryPlatform("stationary1", corner, 1.9f);
	AddMeshToList(stationary1);
	Mesh* stationary2 = Objects2D::CreateStationaryPlatform("stationary2", corner, 1.5f);
	AddMeshToList(stationary2);

	//Create asteroids; add them to the meshes vector
	Mesh* asteroid1 = Objects2D::CreateAsteroid1("asteroid1", corner);
	AddMeshToList(asteroid1);
	Mesh* asteroid2 = Objects2D::CreateAsteroid2("asteroid2", corner);
	AddMeshToList(asteroid2);

	//Create finish platform; add it to the meshes vector
	Mesh* finish = Objects2D::CreateFinishPlatform("finish", corner);
	AddMeshToList(finish);

    //Initialize the transformation factors
    translateX = 0; //translation is between 0 and 1
    translateY = 0; //up
    angularStep = 0; //rotation
    scaleX = 0; //scaling is between 0 and 1
    scaleY = 0;


	s1 = 0;
	s2 = 0;
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
	visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
	DrawScene(visMatrix);
    
    angularStep += deltaTimeSeconds;
    //translateX = UpdateFactor(translateX, 0.1f, 0, 3);

	//obstacle1->
	s1 = UtilsKity::GetStatus(scaleX, 0, 1, s1);
	s2 = UtilsKity::GetStatus(scaleY, 0, 1, s2);
	translateX = UtilsKity::UpdateFactor(translateX, deltaTimeSeconds, s1);
    scaleX = UtilsKity::UpdateFactor(scaleX, deltaTimeSeconds, s1);
    scaleY = UtilsKity::UpdateFactor(scaleY, deltaTimeSeconds, s2);
}

void AstroKity::FrameEnd()
{

}

void AstroKity::DrawScene(glm::mat3 visMatrix)
{
	modelMatrix1 = visMatrix * Transform2D::Translate(0, 0);
	RenderMesh2D(meshes["arena"], shaders["VertexColor"], modelMatrix1);

	modelMatrix1 = visMatrix * Transform2D::Translate(p->translateX, p->translateY);
	RenderMesh2D(meshes["player"], shaders["VertexColor"], modelMatrix1);
	
	modelMatrix1 = visMatrix * Transform2D::Translate(0, 1);
	RenderMesh2D(meshes["reflective1"], shaders["VertexColor"], modelMatrix1);

	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle1->translateX, obstacle1->translateY);
    modelMatrix1 = modelMatrix1 * Transform2D::Rotate(angularStep);
	RenderMesh2D(meshes["asteroid1"], shaders["VertexColor"], modelMatrix1);

	modelMatrix1 = visMatrix * Transform2D::Translate(1.9f, 2.2f);
	RenderMesh2D(meshes["stationary1"], shaders["VertexColor"], modelMatrix1);

	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle2->translateX, obstacle2->translateY);
    modelMatrix1 = modelMatrix1 * Transform2D::Scale(scaleX, scaleY);
	RenderMesh2D(meshes["asteroid2"], shaders["VertexColor"], modelMatrix1);

	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle3->translateX, obstacle3->translateY);
    modelMatrix1 = modelMatrix1 * Transform2D::Translate(translateX, 0);
	RenderMesh2D(meshes["asteroid2"], shaders["VertexColor"], modelMatrix1);

	modelMatrix1 = visMatrix * Transform2D::Translate(1.1f, 3.4f);
	RenderMesh2D(meshes["reflective2"], shaders["VertexColor"], modelMatrix1);

	modelMatrix1 = visMatrix * Transform2D::Translate(0.1f, 4.8f);
	RenderMesh2D(meshes["stationary2"], shaders["VertexColor"], modelMatrix1);
	
	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle4->translateX, obstacle4->translateY);
	RenderMesh2D(meshes["asteroid1"], shaders["VertexColor"], modelMatrix1);
	
	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle5->translateX, obstacle5->translateY);
	RenderMesh2D(meshes["asteroid2"], shaders["VertexColor"], modelMatrix1);

	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle6->translateX, obstacle6->translateY);
	RenderMesh2D(meshes["asteroid2"], shaders["VertexColor"], modelMatrix1);

	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle7->translateX, obstacle7->translateY);
	RenderMesh2D(meshes["asteroid1"], shaders["VertexColor"], modelMatrix1);

	modelMatrix1 = visMatrix * Transform2D::Translate(2.3f, 6.1f);
	RenderMesh2D(meshes["finish"], shaders["VertexColor"], modelMatrix1);
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