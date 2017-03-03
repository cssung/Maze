#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Angel.h"
#include <time.h>
#include "Camera.h"
#include "CameraFP.h"
#include "Maze.h"


#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#endif

const GLfloat lamp2[] =
{
	-0.5f, -0.5f,  0.5f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f,
	-0.5f,  0.5f,  0.5f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f,
	-0.5f,  0.5f, -0.5f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f
};

const GLuint elements2[] = 
{
	0, 1, 2, 2, 1, 3,	// front
	5, 4, 6, 5, 6, 7,	// back
	0, 2, 4, 4, 2, 6,	// left
	3, 1, 5, 3, 5, 7,	// right
	2, 3, 6, 6, 3, 7,	// top
	1, 0, 4, 1, 4, 5	// bottom
};

GLuint lampVao, lampVbo, lampEbo, lampProgram;

Maze* maze;
GLint width, height;
GLboolean keysPressed[4];
Camera camera;
CameraFP cameraFP;

void keyboard()
{
	if(cameraFP.isCameraFPEnabled()) //only allow first person movement if the first person camera is enabled
	{
		if(keysPressed[0])
			cameraFP.keyboard(0);
		if(keysPressed[1])
			cameraFP.keyboard(1);	
		if(keysPressed[2])
			cameraFP.keyboard(2);
		if(keysPressed[3])
			cameraFP.keyboard(3);
	}
}

//swap between first person and trackball camera
void swapCmaeras()
{
	if(camera.isTrackBallEnabled())
	{
		camera.enableTrackball(false);
		cameraFP.cameraFPEnable(true);
		glutSetCursor(GLUT_CURSOR_NONE); 
	}
	else
	{
		camera.enableTrackball(true);
		camera.updateOrigin(cameraFP.getPosition());
		cameraFP.cameraFPEnable(false);
		glutSetCursor(GLUT_CURSOR_INHERIT); 
	}
}

void windowEnter()
{
	if(!camera.isTrackBallEnabled())
	{
		cameraFP.cameraFPEnable(true);
		glutSetCursor(GLUT_CURSOR_NONE); 
		cameraFP.mouseReset();
	}
}

void windowLeave()
{
	cameraFP.cameraFPEnable(false);
	glutSetCursor(GLUT_CURSOR_INHERIT); 
}

void test(int state)
{
	if(state == GLUT_LEFT)
		windowLeave();
	else
		windowEnter();
}

void mouseClick(int button, int state, int x, int y)
{
	windowEnter();
	if (camera.isTrackBallEnabled() && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) 
	{
		camera.startMouse ( x, height-y );
		camera.updateOrigin(cameraFP.getPosition());
	}
}

void motion (int x, int y)
{
	if(camera.isTrackBallEnabled())
	{
		camera.dragMouse(x,height-y);
		camera.updateOrigin(cameraFP.getPosition());
	}
}

void mouseMove(int xPos, int yPos)
{
	if(cameraFP.isCameraFPEnabled())
		cameraFP.mouseMove(xPos, yPos);
}  

void init(int m, int n) 
{
	lampProgram = InitShader("LampVertexShader.glsl", "LampFragShader.glsl");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glutSetOption(GLUT_MULTISAMPLE, 4);
	srand(time(NULL));
	maze =  new Maze(m, n);

	glm::vec3 startPosition = maze->getStartPosition();
	camera.enableTrackball(false);
	cameraFP.cameraFPEnable(false);
	cameraFP.setCameraPos(startPosition);

	glGenVertexArrays(1, &lampVao);
	glGenBuffers(1, &lampVbo);
	glGenBuffers(1, &lampEbo);
	glBindVertexArray(lampVao);
	glBindBuffer(GL_ARRAY_BUFFER, lampVbo);
	GLuint vPosition = glGetAttribLocation(lampProgram, "vPosition");
	glBufferData(GL_ARRAY_BUFFER, sizeof(lamp2), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lamp2), lamp2);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lampEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements2), elements2, GL_STATIC_DRAW);
	
	glBindVertexArray(0);
}

void display(void)
{
	keyboard();
	glClearColor(0.2f, 0.2f, 0.2f, 0.1f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glm::mat4 lightPosMat = glm::translate(glm::mat4(), glm::vec3(0.0f, 2.5f, 0.0f));
	glm::vec4 lightPos = lightPosMat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 view = camera.isTrackBallEnabled() ? camera.getModelviewMatrix() : cameraFP.getViewMatrix();	//get view matrix from either hte first person camera or the trackball camera depending on what's active
	glm::vec3 cameraPos = camera.isTrackBallEnabled() ? camera.getPosition() : cameraFP.getPosition();
	glm::mat4 projection = glm::perspective(65.0f, (float)width / (float)height, 0.1f, 1000.0f);
	
	maze->setView(view);				//have to send the view matrix from the camera to the maze so it can use it to draw the maze
	maze->setProjection(projection);	//have to send the projection matrix to the maze so it can use it to draw teh amze
	maze->setCameraPos(cameraPos);	
	maze->setLightPos(glm::vec3(lightPos.x, lightPos.y, lightPos.z));
	maze->draw();

	glUseProgram(lampProgram);
	glBindVertexArray(lampVao);
	GLuint modelLoc = glGetUniformLocation(lampProgram, "model");
	GLuint viewLoc = glGetUniformLocation(lampProgram, "view");
	GLuint projectionLoc = glGetUniformLocation(lampProgram, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lightPosMat));  
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

void reshape (int w, int h)
{
	width = w;
	height = h;
	glViewport (0, 0, (GLsizei)w, (GLsizei)h); 
}

void timer(int delay)
{
	glutPostRedisplay();
	glutTimerFunc (delay, timer, delay);
}

void keyPressed(unsigned char key, int x, int y)
{
	windowEnter();
	if(key == 119)
		keysPressed[0] = true;
	if(key == 115)
		keysPressed[1] = true;
	if(key == 100)
		keysPressed[2] = true;
	if(key == 97)
		keysPressed[3] = true;
	if(key == 32)
		swapCmaeras();
	if (key == 27)
		exit(0);
}

void keyUp (unsigned char key, int x, int y) 
{  
	windowEnter();
	if(key == 119)
		keysPressed[0] = false;
	if(key == 115)
		keysPressed[1] = false;
	if(key == 100)
		keysPressed[2] = false;
	if(key == 97)
		keysPressed[3] = false;
}  

int main(int argc, char** argv)
{
	int m, n;

	std::cout << "NOTE: the console takes fcuss off of the GLUT window, click on the GLUT window,"
		      << "to bring focus to the GLUT window or elase you won't be able to use input.\n\n"
			  << "This program generates an m x n maze.\n\n"
			  << "Controls:\n"
			  << "w: move foward\n"
			  << "s: move backward\n"
			  << "d: strafe right\n"
			  << "a: strafe left\n"
			  << "space: swap cameras\n"
			  << "move mouse to look around\n\n"
			  << "Enter a value for m: ";
	std::cin >> m;
	std::cout << "Enter a value for n: ";
	std::cin >> n;

	glewExperimental = GL_TRUE;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE );
	glutInitWindowSize(640, 480); 
	glutInitWindowPosition(500, 100);
	glutCreateWindow (argv[0]);
	glutSetWindow(1);

	#ifndef __APPLE__  
	glewInit();
	#endif

	init(m, n);
	glutDisplayFunc(display);
	glutPassiveMotionFunc(mouseMove);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressed);  
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(30, timer, 30);
	glutMouseFunc(mouseClick);
	glutMotionFunc(motion);
	glutEntryFunc(test);
	glutMainLoop();
	return 0;
}
