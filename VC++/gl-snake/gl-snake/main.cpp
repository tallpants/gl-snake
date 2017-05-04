#include <GL\glut.h>
#include <ctime>

#define UP 1
#define RIGHT 2
#define DOWN -1
#define LEFT -2

// Storage container for coordinates of the blocks forming the
// snake's body and the food blocks
struct coordinate {
  int x;
  int y;
};

// Partial deque implementation that encapsulates the snake's body
class Snake {
  struct node {
    coordinate data;
    node* next;
    node* prev;
  };

  node* head;
  node* tail;

public:
  // Current size of the snake's body
  int size;

  // Default constructor for empty snake
  Snake() {
    size = 0;
    head = NULL;
    tail = NULL;
  }

  // Push a block onto the head of the snake
  void push_front(coordinate c) {
    // Is this the first block of the snake?
    if (size == 0) {
      head = new node;
      head->data.x = c.x;
      head->data.y = c.y;
      head->next = NULL;
      head->prev = NULL;
      tail = head;
      size++;
    } else {
      node* temp = new node;
      temp->data.x = c.x;
      temp->data.y = c.y;
      temp->next = head;
      temp->prev = NULL;
      head->prev = temp;
      head = temp;
      size++;
    }
  }

  // Pop the rearmost block off of the snake's body
  void pop_back() {
    tail = tail->prev;
    tail->next = NULL;
    size--;
  }

  // Access the blocks of the snake using the subscript operator
  coordinate operator [] (int index) {
    // The first block of the snake is pointed to by the head
    if (index == 0) {
      coordinate c = { head->data.x, head->data.y };
      return c;
    }

    // Traverse the structure till you get to the right node
    node* temp = head;
    int i = 0;
    while (i != index) {
      temp = temp->next;
      i++;
    }

    coordinate c = { temp->data.x, temp->data.y };
    return c;
  }
};

void display_callback();
void input_callback(int, int, int);
void reshape_callback(int, int);
void timer_callback(int);
int random(int, int);
void drawFood();
void drawSnake();

int rows = 40;
int columns = 40;
bool length_inc = false;
bool seedflag = false;
bool food = false;
bool game_over = false;
int direction = RIGHT;
int foodx, foody;
int posx[1000] = { 4, 3, 2, 1, 0, -1, -1 };
int posy[1000] = { 10, 10, 10, 10, 10, 10, 10 };
int length = 7;

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(600, 600);
  glutCreateWindow("Snake");
  glutDisplayFunc(display_callback);
  glutReshapeFunc(reshape_callback);
  glutSpecialFunc(input_callback);
  glutTimerFunc(100, timer_callback, 0);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glutMainLoop();
  return 0;
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

void reshape_callback(int w, int h) {
  glViewport(0, 0, (GLfloat) w, (GLfloat) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, columns, 0.0, rows, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void timer_callback(int) {
  glutPostRedisplay();
  glutTimerFunc(100, timer_callback, 0);
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

void drawSnake() {
  for (int i = length - 1; i > 0; i--) {
    posx[i] = posx[i - 1];
    posy[i] = posy[i - 1];
  }

  for (int i = 0; i < length; i++) {
    glColor3f(0.0, 1.0, 0.0);
    if (i == 0) {
      glColor3f(0.0, 0.0, 1.0);
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

      if (posx[i] == 0 || posx[i] == columns - 1 || posy[i] == 0 || posy[i] == rows - 1) {
        game_over = true;
      } else if (posx[i] == foodx && posy[i] == foody) {
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

void drawFood() {
  if (!food) {
    foodx = random(2, columns - 2);
    foody = random(2, rows - 2);
    food = true;
  }

  glBegin(GL_QUADS);
  glVertex2d(foodx, foody);
  glVertex2d(foodx + 1, foody);
  glVertex2d(foodx + 1, foody + 1);
  glVertex2d(foodx, foody + 1);
  glEnd();
}

int random(int min, int max) {
  if (!seedflag) {
    srand(time(NULL));
    seedflag = true;
  } else {
    seedflag = false;
  }

  return min + rand() % (max - min);
}