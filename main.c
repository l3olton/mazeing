#include "raylib.h"
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#define WIDTH 800
#define HEIGHT 800
#define TILE_SIZE 12
#define GRID_SIZE 64

typedef enum {
  START_MENU,
  LOADING_LEVEL,
  PLAYING,
  COMPLETED
} GameState;

typedef struct {
  float x, y;
  float vx, vy;
} Player;

const float speed = 500.0;

typedef uint8_t Grid[GRID_SIZE][GRID_SIZE];

float GetWindowPos(size_t gridIndex)
{
  return gridIndex * TILE_SIZE;
}

size_t GetGridIndex(float windowPosition)
{
  return (size_t)(windowPosition / TILE_SIZE);
}

int main(void)
{
  InitWindow(WIDTH, HEIGHT, "Mazeing");
  SetTargetFPS(60);

  GameState gameState = START_MENU;
  size_t level = 1;
  Player player = {0};
  Grid grid;
  size_t currentMenuItem = 0;  

  const size_t maxFileNameLength = 16;
  char fileName[maxFileNameLength];  

  while (!WindowShouldClose())
    {
      switch (gameState) {
      case START_MENU:
	BeginDrawing();
	ClearBackground(BLACK);

	char *menu[] = {
	  "Start new game",
	  "Load saved game",
	  "Level editor",
	  "Settings"
	};

	size_t menuLength = sizeof(menu) / sizeof(menu[0]);

	// TODO: center menu items properley
	for (size_t i = 0; i < menuLength; ++i) {
	  DrawText(menu[i], (WIDTH / 2) - 100, (i + 1) * 100, 25, currentMenuItem == (i + 1) ? BLUE : PURPLE);	  
	}

	if (IsKeyPressed(KEY_ENTER)) {
	  switch (currentMenuItem) {
	  case 1:
	    gameState = LOADING_LEVEL;
	    break;
	  default:
	    printf("Not implemented\n");
	    break;
	  }
	}

	if (IsKeyPressed(KEY_DOWN)) {
	  currentMenuItem = ++currentMenuItem % (menuLength + 1);
	}

	// TODO: cycling from top to bottom item doesnt work
	if (IsKeyPressed(KEY_UP)) {
	  currentMenuItem = --currentMenuItem % (menuLength + 1);
	}	
	
	EndDrawing();
	break;
      case LOADING_LEVEL:


	// read file
  
	int bytesWritten = snprintf(fileName, maxFileNameLength, "levels/%ld.txt", level);
	if (bytesWritten >= maxFileNameLength || bytesWritten < 0) {
	  // TODO: error handling
	  printf("Error formatting level file name\n");
	  return 1;
	}
	int byteCount = 0;
	unsigned char *fileData = LoadFileData(fileName, &byteCount);

	if (fileData == NULL) {
	  // TODO: error handling
	  printf("unable to load file data\n");
	  return 1;
	}

	printf("File loaded. %d bytes\n", byteCount);

	// done reading file	

	// Parse level file

	size_t lineCount = 0;
	for (size_t i = 0; i < byteCount; ++i) {
	  size_t x, y;

	  size_t col = i % (GRID_SIZE + 1);
	  if (col > GRID_SIZE) continue;
	
	  switch (fileData[i]) {
	  case '\n':
	    lineCount++;
	    break;
	  case 'w':
	    grid[lineCount][col] = 1;
	    break;
	  case 'e':
	    grid[lineCount][col] = 2;
	    break;
	  case 'p':
	    x = (i % (GRID_SIZE + 1)) * TILE_SIZE;
	    y = lineCount * TILE_SIZE;
	    player.x = x;
	    player.y = y;
	    grid[lineCount][col] = 0;
	    break;
	  case '.':
	  default:
	    grid[lineCount][col] = 0;
	    break;
	  }
	}

	// parseing done

	gameState = PLAYING;

	break;
      case PLAYING:	

	float dt = GetFrameTime();
	player.vy = 0;
	player.vx = 0;

	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
	  player.vy = -speed;
	}
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
	  player.vx = -speed;
	}
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
	  player.vy = speed;
	}
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
	  player.vx = speed;
	}

	float nextX = player.x + player.vx * dt;
	float nextY = player.y + player.vy * dt;

	// move in y-axis
	player.y = nextY;

	// calc overlapping tiles
	size_t tileIndexBelow = GetGridIndex(player.y) + 1;
	size_t tileIndexAbove = GetGridIndex(player.y);
	size_t tileIndexRight = GetGridIndex(player.x) + 1;
	size_t tileIndexLeft = GetGridIndex(player.x);      

	if (grid[tileIndexBelow][GetGridIndex(player.x)] == 1) { // TODO: func for this condition
	  player.y = GetWindowPos(tileIndexBelow) - TILE_SIZE;
	  player.vy = 0;
	}

	if (grid[tileIndexAbove][GetGridIndex(player.x)] == 1) {
	  player.y = GetWindowPos(tileIndexAbove) + TILE_SIZE;
	  player.vy = 0;
	}

	// move in x-axis
	player.x = nextX;

	// re-calc overlapping tiles
	tileIndexBelow = GetGridIndex(player.y) + 1;
	tileIndexAbove = GetGridIndex(player.y);
	tileIndexRight = GetGridIndex(player.x) + 1;
	tileIndexLeft = GetGridIndex(player.x);            

	if (grid[GetGridIndex(player.y)][tileIndexRight] == 1) {
	  player.x = GetWindowPos(tileIndexRight) - TILE_SIZE;
	  player.vx = 0;
	}

	if (grid[GetGridIndex(player.y)][tileIndexLeft] == 1) {
	  player.x = GetWindowPos(tileIndexLeft) + TILE_SIZE;
	  player.vx = 0;
	}

	if (grid[tileIndexAbove][GetGridIndex(player.x)] == 2 ||
	    grid[tileIndexBelow][GetGridIndex(player.x)] == 2 ||
	    grid[GetGridIndex(player.y)][tileIndexRight] == 2 ||
	    grid[GetGridIndex(player.y)][tileIndexLeft] == 2) {
	  level++;
	  gameState = LOADING_LEVEL;
	}	

	if (IsKeyPressed(KEY_R)) {
	  gameState = LOADING_LEVEL;
	}

	BeginDrawing();
	ClearBackground(BLACK);

	for (size_t y = 0; y < sizeof(grid) / sizeof(grid[0]); ++y) {
	  for (size_t x = 0; x < sizeof(grid[0]); ++x) {
	    switch (grid[y][x]) {
	    case 1:
	      DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, PURPLE);
	      break;
	    case 2:
	      DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, YELLOW);
	      break;
	    }
	  }
	}

	DrawRectangle(player.x, player.y, TILE_SIZE, TILE_SIZE, BLUE);

	EndDrawing();	
	
	break;
      case COMPLETED:
	break;
      }
    }

  CloseWindow();

  return 0;
}
