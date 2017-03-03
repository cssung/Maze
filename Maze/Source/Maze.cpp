#include "Maze.h"

Maze::Maze(int m, int n)
	: rows(m)
	, collums(n)
{
	//setup the grid to represent the maze
	grid = new Cell*[m];
	for(int i = 0; i < m; i++)
		grid[i] = new Cell[n];
	
	//set maze cell indexs in the grid
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < collums; j++)
		{
			grid[i][j].x = i;
			grid[i][j].y = j;
		}
	}

	glInit();							//OpenGL setup
	generate();							//generate the maze's structure
	makeTransformationInstances();		//make two entereances, one at top, other at bottom
}


Maze::~Maze()
{
	for(int i = 0; i < rows; i++)
		delete [] grid[i];
	delete [] grid;
}

void Maze::draw()
{
	glUseProgram(program);
    glBindVertexArray(vao);

	//model view and projection uniform locations
	GLuint modelLoc = glGetUniformLocation(program, "model");
	GLuint viewLoc = glGetUniformLocation(program, "view");
	GLuint projectionLoc = glGetUniformLocation(program, "projection");
	//object light and camera uniform locations
	GLint lightLoc = glGetUniformLocation(program, "light");
	GLint cameraPosLoc = glGetUniformLocation(program, "cameraPos");
	//material uniform locations
	GLint matAmbientLoc = glGetUniformLocation(program, "material.ambient");
	GLint matDiffuseLoc = glGetUniformLocation(program, "material.diffuse");
	GLint matSpecularLoc = glGetUniformLocation(program, "material.specular");
	GLint matShineLoc = glGetUniformLocation(program, "material.shininess"); 
	//light uniform locations
	GLint lightAmbientLoc  = glGetUniformLocation(program, "light.ambient");
	GLint lightDiffuseLoc  = glGetUniformLocation(program, "light.diffuse");
	GLint lightSpecularLoc = glGetUniformLocation(program, "light.specular");
	GLint lightConstantLoc = glGetUniformLocation(program, "light.constant");
	GLint lightLinearLoc = glGetUniformLocation(program, "light.linear");
	GLint lightQuadraticLoc = glGetUniformLocation(program, "light.quadratic");

	//view and projection settings
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));	
	//set object light position and camera uniforms
	glUniform3f(lightLoc, lightPos.x, lightPos.y, lightPos.z);  
	glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
	//set material uniforms
	glUniform3f(matAmbientLoc, 0.24725f, 0.1995f, 0.0745f);
	glUniform3f(matDiffuseLoc, 0.75164f, 0.60648f, 0.22648);
	glUniform3f(matSpecularLoc, 0.628281f, 0.555802f, 0.366065f);
	glUniform1f(matShineLoc, 25.0f);
	//set light uniforms
	glUniform3f(lightAmbientLoc,  1.0f, 1.0f, 1.0f);
	glUniform3f(lightDiffuseLoc,  1.0f, 1.0f, 1.0f); 
	glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);  
	glUniform1f(lightConstantLoc, 1.0f);
	glUniform1f(lightLinearLoc, 0.001f);
	glUniform1f(lightQuadraticLoc, 0.002f);

	//repeatedly set transformed model matrix uniform by traversing the instanced matricies and draw the maze wall
	for(int i = 0; i < mats.size(); i++)
	{
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mats[i]));  
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glBindVertexArray(0);
}

int Maze::getRows()
{
	return rows;
}

int Maze::getCols()
{
	return collums;
}

bool Maze::topWallUp(int xPos, int yPos)
{
	return grid[xPos][yPos].top;
}

bool Maze::bottomWallUp(int xPos, int yPos)
{
	return grid[xPos][yPos].bottom;
}

bool Maze::leftWallUp(int xPos, int yPos)
{
	return grid[xPos][yPos].left;
}

bool Maze::rightWallUp(int xPos, int yPos)
{
	return grid[xPos][yPos].right;
}

int Maze::getEnterencePosX()
{
	return enterancePos;
}

GLint Maze::getProgram()
{
	return program;
}

GLuint Maze::getVao()
{
	return vao;
}

glm::vec3 Maze::getStartPosition()
{
	glm::vec3 startPosition(0.0f, 0.0f, 4.0f);
	startPosition.x -= (wallData::wallLength * (getEnterencePosX())); //set camera to face the open enterence at the top of the maze
	return startPosition;
}

//dfs traversal using stack to carve out maze paths
void Maze::generate()
{
    std::stack<Cell*> stk;
    Cell* currentCell = &grid[0][0];
    currentCell->visited = true;
    int visitedCells = 1;
	int numCells = rows * collums;
    while(visitedCells < numCells)
    {
        if(canMove(*currentCell))
        {
            stk.push(currentCell);
            currentCell = moveToNextCell(*currentCell);
            currentCell->visited = true;
            visitedCells++;
        }
        else
        {
            currentCell = stk.top();
            stk.pop();
        }
    }
	makeEnterances();
}

