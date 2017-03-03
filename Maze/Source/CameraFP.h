/*
Camera algorithms by Joey de Vries
https://learnopengl.com/#!Getting-started/Camera
*/


#ifndef CAMERAFP_H
#define CAMERAFP_H

#include <GL\glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace camSettings
{
	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat SPEED = 0.1f;
	const GLfloat SENSITIVTY = 0.25f;
}

class CameraFP
{
public:
    CameraFP(glm::vec3 position = glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = camSettings::YAW, GLfloat pitch = camSettings::PITCH);
    glm::mat4 getViewMatrix();
	void keyboard(GLuint direction);
    void mouseMove(GLfloat xPos, GLfloat yPos);
	void mouseReset();
	void setCameraPos(glm::vec3 pos);
	void cameraFPEnable(bool state);
	bool isCameraFPEnabled();
	glm::vec3 getPosition();
	glm::vec3 getFront();

private:
	enum Movements	
	{
		FORWARD = 0,
		BACK = 1,
		LEFT = 2,
		RIGHT = 3
	};

    void update();

	bool enabled;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
	GLfloat lastX;
	GLfloat lastY;
    GLfloat yaw;
    GLfloat pitch;
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;
};
#endif

