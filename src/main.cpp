#include <GL/glut.h>
#include <iostream>

// Movement direction macros
#define UP 1
#define RIGHT 2
#define DOWN -1
#define LEFT -2

// Number of rows and columns forming the map
int rows = 40;
int columns = 40;

// Will the snake grow this iteration?
bool length_inc = false;

// Is there food on the map this iteration?
bool food = false;

// Is the game over?
bool game_over = false;

// Initial movement direction for the snake
int direction = RIGHT;

// The coordinates of the food on the map
int foodx;
int foody;

// X and Y coordinates of the blocks forming the snake's body
int posx[1000] = {4, 3, 2, 1, 0};
int posy[1000] = {10, 10, 10, 10, 10};

// Initial length of the snake
int length = 5;

// Returns a random number between the specified range
int random(int min, int max)
{
  return min + rand() % (max - min);
}

// If there is no food on the screen, generates coordinates
// that don't collide with the snake, sets the food variable
// to true, and draws the food on the screen
void drawFood()
{

  // Only calculate new coordinates for food if there isn't
  // already food on the map. If there is, then just draw it
  if (!food)
  {

    bool collides = false;

    // Temporary coordinates for the food
    int tempfoodx;
    int tempfoody;

    // Keep generating temporary coordinates until
    // you find one that doesn't collide with the snake
    while (true)
    {
      tempfoodx = random(2, columns - 2);
      tempfoody = random(2, rows - 2);

      // Does any part of the snake's body have the same
      // coordinates as the food generated?
      for (int i = 0; i < length; i++)
      {
        if (posx[i] == tempfoodx && posy[i] == tempfoody)
        {
          collides = true;
        }
      }

      // If the temporary coordinates don't collide with the snake
      // set them to be the actual food coordinates
      if (!collides)
      {
        foodx = tempfoodx;
        foody = tempfoody;
        food = true;
        break;
      }
    }
  }

  // Red colored food
  glColor3f(1, 0, 0);

  // Draw the food square
  glBegin(GL_QUADS);
    glVertex2d(foodx, foody);
    glVertex2d(foodx + 1, foody);
    glVertex2d(foodx + 1, foody + 1);
    glVertex2d(foodx, foody + 1);
  glEnd();
}


// Calculate new position after movement, check if the snake is hitting itself or hitting the
// wall, check if the snake is eating food and growing, set the new length
// and draw the body of the snake.
void drawSnake()
{

  // New position of each block will be the old position
  // of the previous block (all blocks in the snake will follow
  // the same path made by the first block.)
  for (int i = length - 1; i > 0; i--)
  {
    posx[i] = posx[i - 1];
    posy[i] = posy[i - 1];
  }

  // Calculate the new position of the head block of the snake
  // depending on the current movement direction
  for (int i = 0; i < length; i++)
  {

    if (i == 0)
    {
      switch (direction)
      {
        // If direction is UP, Y coordinate of the block increments.
        case UP:
          posy[i]++;
          break;
        
        // If direction is DOWN, Y coordinate of the block decrements.
        case DOWN:
          posy[i]--;
          break;

        // If direction is RIGHT, X coordinate of the block increments
        case RIGHT:
          posx[i]++;
          break;

        // If direction is DOWN, X coordinate of the block decrements.
        case LEFT:
          posx[i]--;
          break;
      }

      // Is the snake hitting the the left or right wall?
      if (posx[i] == 0 || posx[i] == columns - 1)
      {
        game_over = true;
      }

      // Is the snake hitting the top or bottom wall?
      if (posy[i] == 0 || posy[i] == rows - 1)
      {
        game_over = true;
      }

      // Is the snake eating the food?
      else if (posx[i] == foodx && posy[i] == foody)
      {
        food = false;

        // Snake's length will get incremented this turn.
        length_inc = true;
      }

      // Is the snake eating itself? 
      for (int j = 1; j < length; j++)
      {
        if (posx[j] == posx[0] && posy[j] == posy[0])
          game_over = true;
      }
    }

    // Draw the snake in black
    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_QUADS);
      glVertex2d(posx[i], posy[i]);
      glVertex2d(posx[i] + 1, posy[i]);
      glVertex2d(posx[i] + 1, posy[i] + 1);
      glVertex2d(posx[i], posy[i] + 1);
    glEnd();
  }

  // Snake getting incremented
  if (length_inc)
  {
    length++;
    length_inc = false;
  }
}


// Closes the program if the game is over.
// Else draws the food and moves the snake
void display_callback()
{

  // Quit if the game is over.
  if (game_over)
  {
    exit(0);
  }

  // Clearing color buffer for drawFood() and drawSnake()
  glClear(GL_COLOR_BUFFER_BIT);

  // Generate coordinates for food if required and draw the food
  drawFood();

  // Move the snake, calculate if it hits the wall or hits itself
  // or if the snake ate food and grew, then draw the snake
  drawSnake();

  // Swap buffers after drawing (double buffering)
  glutSwapBuffers();
}

// Changes direction of the snake (if allowed) on keyboard input
void input_callback(int key, int x, int y)
{

  switch (key)
  {
    // Up arrow key was pressed
    case GLUT_KEY_UP:
      // You can't change direction to UP if you were moving DOWN
      if (direction != DOWN)
        direction = UP;
      break;

    // Down arrow key was pressed
    case GLUT_KEY_DOWN:
      // You can't change direction to DOWN if you were moving UP
      if (direction != UP)
        direction = DOWN;
      break;

    // Right arrow key was pressed
    case GLUT_KEY_RIGHT:
      // You can't change direction to RIGHT if you were moving LEFT
      if (direction != LEFT)
        direction = RIGHT;
      break;

    // Left arrow key was pressed
    case GLUT_KEY_LEFT:
      // You can't change direction to LEFT if you were moving RIGHT
      if (direction != RIGHT)
        direction = LEFT;
      break;
  }
}

// Redisplay the window every 100ms
void timer_callback(int)
{
  // Mark the current window as needing to be redisplayed
  glutPostRedisplay();

  // Repeat every 100ms
  glutTimerFunc(100, timer_callback, 0);
}

int main(int argc, char **argv)
{
  // Initialize the window and the display mode (double buffering)
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Create a 600x600 window with the title "Snake"
  glutInitWindowSize(600, 600);
  glutCreateWindow("Snake");

  // Viewport to match the window's dimensions
  glViewport(0, 0, 600, 600);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Coordinate system of columns x rows with origin (0,0)
  // as the bottom left corner of the window and the point
  // (columns - 1, rows - 1) as the upper right corner.
  glOrtho(0.0, columns, 0.0, rows, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Register display function and input function
  glutDisplayFunc(display_callback);
  glutSpecialFunc(input_callback);

  // The main game loop will trigger every 100ms
  glutTimerFunc(100, timer_callback, 0);

  // White background
  glClearColor(1, 1, 1, 1);

  // Start the main loop
  glutMainLoop();
  return 0;
}