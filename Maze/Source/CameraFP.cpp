#include "CameraFP.h"

CameraFP::CameraFP(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) 
	: front(glm::vec3(0.0f, 0.0f, -1.0f))
	, movementSpeed(camSettings::SPEED)
	, mouseSensitivity(camSettings::SENSITIVTY)
	, enabled(false)
{
   this->position = position;
   this->up = up;
   this->yaw = yaw;
   this->pitch = pitch;
   update();
}

glm::mat4 CameraFP::getViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void CameraFP::keyboard(GLuint direction)
{
    if (direction == FORWARD)
	{
		glm::vec3 temp = glm::normalize(glm::cross(front, up));
		position -= glm::normalize(glm::cross(temp, up)) * movementSpeed;
	}
    if (direction == BACK)
	{
		glm::vec3 temp = glm::normalize(glm::cross(front, up));
		position += glm::normalize(glm::cross(temp, up)) * movementSpeed;
	}
    if (direction == LEFT)
		position += glm::normalize(glm::cross(front, up)) * movementSpeed;
    if (direction == RIGHT)
		position -= glm::normalize(glm::cross(front, up)) * movementSpeed;
	
	position.y = 0.0f;
}

void CameraFP::mouseMove(GLfloat xPos, GLfloat yPos)
{

	if(xPos != lastX || yPos != lastY)
		mouseReset();

	GLfloat dx = xPos - lastX;
	GLfloat dy = lastY - yPos; 
	GLfloat sensitivity = 0.123;

	lastX = xPos;
	lastY = yPos;
	dx *= sensitivity;
	dy *= sensitivity;
	yaw += dx;
	pitch += dy;

    if (pitch > 89.99f)
		pitch = 89.99f;
    if (pitch < -89.99f)
		pitch = -89.99f;

    update();
}

void CameraFP::mouseReset()
{
	lastX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	lastY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
}

void CameraFP::setCameraPos(glm::vec3 pos)
{
	position = pos;
}

void CameraFP::cameraFPEnable(bool state)
{
	enabled = state;
}

bool CameraFP::isCameraFPEnabled()
{
	return enabled;
}

glm::vec3 CameraFP::getPosition()
{
	return position;
}

glm::vec3 CameraFP::getFront()
{
	return front;
}

void CameraFP::update()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
}