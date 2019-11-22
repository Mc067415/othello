#include "scene.h"
#include <string>
#include <sstream>
#include <iomanip>
using std::string;
using std::stringstream;
using std::setprecision;

Scene scene;
float stepsize = 0.1; 
float camrotationamount = 0.01;
// usedd for mouse look around
int oldx,oldy,deltax,deltay;
float lookaroundsensetivity=0.0008;


extern "C" void display();
extern "C" void idleanimation();
extern "C" void special(int key, int x, int y);
extern "C" void keyboard(unsigned char key, int x, int y);
extern "C" void reshape(int width, int height);
extern "C" void lookaround(int x, int y);
extern "C" void updatemousexy(int x,int y);
// Simple animation
GLint lasttime=0;
extern "C" void idleanimation(){
  GLint time = glutGet(GLUT_ELAPSED_TIME);

  // cameras angle about the y axis how you would normally rotate a camera left and right
  stringstream cameraangleiny;
  cameraangleiny  << "CAY: " << setprecision(2) << scene.camera.get_cameraYangel();
  // camera angle about the x axis rotating the camera up and down
  stringstream cameraangleinx;
  cameraangleinx << "CAX: " <<  setprecision(2)  <<  scene.camera.get_cameraXangel();


  string newtitle = cameraangleiny.str() + "  " + cameraangleinx.str() ;
  glutSetWindowTitle(newtitle.c_str());

  lasttime=time;
  glutPostRedisplay();
}
extern "C" void special(int key, int x, int y){
  switch(key) {
  // case GLUT_KEY_UP:    scene.camera.turnup(camrotationamount); break;
  // case GLUT_KEY_DOWN:  scene.camera.turndown(camrotationamount);    break;
  // case GLUT_KEY_LEFT:  scene.camera.turnleft(camrotationamount); break;
  // case GLUT_KEY_RIGHT:  scene.camera.turnright(camrotationamount); break;
  
  
  case GLUT_KEY_UP:    scene.peicedevelopment->Translate(0,1,0);   break;  
  case GLUT_KEY_DOWN:  scene.peicedevelopment->Translate(0,-1,0);   break;   
  case GLUT_KEY_LEFT:  scene.peicedevelopment->Translate(-1,0,0);   break;   
  case GLUT_KEY_RIGHT: scene.peicedevelopment->Translate(1,0,0);   break;   

  }
}
extern "C" void keyboard(unsigned char key, int x, int y){
  switch(key) {
  case 033: // Escape Key
  case 'q': case 'Q':
    exit(EXIT_SUCCESS);
    break;

    // Speed up/slow down movements
  
  case '+': scene.incr*=2.0; break;
  case '-': scene.incr/=2.0; break;

  case 'z': scene.zNear  *= 1.1; scene.zFar /= 1.1; break;
  case 'Z': scene.zNear /= 1.1; scene.zFar *= 1.1;  break;
  
  // I believe i can fly 
  case 'Y': scene.camera.moveup(stepsize);   break;
  case 'y': scene.camera.movedown(stepsize); break;
  //


  // standard walking movement
  case 'w': scene.camera.moveforward(stepsize); break;
  case 's': scene.camera.moveback(stepsize);    break;
  case 'a': scene.camera.moveright(stepsize); break;
  case 'd': scene.camera.moveleft(stepsize);  break;


  case 'v': 
    scene.fovy-=5; 
    if (scene.fovy < 0) {
      // Min angle of view 1 degree
      scene.fovy = 1;
    }
    break;
  case 'V': scene.fovy+=5; break;
    if (scene.fovy > 179) {
      // Max angle of view 179 degrees
      scene.fovy = 179;
    }
    break;

  case ' ':  // reset values to their defaults

    scene.incr=0.1;

    break;
  }

  glutPostRedisplay();
}
extern "C" void reshape(int width, int height){
  glViewport(0, 0, width, height);

  scene.aspect = GLfloat(width)/height;
}
extern "C" void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //send Camera matrix to gpu
  mat4 cv = scene.camera.generate_view_matrix();
  glUniformMatrix4fv(scene.shader.cameraViewMatrix(), 1, GL_TRUE, cv);
  //send Prespective matrix to gpu
  mat4  p = Perspective(scene.fovy, scene.aspect, scene.zNear, scene.zFar) ;
  glUniformMatrix4fv(scene.shader.projectionMatrix(), 1, GL_TRUE, p);
  
  
  scene.peicedevelopment->draw();

  glutSwapBuffers();
}
extern "C" void lookaround(int x, int y){
  deltax = oldx-x;
  deltay = oldy-y;
  
  if(deltax <= 0){
    scene.camera.turnleft(deltax*lookaroundsensetivity);
  }else{
    scene.camera.turnright(-deltax*lookaroundsensetivity);
  }
  // Uncomment if you believe you can fly.
  if(deltay <= 0){
    scene.camera.turnup(deltay*lookaroundsensetivity);
  }else{
    scene.camera.turndown(-deltay*lookaroundsensetivity);
  }

  oldx = x;
  oldy = y;
}
extern "C" void updatemousexy(int x,int y){
  oldx = x;
  oldy = y;
}


int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Color Cube");

  glewInit();

  scene.init();
  
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutIdleFunc(idleanimation);
  glutReshapeFunc(reshape);
  glutMotionFunc(lookaround);
  glutPassiveMotionFunc(updatemousexy);

  glutMainLoop();
  return(EXIT_SUCCESS);
}
