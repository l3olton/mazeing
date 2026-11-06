#include "raylib.h"
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#define WIDTH 800
#define HEIGHT 800
#define TILE_SIZE 12
#define MAX_CHAR_WIDTH 64

typedef struct {
  float x;
  float y;
} Position;

typedef struct {
  float x, y;
  float vx, vy;
} Player;

const float speed = 500.0;

typedef uint8_t Grid[MAX_CHAR_WIDTH][MAX_CHAR_WIDTH];

float GetWindowPos(size_t gridY)
{
  return gridY * TILE_SIZE;
}

size_t GetGridIndex(float windowY)
{
  return (size_t)(windowY / TILE_SIZE);
}

int main(void)
{
  InitWindow(WIDTH, HEIGHT, "Mazeing 1");
  SetTargetFPS(60);

  Player player = {0};
  Position initialPlayerPosition = {0};

  int byteCount = 0;
  unsigned char *fData = LoadFileData("1.maze", &byteCount);

  if (fData == NULL) {
    printf("unable to load file data\n");
  }

  printf("File loaded. %d bytes\n", byteCount);

  Grid grid;

  // Parse level file

  size_t lineCount = 0;
  for (size_t i = 0; i < byteCount; ++i) {
    size_t x, y;

    size_t col = i % (MAX_CHAR_WIDTH + 1);
    if (col > MAX_CHAR_WIDTH) continue;
	
    switch (fData[i]) {
    case '\n':
      lineCount++;
      break;
    case 'w':
      grid[lineCount][col] = 1;
      break;
    case 'e':
      // TODO: get end position
      grid[lineCount][col] = 2;
      break;
    case 'p':
      x = (i % (MAX_CHAR_WIDTH + 1)) * TILE_SIZE;
      y = lineCount * TILE_SIZE;
      player.x = x;
      player.y = y;
      initialPlayerPosition.x = x;
      initialPlayerPosition.y = y;
      grid[lineCount][col] = 0;
      break;
    case '.':
    default:
      grid[lineCount][col] = 0;
      break;
    }
  }

  // parseing done

  while (!WindowShouldClose())
    {
      float dt = GetFrameTime();
      player.vy = 0;
      player.vx = 0;

      if (IsKeyDown(KEY_W)) {
	player.vy = -speed;
      }
      if (IsKeyDown(KEY_A)) {
	player.vx = -speed;
      }
      if (IsKeyDown(KEY_S)) {
	player.vy = speed;
      }
      if (IsKeyDown(KEY_D)) {
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

      if (grid[tileIndexBelow][GetGridIndex(player.x)]) {
	player.y = GetWindowPos(tileIndexBelow) - TILE_SIZE;
	player.vy = 0;
      }

      if (grid[tileIndexAbove][GetGridIndex(player.x)]) {
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

      if (grid[GetGridIndex(player.y)][tileIndexRight]) {
	player.x = GetWindowPos(tileIndexRight) - TILE_SIZE;
	player.vx = 0;
      }

      if (grid[GetGridIndex(player.y)][tileIndexLeft]) {
	player.x = GetWindowPos(tileIndexLeft) + TILE_SIZE;
	player.vx = 0;
      }

      if (IsKeyPressed(KEY_R)) {
	player.x = initialPlayerPosition.x;
	player.y = initialPlayerPosition.y;
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
	    DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKPURPLE);
	    break;
	  }
	}
      }

      DrawRectangle(player.x, player.y, TILE_SIZE, TILE_SIZE, BLUE);

      EndDrawing();
    }

  CloseWindow();

  return 0;
}
