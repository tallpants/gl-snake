#include <deque>
#include <GL/glut.h>
#include <stdlib.h>

// Snake movement direction
#define UP      1
#define DOWN    2
#define LEFT    3
#define RIGHT   4

int map_half_length = 30;

// Current movement direction of the snake.
// Initial direction when the game starts is DOWN
int direction = DOWN;

// For glutTimerFunc()
// The callback registered will be triggered at intervals defined by
// the milliseconds given here.
int move_speed = 100;

// Did the player move the snake this iteration?
bool moved = false;

// Coordinates of the snake's body
// part_coords[i][0] => X coordinate of the i'th part of the snake's body
// part_coords[i][1] => Y coordinate of the i'th part of the snake's body
// TODO: Do we really need a queue of queues? Can we use queue of array[2] instead?
std::deque< std::deque<int> > part_coords;

// Initial size of the snake (number of blocks)
int initial_size = 3;

// Is there food on the map?
bool food_available = false;

// Coordinates of the food on the map
int food_coords[2];

// TODO: Document these variables
// TODO: Do we really need both?
int growth_stage = 0;
int growth = 2;

/**
 * Return a random coordinate within the bounds of the map
 */
int randomCoordinate() {
  return 2 * (rand() % (map_half_length + 1)) - map_half_length;
}

/**
 * Generates coordinates for food on the screen, sets the food_available
 * variable to true, and draws the food on the screen
 */
void spawnFood() {
  
  // Is there food on the map right now?
  if (!food_available) {

    // Keep generating coordinates until you get one that doesn't collide
    // with the body of the snake. Then set that as the real food coordinates.
    while (true) {   
      bool collides = false;

      // Generate temporary coordinates
      int temp_coords[2] = {randomCoordinate(), randomCoordinate()};

      // Do the temp coords collide with the snake's body?
      for (int i = 0; i < part_coords.size(); i++) {
        if (temp_coords[0] == part_coords[i][0] && temp_coords[1] == part_coords[i][1]) {
          collides = true;
        }
      }

      // If it doesn't collide then set it as the actual food coordinates
      if (!collides) {
        food_coords[0] = temp_coords[0];
        food_coords[1] = temp_coords[1];

        food_available = true;
        break;
      }
    }
  }

  // Drawing the food on the screen at the given coordinates
  glLoadIdentity();
  glTranslated(food_coords[0], food_coords[1], -40);
  glColor3d(1, 0, 0);

  glBegin(GL_POLYGON);
  glVertex2d(1, 1);
  glVertex2d(1, -1);
  glVertex2d(-1, -1);
  glVertex2d(-1, 1);
  glEnd();
}

/**
 * Draw the snake and the food on the buffer
 */
void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  // Iterate over the snake and draw each part at the correct coordinates
  for (int i = 0; i < part_coords.size(); i++) {
    glLoadIdentity();
    glTranslated(part_coords[i][0], part_coords[i][1], -40);
    glColor3d(0, 0, 0);

    glBegin(GL_POLYGON);
    glVertex2d(1, 1);
    glVertex2d(1, -1);
    glVertex2d(-1, -1);
    glVertex2d(-1, 1);
    glEnd();
  }

  // Set the food position in the process state, and draw it on the buffer
  spawnFood();

  // Swap buffers to display what we've drawn so far on the screen.
  // (Double buffering)
  glutSwapBuffers();
}

/**
 * Handles moving the snake.
 *
 * Checks for collisions with the walls or the snake itself, growing the snake
 * when food is eater, and updating the coordinates of the snake's front and
 * rear blocks.
 */
