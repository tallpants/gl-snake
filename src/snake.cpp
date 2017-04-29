#include <GL/glut.h>
#include <ctime>
#include <deque>
#include <unistd.h>

int main(int argc, char **argv) {

  glutInit(&argc, argv);

  glutInitWindowSize(600, 600);
  glutCreateWindow("Snake");

  glMatrixMode(GL_PROJECTION);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  

  glutMainLoop();

  return 0;
}