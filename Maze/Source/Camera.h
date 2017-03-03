/*
Author: Dr. William Thibault 
https://github.com/wthibault
*/


#ifndef CAMERA_H
#define CAMERA_H

#pragma once
#include <GL\glew.h>
#include "Trackball.h"
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera {
public:
 Camera() : trackballEnabled(false),
    trackball(320,240,240) ,
    distance(6),
    position(0,0,distance),
    lookat ( 0,0,0 ),
    up ( 0,1,0 ),
    fovy ( 65.0 ),
    wscreen ( 640 ), 
    hscreen ( 480 )
      { M = glm::translate ( glm::mat4(), glm::vec3(0,0,-distance) );}

  void enableTrackball(bool on) { trackballEnabled=on; }
  void startMouse ( float x, float y ) { trackball.startMouse(x,y); }
  void dragMouse ( float x, float y )  { trackball.dragMouse(x,y); };
  void setDistance ( float d ) { distance = d; }

  void setPosition ( glm::vec3 pos ) { position = pos; M = glm::lookAt(position,lookat,up); }
  glm::vec3 getPosition ( ) { return position; }

  void setLookat   ( glm::vec3 look ) { lookat = look; M = glm::lookAt(position,lookat,up); }
  glm::vec3 getLookat   ( ) { return lookat; }

	void updateOrigin(glm::vec3 center)
	{
		M = glm::translate(glm::mat4(),glm::vec3(0,0,-distance)) * trackball.getMat4();
		M = glm::translate(M, -center);
	}

	bool isTrackBallEnabled()
	{
		return trackballEnabled;
	}

  void setFov ( float afovy ) { 
    // you'd better call setupPerspective from the resize callback else aspect ratio probably wrong.
    // later, you can call setFov(fovy) to change the "lens"
    fovy = afovy; 
    setupPerspective ( wscreen, hscreen );
  }
  float getFov () { return fovy; }

  void setProjectionMatrix(glm::mat4 m) {P=m;}
  glm::mat4 getProjectionMatrix() {return P;}

  void setModelviewMatrix(glm::mat4 m)  {M=m;}
  glm::mat4 getModelviewMatrix()  {return M;}

  void setupPerspective ( int w, int h ) {
    hscreen = h;
    wscreen = w;
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    P = glm::perspective ( fovy, (GLfloat) w / (GLfloat) h, 0.1f, 200.0f );
    float halfw = float(w)/2.0f;
    float halfh = float(h)/2.0f;
    trackball = Trackball ( halfw, halfh, std::min ( halfw, halfw ) );
  }

protected:
  bool      trackballEnabled;
  Trackball trackball;
  float     distance;
  glm::vec3 position, lookat, up;
  float     fovy;
  int       wscreen, hscreen;
  glm::mat4 P;
  glm::mat4 M;

};
#endif

