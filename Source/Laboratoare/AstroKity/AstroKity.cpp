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
	// Parameters for Animation2D: translateX, translateY, scaleX, scaleY, angularStep, status
	// Initial parameters
	p = new Animation2D(1.8f, 0.01f, 0, 0, 0, 0);
	obstacle1 = new Animation2D(2.7f, 1.4f, 0, 0, 0, 0);
	obstacle2 = new Animation2D(1, 2.1f, 0, 0, 0, 0);
	obstacle3 = new Animation2D(1, 2.6f, 0, 0, 0, 0);
	obstacle4 = new Animation2D(3, 4.2f, 0, 0, 0, 0);
	obstacle5 = new Animation2D(2.4f, 4.7f, 0, 0, 0, 0);
	obstacle6 = new Animation2D(3.5f, 4.7f, 0, 0, 0, 0);
	obstacle7 = new Animation2D(3, 5.1f, 0, 0, 0, 0);
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

	//Launch param: 0 - ready, 1 - fire
	launch = 0;
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

glm::vec3 AstroKity::TransformViewPoint(const LogicSpace &logicSpace, const ViewportSpace &viewSpace, glm::vec3 point)
{
	float sx, sy, tx, ty, smin, xf, yf;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;
	xf = (point[0] - tx) / smin ;
	yf = (point[1] - ty) / smin;
	return glm::vec3(xf, yf, 0);
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

	//Animate player
	if (launch == 1)
	{
		p->status = UtilsKity::GetStatus(p->translateX, p->mintX, p->maxtX, p->status);
		p->translateX = UtilsKity::UpdateFactor(p->translateX, deltaTimeSeconds, p->status);
		p->status = UtilsKity::GetStatus(p->translateY, p->mintY, p->maxtY, p->status);
		p->translateY = UtilsKity::UpdateFactor(p->translateY, deltaTimeSeconds, p->status);
		
	}

	//Animate obstacle1
	obstacle1->angularStep += deltaTimeSeconds;
	
	//Animate obstacle2
	obstacle2->status = UtilsKity::GetStatus(obstacle2->scaleX, 0.5f, 1, obstacle2->status);
	obstacle2->scaleX = UtilsKity::UpdateFactor(obstacle2->scaleX, deltaTimeSeconds, obstacle2->status);
	obstacle2->status = UtilsKity::GetStatus(obstacle2->scaleY, 0.5f, 1, obstacle2->status);
	obstacle2->scaleY = UtilsKity::UpdateFactor(obstacle2->scaleY, deltaTimeSeconds, obstacle2->status);

	//Animate obstacle3
	obstacle3->status = UtilsKity::GetStatus(obstacle3->translateX, 0.2f, 0.8f, obstacle3->status);
	obstacle3->translateX = UtilsKity::UpdateFactor(obstacle3->translateX, deltaTimeSeconds, obstacle3->status);

	//Animate obstacle4
	obstacle4->angularStep += deltaTimeSeconds;

	//Animate obstacle5
	obstacle5->status = UtilsKity::GetStatus(obstacle5->scaleX, 1, 1.5f, obstacle5->status);
	obstacle5->scaleX = UtilsKity::UpdateFactor(obstacle5->scaleX, deltaTimeSeconds, obstacle5->status);
	obstacle5->status = UtilsKity::GetStatus(obstacle5->scaleY, 1, 1.5f, obstacle5->status);
	obstacle5->scaleY = UtilsKity::UpdateFactor(obstacle5->scaleY, deltaTimeSeconds, obstacle5->status);

	//Animate obstacle6
	obstacle6->status = UtilsKity::GetStatus(obstacle6->scaleX, 1, 1.5f, obstacle6->status);
	obstacle6->scaleX = UtilsKity::UpdateFactor(obstacle6->scaleX, deltaTimeSeconds, obstacle6->status);
	obstacle6->status = UtilsKity::GetStatus(obstacle6->scaleY, 1, 1.5f, obstacle6->status);
	obstacle6->scaleY = UtilsKity::UpdateFactor(obstacle6->scaleY, deltaTimeSeconds, obstacle6->status);

	//Animate obstacle7
	obstacle7->status = UtilsKity::GetStatus(obstacle7->translateY, 5.1f, 5.7f, obstacle7->status);
	obstacle7->translateY = UtilsKity::UpdateFactor(obstacle7->translateY, deltaTimeSeconds, obstacle7->status);
}

