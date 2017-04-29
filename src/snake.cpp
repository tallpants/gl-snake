#include <GL/glut.h>
#include <ctime>
#include <deque>
#include <unistd.h>

// Snake directions
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

char title[] = "Snake";
float map_half_length = 30.0;

// The current direction of the snake is stored here.
// Initial direction when the game starts is DOWN.
int direction = DOWN;

// For glutTimerFunc()
// The registered callback function will be triggered in
// intervals of the milliseconds specified here.
int timer = 100;

// Did the player change the direction of the snake?
bool changed_direction = false;

// Maintains the coordinates of the body of the snake
std::deque< std::deque<float> > part_coords;

// Is there food on the map right now?
bool food_available = false;

// X, Y coordinates of the food on the map
int food_coords[2];

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {

  glutInit(&argc, argv);

  glutInitWindowSize(600, 600);
  glutCreateWindow("Snake");

  glutDisplayFunc(display);

  glMatrixMode(GL_PROJECTION);
  glClearColor(1.0, 1.0, 1.0, 1.0);

  glutMainLoop();

  return 0;
}