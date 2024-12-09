#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>  // For sleep()

#define WIDTH 20
#define HEIGHT 20
#define MAX_SNAKE_SIZE 100

// Direction Enum
typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

// Structure to define a position (x, y)
typedef struct {
    int x, y;
} Position;

// Snake Structure
typedef struct {
    Position body[MAX_SNAKE_SIZE];
    int size;
    Direction dir;
} Snake;

// Food Structure
typedef struct {
    Position pos;
} Food;

void gotoxy(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void draw(Snake* snake, Food* food) {
    system("cls");  // Clears the console
    // Draw the boundary
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1) {
                printf("#");
            } else if (j == 0 || j == WIDTH - 1) {
                printf("#");
            } else if (i == food->pos.y && j == food->pos.x) {
                printf("*");  // Draw food
            } else {
                int snake_part = 0;
                for (int k = 0; k < snake->size; k++) {
                    if (snake->body[k].x == j && snake->body[k].y == i) {
                        printf("O");  // Draw snake body
                        snake_part = 1;
                        break;
                    }
                }
                if (!snake_part) printf(" ");
            }
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");
}

int collision(Snake* snake) {
    // Check if snake hits the wall
    if (snake->body[0].x == 0 || snake->body[0].x == WIDTH - 1 ||
        snake->body[0].y == 0 || snake->body[0].y == HEIGHT - 1) {
        return 1;
    }

    // Check if snake collides with itself
    for (int i = 1; i < snake->size; i++) {
        if (snake->body[0].x == snake->body[i].x && snake->body[0].y == snake->body[i].y) {
            return 1;
        }
    }
    return 0;
}

void move_snake(Snake* snake) {
    // Move the snake's body
    for (int i = snake->size - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }

    // Move the snake's head
    switch (snake->dir) {
        case UP: snake->body[0].y--; break;
        case DOWN: snake->body[0].y++; break;
        case LEFT: snake->body[0].x--; break;
        case RIGHT: snake->body[0].x++; break;
    }
}

void generate_food(Food* food, Snake* snake) {
    int food_x, food_y;
    int collide;
    do {
        collide = 0;
        food_x = rand() % (WIDTH - 2) + 1;
        food_y = rand() % (HEIGHT - 2) + 1;
        // Check if the food is on the snake's body
        for (int i = 0; i < snake->size; i++) {
            if (snake->body[i].x == food_x && snake->body[i].y == food_y) {
                collide = 1;
                break;
            }
        }
    } while (collide);
    food->pos.x = food_x;
    food->pos.y = food_y;
}

void update(Snake* snake, Food* food) {
    move_snake(snake);
    if (snake->body[0].x == food->pos.x && snake->body[0].y == food->pos.y) {
        snake->size++;  // Snake grows when it eats food
        generate_food(food, snake);
    }
}

void input(Snake* snake) {
    if (_kbhit()) {
        char ch = _getch();
        switch (ch) {
            case 'w': case 'W': if (snake->dir != DOWN) snake->dir = UP; break;
            case 's': case 'S': if (snake->dir != UP) snake->dir = DOWN; break;
            case 'a': case 'A': if (snake->dir != RIGHT) snake->dir = LEFT; break;
            case 'd': case 'D': if (snake->dir != LEFT) snake->dir = RIGHT; break;
        }
    }
}

int main() {
    Snake snake = {{{10, 10}}, 1, RIGHT};  // Initial position of the snake
    Food food;
    generate_food(&food, &snake);

    while (1) {
        draw(&snake, &food);
        input(&snake);
        update(&snake, &food);
        if (collision(&snake)) {
            printf("Game Over!\n");
            break;
        }
        Sleep(100);  // Sleep for 100 milliseconds to control the snake's speed
    }

    return 0;
}