void moveSnake(int new_direction) {

  // Set the current direction to the new direction specified
  direction = new_direction;

  // Coordinate of the head of the snake
  int last_part = part_coords.size() - 1;

  // The new head of the snake after this iteration
  std::deque<int> new_head = part_coords[last_part];

  // Did the snake go up?
  if (direction == UP) {
    // Did the snake hit itself?
    for (int i = 0; i < part_coords.size(); i++) {
      if (part_coords[0][0] == part_coords[i][0] && part_coords[0][1] + 2 == part_coords[i][1]) {
        exit(0);
      }
    }

    // Did the snake hit a wall?
    if (part_coords[0][1] == map_half_length) {
      exit(0);
    }

    // Did the snake eat food?
    if (part_coords[0][0] == food_coords[0] && part_coords[0][1] + 2 == food_coords[1]) {
      growth_stage++;
      food_available = false;
    }

    // Update the snake's position
    new_head[1] = part_coords[0][1] + 2;
  }

  // Did the snake go down?
  else if (direction == DOWN) {
    // Did the snake hit itself?
    for (int i = 0; i < part_coords.size(); i++) {
      if (part_coords[0][0] == part_coords[i][0] && part_coords[0][1] - 2 == part_coords[i][1]) {
        exit(0);
      }
    }

    // Did the snake hit a wall?
    if (part_coords[0][1] == -map_half_length) {
      exit(0);
    }

    // Did the snake eat food?
    if (part_coords[0][0] == food_coords[0] && part_coords[0][1] - 2 == food_coords[1]) {
      growth_stage++;
      food_available = false;
    }

    // Update the snake's position
    new_head[1] = part_coords[0][1] - 2;
  } 
  
  // Snake didn't move up or down, so the Y coordinates of the snake's new position
  // remain the same.
  else {
    new_head[1] = part_coords[0][1];
  }

  // Did the snake go left?
  if (direction == LEFT) {
    // Did the snake hit itself?
    for (int i = 0; i < part_coords.size(); i++) {
      if (part_coords[0][0] - 2 == part_coords[i][0] && part_coords[0][1] == part_coords[i][1]) {
        exit(0);
      }
    }

    // Did the snake hit a wall?
    if (part_coords[0][0] == -map_half_length) {
      exit(0);
    }

    // Did the snake eat food?
    if (part_coords[0][0] - 2 == food_coords[0] && part_coords[0][1] == food_coords[1]) {
      growth_stage++;
      food_available = false;
    }

    // Update the snake's position
    new_head[0] = part_coords[0][0] - 2;
  }

  // Did the snake go right?
  else if (direction == RIGHT) {
    // Did the snake hit itself?
    for (int i = 0; i < part_coords.size(); i++) {
      if (part_coords[0][0] + 2 == part_coords[i][0] && part_coords[0][1] == part_coords[i][1]) {
        exit(0);
      }
    }

    // Did the snake hit a wall?
    if (part_coords[0][0] == map_half_length) {
      exit(0);
    }

    // Did the snake eat food?
    if (part_coords[0][0] + 2 == food_coords[0] && part_coords[0][1] == food_coords[1]) {
      growth_stage++;
      food_available = false;
    }

    // Update the snake's position
    new_head[0] = part_coords[0][0] + 2;
  }

  // The snake didn't go left or right, so the X coordinates of the snake's new
  // position remain the same  
  else {
    new_head[0] = part_coords[0][0];
  }

  // Push the newly calculated head block to the front of the snake
  part_coords.push_front(new_head);

  // Pop the last block if the snake didn't grow to "move" the snake
  if (!growth_stage) {
    part_coords.pop_back();
  } else if (growth_stage == growth) {
    growth_stage = 0;
  } else {
    growth_stage++;
  }

  // Marks the current window as needing to be redisplayed in the next
  // iteration of glutMainLoop.
  glutPostRedisplay();
}

/**
 * Checks key events, and if it is a valid movement then sets the moved
 * variable to true and calls the moveSnake() function with the
 * appropriate argument
 */
void keyboard(int key, int x, int y) {

  switch(key) {
    case GLUT_KEY_UP:
      // You can only move UP if you were already moving LEFT or RIGHT
      if (direction == LEFT || direction == RIGHT) {
        moved = true;
        moveSnake(UP);
      }
      break;

    case GLUT_KEY_DOWN:
      // You can only move DOWN if you were already moving LEFT or RIGHT
      if (direction == LEFT || direction == RIGHT) {
        moved = true;
        moveSnake(DOWN);
      }
      break;

    case GLUT_KEY_LEFT:
      // You can only LEFT if you were already moving UP or DOWN
      if (direction == UP || direction == DOWN) {
        moved = true;
        moveSnake(LEFT);
      }
      break;

    case GLUT_KEY_RIGHT:
      // You can only move RIGHT if you were already moving UP or DOWN
      if (direction == UP || direction == DOWN) {
        moved = true;
        moveSnake(RIGHT);
      }
      break;
  }

  // Marks the current window as needing to be redisplayed in the next
  // iteration of glutMainLoop.
  glutPostRedisplay();
}

void moveSnakeAuto(int value) {
  if (!moved) {

    switch(direction) {
      case UP: 
        moveSnake(UP);
        break;
      
      case DOWN:
        moveSnake(DOWN);
        break;
     
      case LEFT: 
        moveSnake(LEFT);
        break;
      
      case RIGHT: 
        moveSnake(RIGHT);
        break;
    }
  }
  
  else {
    moved = false;
  }

  glutTimerFunc(move_speed, moveSnakeAuto, 0);
}

int main(int argc, char **argv) {

  glutInit(&argc, argv);

  glutInitWindowSize(600, 600);
  glutCreateWindow("Snake");

  glutDisplayFunc(display);
  glutSpecialFunc(keyboard);
  glutTimerFunc(move_speed, moveSnakeAuto, 0);

  for (int i = 1; i <= initial_size; i++) {
    std::deque<int> row;

    row.push_back(0);
    row.push_back((map_half_length + 2 + (initial_size * 2)) - (i * 2));

    part_coords.push_front(row);
  }

  glMatrixMode(GL_PROJECTION);
  gluPerspective(75, 1, 0, 35);
  glClearColor(1, 1, 1, 1);
  glutMainLoop();

  return 0;
}