//setup shader programs, vao, vbo and send data to GPU
void Maze::glInit()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	program = InitShader("VertexShader.glsl", "FragShader.glsl");
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint normal = glGetAttribLocation(program, "normal");
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallData::verticies), wallData::verticies, GL_DYNAMIC_DRAW); 
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);	
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(normal);
	glBindVertexArray(0);
}

//setup instanced transformations for the maze walls based on the walls that are still up after the maze structure has been generated
void Maze::makeTransformationInstances()
{
	glm::mat4 model, trans, rotate, baseTrans;
	for(int z = 0; z < getRows(); z++)
	{
		for(int x = 0; x < getCols(); x++)
		{
			trans = glm::translate(baseTrans, glm::vec3(-(x * wallData::cellShift), 0.0f, -(z * wallData::cellShift)));
			rotate = glm::rotate(rotate, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			if(rightWallUp(z, x))
			{
				model = trans * rotate;
				mats.push_back(model);
			}

			rotate = glm::rotate(rotate, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			if(topWallUp(z, x))
			{
				model = trans * rotate;
				mats.push_back(model);
			}

			rotate = glm::rotate(rotate, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			if(leftWallUp(z, x))
			{
				model = trans * rotate;
				mats.push_back(model);
			}

			rotate = glm::rotate(rotate, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			if(bottomWallUp(z, x))
			{
				model = trans * rotate;
				mats.push_back(model);
			}
		}
	}
}

//we need to pick two random enterances, one at top, one at bottom
void Maze::makeEnterances()
{
	int topEnterance = rand() % rows;
	int bottomEnterance = rand() % rows;
	enterancePos = topEnterance; 

	grid[0][topEnterance].top = false;
	grid[rows - 1][bottomEnterance].bottom = false;
}

void Maze::setView(glm::mat4 mat)
{
	view = mat;
}

void Maze::setProjection(glm::mat4 mat)
{
	projection = mat;
}

void Maze::setLightPos(glm::vec3 position)
{
	lightPos = position;
}

void Maze::setCameraPos(glm::vec3 position)
{
	cameraPos = position;
}

int Maze::getRandomWall()
{
	return rand() % 4;
}

bool Maze::canMoveUp(const Cell& position)
{
	if(position.x == 0)
		return false;
	if(grid[position.x - 1][position.y].visited)
		return false;
	return true;
}

bool Maze::canMoveDown(const Cell& position)
{
	if(position.x == rows - 1)
		return false;
	if(grid[position.x + 1][position.y].visited)
		return false;
	return true;
}

bool Maze::canMoveLeft(const Cell& position)
{
	if(position.y == 0)
		return false;
	if(grid[position.x][position.y - 1].visited)
		return false;
	return true;
}

bool Maze::canMoveRight(const Cell& position)
{
	if(position.y == collums - 1)
		return false;
	if(grid[position.x][position.y + 1].visited)
		return false;
	return true;
}

bool Maze::canMove(const Cell& position)
{
	if(canMoveDown(position))
		return true;
	if(canMoveRight(position))
		return true;
	if(canMoveUp(position))
		return true;
	if(canMoveLeft(position))
		return true;
	return false;
}

//pick a random cell and try to move to it, destroying the respective walls
//cell walls overlap so you need to destroy two walls to make a path (top wall with bottom, left with right)
Cell* Maze::moveToNextCell(const Cell& position)
{
	int wall = getRandomWall();

	while(true)
	{	
		if(wall == UP && position.x != 0 && canMoveUp(position))
		{
			grid[position.x][position.y].top = false;
			grid[position.x - 1][position.y].bottom = false;
			return &grid[position.x - 1][position.y];
		}
		if(wall == DOWN && position.x != rows - 1 && canMoveDown(position))
		{
			grid[position.x][position.y].bottom = false;
			grid[position.x + 1][position.y].top = false;
			return &grid[position.x + 1][position.y];
		}
		if(wall == LEFT && position.y != 0 && canMoveLeft(position))
		{
			grid[position.x][position.y].left = false;
			grid[position.x][position.y - 1].right = false;
			return &grid[position.x][position.y - 1];
		}
		if(wall == RIGHT && position.y != collums - 1 && canMoveRight(position))
		{
			grid[position.x][position.y].right = false;
			grid[position.x][position.y + 1].left = false;
			return &grid[position.x][position.y + 1];
		}
		wall = wall == 4 ? 0 : ++wall;
	}
}
