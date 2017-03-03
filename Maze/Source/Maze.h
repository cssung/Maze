#ifndef MAZE_H
#define MAZE_H

#include <ostream>
#include<stack>
#include <vector>
#include "Angel.h"
#include <time.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace wallData
{
const GLfloat xyGreatest = 1.0f;
const GLfloat xyLeast = -1.0f;
const GLfloat zBack = -1.0f;
const GLfloat wallThickness = (xyGreatest - xyLeast) / 25.0f;
const GLfloat wallLength = xyGreatest - xyLeast - (2 * wallThickness);
const GLfloat zFront = zBack + wallThickness;
const GLfloat cellShift = (2 * xyGreatest) - (2 * wallThickness);
const GLfloat shift = wallLength + wallThickness;

/*
setting up the size and dimmensions of the maze walls
 format is (position.x, position.y, position.z, position.w, normal.x, normal.y, normal.z)
*/
const GLfloat verticies[] = 
{
		xyLeast,     xyLeast,    zFront, 1.0f, 0.0f, 0.0f, 1.0f, // front bottom left
		xyGreatest,  xyLeast,    zFront, 1.0f, 0.0f, 0.0f, 1.0f, // front bottom right
		xyLeast,     xyGreatest, zFront, 1.0f, 0.0f, 0.0f, 1.0f, // front top left
		xyGreatest,  xyGreatest, zFront, 1.0f, 0.0f, 0.0f, 1.0f, // front top right
		xyLeast,     xyGreatest, zFront, 1.0f, 0.0f, 0.0f, 1.0f, // front top left
		xyGreatest,  xyLeast,    zFront, 1.0f, 0.0f, 0.0f, 1.0f, // front bottom right

		xyGreatest,  xyLeast,    zBack,  1.0f, 0.0f, 0.0f, -1.0f, // back bottom right
		xyLeast,     xyLeast,    zBack,  1.0f, 0.0f, 0.0f, -1.0f,  // back bottom left
		xyLeast,     xyGreatest, zBack,  1.0f, 0.0f, 0.0f, -1.0f, // back top left
		xyLeast,     xyGreatest, zBack,  1.0f, 0.0f, 0.0f, -1.0f, // back top left
		xyGreatest,  xyGreatest, zBack,  1.0f, 0.0f, 0.0f, -1.0f,  // back top right
		xyGreatest,  xyLeast,    zBack,  1.0f, 0.0f, 0.0f, -1.0f, // back bottom right

		xyLeast,     xyGreatest, zFront, 1.0f, 0.0f, 1.0f, 0.0f, // front top left
		xyGreatest,  xyGreatest, zFront, 1.0f, 0.0f, 1.0f, 0.0f, // front top right
		xyLeast,     xyGreatest, zBack,  1.0f, 0.0f, 1.0f, 0.0f,// back top left
		xyGreatest,  xyGreatest, zBack,  1.0f, 0.0f, 1.0f, 0.0f, // back top right
		xyLeast,     xyGreatest, zBack,  1.0f, 0.0f, 1.0f, 0.0f,// back top left
		xyGreatest,  xyGreatest, zFront, 1.0f, 0.0f, 1.0f, 0.0f,// front top right

		xyGreatest,  xyLeast,    zFront, 1.0f, 0.0f, -1.0f, 0.0f,// front bottom right
		xyLeast,     xyLeast,    zFront, 1.0f, 0.0f, -1.0f, 0.0f,// front bottom left
		xyLeast,     xyLeast,    zBack,  1.0f, 0.0f, -1.0f, 0.0f,// back bottom left
		xyLeast,     xyLeast,    zBack,  1.0f, 0.0f, -1.0f, 0.0f,// back bottom left
		xyGreatest,  xyLeast,    zBack,  1.0f, 0.0f, -1.0f, 0.0f,// back bottom right
		xyGreatest,  xyLeast,    zFront, 1.0f, 0.0f, -1.0f, 0.0f,// front bottom right

		xyGreatest,  xyGreatest, zFront, 1.0f, 1.0f, 0.0f, 0.0f,// front top right
		xyGreatest,  xyLeast,    zFront, 1.0f, 1.0f, 0.0f, 0.0f,// front bottom right
		xyGreatest,  xyLeast,    zBack,  1.0f, 1.0f, 0.0f, 0.0f,// back bottom right
		xyGreatest,  xyGreatest, zBack,  1.0f, 1.0f, 0.0f, 0.0f, // back top right
		xyGreatest,  xyGreatest, zFront, 1.0f, 1.0f, 0.0f, 0.0f,// front top right
		xyGreatest,  xyLeast,    zBack,  1.0f, 1.0f, 0.0f, 0.0f,// back bottom right

		xyLeast,     xyLeast,    zFront, 1.0f, -1.0f, 0.0f, 0.0f,// front bottom left
		xyLeast,     xyGreatest, zFront, 1.0f, -1.0f, 0.0f, 0.0f,// front top left
		xyLeast,     xyLeast,    zBack,  1.0f, -1.0f, 0.0f, 0.0f,// back bottom left
		xyLeast,     xyGreatest, zFront, 1.0f, -1.0f, 0.0f, 0.0f,// front top left
		xyLeast,     xyGreatest, zBack,  1.0f, -1.0f, 0.0f, 0.0f,// back top left
		xyLeast,     xyLeast,    zBack,  1.0f, -1.0f, 0.0f, 0.0f,// back bottom left
	};
}

class Cell
{
public:
	bool top;
	bool bottom;
	bool left;		
	bool right;
	bool visited;
	int x;
	int y;

	Cell()
		: top(true)
		, bottom(true)
		, left(true)
		, right(true)
		, visited(false)
		, x(0)
		, y(0)
	{}
};

class Maze
{
public:
	Maze(int m, int n);
	~Maze();
	void draw();
	int getRows();
	int getCols();
	bool topWallUp(int xPos, int yPos);
	bool bottomWallUp(int xPos, int yPos);
	bool leftWallUp(int xPos, int yPos);
	bool rightWallUp(int xPos, int yPos);
	int getEnterencePosX();
	GLint getProgram();
	GLuint getVao();
	glm::vec3 getStartPosition();
	void setLightPos(glm::vec3 position);
	void setCameraPos(glm::vec3 position);
	void setView(glm::mat4 mat);
	void setProjection(glm::mat4 mat);

private:
	enum Dirrections
	{
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3
	};

    void generate();
	void glInit();
	void makeTransformationInstances();
	void makeEnterances();	
	int getRandomWall();
	bool canMoveUp(const Cell& position);
	bool canMoveDown(const Cell& position);
	bool canMoveLeft(const Cell& position);
	bool canMoveRight(const Cell& position);
	bool canMove(const Cell& position);
	Cell* moveToNextCell(const Cell& position);

	int collums;
	int rows;
	int enterancePos;
	Cell** grid;
	GLuint vbo;
	GLuint vao; 
	GLuint ebo;
	GLuint program;
	glm::vec3 lightPos;
	glm::vec3 cameraPos;
	glm::mat4 view;
	glm::mat4 projection;
	std::vector<glm::mat4> mats;
};
#endif

