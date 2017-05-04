#include <GL\glut.h>

#define UP 1
#define RIGHT 2
#define DOWN -1
#define LEFT -2

int rows = 40;

int columns = 40;

bool length_inc = false;

bool food = false;

bool game_over = false;

int direction = RIGHT;

int foodx;
int foody;

int posx[1000] = { 4, 3, 2, 1, 0 };
int posy[1000] = { 10, 10, 10, 10, 10 };

int length = 7;

int random(int min, int max) {
  return min + rand() % (max - min);
}

void drawFood() {

  if (!food) {

    bool collides = false;

    int tempfoodx;
    int tempfoody;

    while (true) {
      tempfoodx = random(2, columns - 2);
      tempfoody = random(2, rows - 2);

      for (int i = 0; i < length; i++) {
        if (posx[i] == tempfoodx && posy[i] == tempfoody) {
          collides = true;
        }
      }

      if (!collides) {
        foodx = tempfoodx;
        foody = tempfoody;
        food = true;
        break;
      }
    }
  }

  glColor3f(1, 0, 0);

  glBegin(GL_QUADS);
    glVertex2d(foodx, foody);
    glVertex2d(foodx + 1, foody);
    glVertex2d(foodx + 1, foody + 1);
    glVertex2d(foodx, foody + 1);
  glEnd();
}

void drawSnake() {

  for (int i = length - 1; i > 0; i--) {
    posx[i] = posx[i - 1];
    posy[i] = posy[i - 1];
  }

  glColor3f(0.0, 0.0, 0.0);

  for (int i = 0; i < length; i++) {

    if (i == 0) {
      switch (direction) {
        case UP:
          posy[i]++;
          break;

        case DOWN:
          posy[i]--;
          break;

        case RIGHT:
          posx[i]++;
          break;

        case LEFT:
          posx[i]--;
          break;
      }

      if (posx[i] == 0 || posx[i] == columns - 1) {
        game_over = true;
      }

      if (posy[i] == 0 || posy[i] == rows - 1) {
        game_over = true;
      }

      else if (posx[i] == foodx && posy[i] == foody) {
        food = false;
        length_inc = true;
      }

      for (int j = 1; j < length; j++) {
        if (posx[j] == posx[0] && posy[j] == posy[0])
          game_over = true;
      }
    }

    glBegin(GL_QUADS);
      glVertex2d(posx[i], posy[i]);
      glVertex2d(posx[i] + 1, posy[i]);
      glVertex2d(posx[i] + 1, posy[i] + 1);
      glVertex2d(posx[i], posy[i] + 1);
    glEnd();
  }

  if (length_inc) {
    length++;
    length_inc = false;
  }
}

void display_callback() {

  if (game_over) {
    exit(0);
  }

  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  drawFood();
  drawSnake();

  glutSwapBuffers();
}

void input_callback(int key, int x, int y) {

  switch (key) {
    case GLUT_KEY_UP:
      if (direction != DOWN)
        direction = UP;
      break;

    case GLUT_KEY_DOWN:
      if (direction != UP)
        direction = DOWN;
      break;

    case GLUT_KEY_RIGHT:
      if (direction != LEFT)
        direction = RIGHT;
      break;

    case GLUT_KEY_LEFT:
      if (direction != RIGHT)
        direction = LEFT;
      break;
  }
}

void timer_callback(int) {
  glutPostRedisplay();
  glutTimerFunc(100, timer_callback, 0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  glutInitWindowPosition(10, 10);
  glutInitWindowSize(600, 600);
  glutCreateWindow("Snake");

  glViewport(0, 0, 600, 600);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0.0, columns, 0.0, rows, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glutDisplayFunc(display_callback);
  glutSpecialFunc(input_callback);
  glutTimerFunc(100, timer_callback, 0);

  glClearColor(1, 1, 1, 1);

  glutMainLoop();

  return 0;
}