void AstroKity::FrameEnd()
{

}

void AstroKity::DrawScene(glm::mat3 visMatrix)
{
	//Draw arena
	modelMatrix1 = visMatrix * Transform2D::Translate(0, 0);
	RenderMesh2D(meshes["arena"], shaders["VertexColor"], modelMatrix1);

	//Draw player
	modelMatrix1 = visMatrix * Transform2D::Translate(p->translateX, p->translateY);
	RenderMesh2D(meshes["player"], shaders["VertexColor"], modelMatrix1);
	
	//Draw reflective surface 1
	modelMatrix1 = visMatrix * Transform2D::Translate(0, 1);
	RenderMesh2D(meshes["reflective1"], shaders["VertexColor"], modelMatrix1);

	//Draw obstacle1 type asteroid1 - decagon
	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle1->translateX, obstacle1->translateY);
    modelMatrix1 *= Transform2D::Rotate(obstacle1->angularStep);
	RenderMesh2D(meshes["asteroid1"], shaders["VertexColor"], modelMatrix1);

	//Draw stationary surface 1
	modelMatrix1 = visMatrix * Transform2D::Translate(1.9f, 2.2f);
	RenderMesh2D(meshes["stationary1"], shaders["VertexColor"], modelMatrix1);

	//Draw obstacle2 type asteroid2 - octagon
	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle2->translateX, obstacle2->translateY);
    modelMatrix1 *= Transform2D::Scale(obstacle2->scaleX, obstacle2->scaleY);
	RenderMesh2D(meshes["asteroid2"], shaders["VertexColor"], modelMatrix1);

	//Draw obstacle3 type asteroid2 - octagon
	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle3->translateX, obstacle3->translateY);
    modelMatrix1 *= Transform2D::Translate(obstacle3->translateX, 0);
	RenderMesh2D(meshes["asteroid2"], shaders["VertexColor"], modelMatrix1);

	//Draw reflective surface 2
	modelMatrix1 = visMatrix * Transform2D::Translate(1.1f, 3.4f);
	RenderMesh2D(meshes["reflective2"], shaders["VertexColor"], modelMatrix1);

	//Draw stationary surface 2
	modelMatrix1 = visMatrix * Transform2D::Translate(0.1f, 4.8f);
	RenderMesh2D(meshes["stationary2"], shaders["VertexColor"], modelMatrix1);
	
	//Draw obstacle4 type asteroid1 - decagon
	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle4->translateX, obstacle4->translateY);
	modelMatrix1 *= Transform2D::Rotate(obstacle4->angularStep);
	RenderMesh2D(meshes["asteroid1"], shaders["VertexColor"], modelMatrix1);
	
	//Draw obstacle5 type asteroid2 - octagon
	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle5->translateX, obstacle5->translateY);
	modelMatrix1 *= Transform2D::Scale(obstacle5->scaleX, obstacle5->scaleY);
	RenderMesh2D(meshes["asteroid2"], shaders["VertexColor"], modelMatrix1);

	//Draw obstacle6 type asteroid2 - octagon
	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle6->translateX, obstacle6->translateY);
	modelMatrix1 *= Transform2D::Scale(obstacle6->scaleX , obstacle6->scaleY);
	RenderMesh2D(meshes["asteroid2"], shaders["VertexColor"], modelMatrix1);

	//Draw obstacle7 type asteroid1 - decagon
	modelMatrix1 = visMatrix * Transform2D::Translate(obstacle7->translateX, obstacle7->translateY);
	//modelMatrix1 *= Transform2D::Translate(obstacle7->translateX, obstacle7->translateY);
	RenderMesh2D(meshes["asteroid1"], shaders["VertexColor"], modelMatrix1);

	//Draw finish surface
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
}

void AstroKity::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	p->mintX = p->translateX;
	p->mintY = p->translateY;
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT) || IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		glm::vec3 point = TransformViewPoint(logicSpace, viewSpace, glm::vec3(mouseX, mouseY, 0));
		p->maxtX = float(point[0]);
		p->maxtY = float(point[1]);
		printf("mouseX=%i, mouseY=%i\n", mouseX, mouseY);
		printf("mouseXaf=%f, mouseYaf=%f\n", point[0], point[1]);
		launch = 1;
	}
}

void AstroKity::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void AstroKity::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}