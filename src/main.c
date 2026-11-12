#include "raylib.h"
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include "load_level.c"
#include "start_menu.c"
#include "game_state.h"

const float speed = 500.0;

float GetWindowPos(size_t gridIndex)
{
  return gridIndex * TILE_SIZE;
}

size_t GetGridIndex(float windowPosition)
{
  return (size_t)(windowPosition / TILE_SIZE);
}

uint8_t IsSolidTile(const Grid grid, const size_t x, const size_t y)
{
  return grid[y][x] == 1
    || grid[y][x] == 'a';
}

uint8_t IsColliding()
{
   GetGridIndex(player.y) + 1;
  size_t tileIndexAbove = GetGridIndex(player.y);
  size_t tileIndexRight = GetGridIndex(player.x) + 1;
  size_t tileIndexLeft = GetGridIndex(player.x);       
}

int main(void)
{
  InitWindow(WIDTH, HEIGHT, "Mazeing");
  SetTargetFPS(60);

  GameState gameState = START_MENU;
  size_t level = 1;
  Player player = {0};
  Grid grid;

  while (!WindowShouldClose())
    {
      switch (gameState) {
      case START_MENU:
	StartMenu(&gameState);
	break;
      case LOADING_LEVEL:
	int levelLoaded = LoadLevel(level, grid, &player);
	
	if (!levelLoaded) {
	  printf("Failed to load level\n");
	}

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

	if (IsSolidTile(grid, GetGridIndex(player.x), tileIndexBelow)) {
	  player.y = GetWindowPos(tileIndexBelow) - TILE_SIZE;
	  player.vy = 0;
	}

	if (IsSolidTile(grid, GetGridIndex(player.x), tileIndexAbove)) {
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

	if (IsSolidTile(grid, tileIndexRight, GetGridIndex(player.y))) {
	  player.x = GetWindowPos(tileIndexRight) - TILE_SIZE;
	  player.vx = 0;
	}

	if (IsSolidTile(grid, tileIndexLeft, GetGridIndex(player.y))) {
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

	if (grid[tileIndexAbove][GetGridIndex(player.x)] == 'A' ||
	    grid[tileIndexBelow][GetGridIndex(player.x)] == 'A' ||
	    grid[GetGridIndex(player.y)][tileIndexRight] == 'A' ||
	    grid[GetGridIndex(player.y)][tileIndexLeft] == 'A') {
	  // make door slide open then slide closed
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
	    case 'a':
	      DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
	      break;
	    case 'A':
	      DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
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
