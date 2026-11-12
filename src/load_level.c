#include "raylib.h"
#include <stdio.h>
#include <stdint.h>

#define GRID_SIZE 64
#define TILE_SIZE 12
#define MAX_FILENAME_LEN 16

typedef struct {
  float x, y;
  float vx, vy;
} Player;

typedef uint8_t Grid[GRID_SIZE][GRID_SIZE];

char fileName[MAX_FILENAME_LEN];

int LoadLevel(const size_t level, Grid grid, Player *player)
{
  int bytesWritten = snprintf(fileName, MAX_FILENAME_LEN, "levels/%ld.txt", level);
  if (bytesWritten >= MAX_FILENAME_LEN || bytesWritten < 0) {
    printf("Error formatting level file name\n");
    return 0;
  }
  int byteCount = 0;
  unsigned char *fileData = LoadFileData(fileName, &byteCount);

  if (fileData == NULL) {
    printf("unable to load file data\n");
    return 0;
  }

  printf("File loaded. %d bytes\n", byteCount);

  // Parse level file - TODO: probably will break on windows, should use raylib function

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
      player->x = x;
      player->y = y;
      grid[lineCount][col] = 0;
      break;
    case 'a':
      grid[lineCount][col] = 'a';
      x = (i % (GRID_SIZE + 1)) * TILE_SIZE;
      y = lineCount * TILE_SIZE;
      // allocate mem for new door
      break;
    case 'A':
      grid[lineCount][col] = 'A';	    
      break;
    case '.':
    default:
      grid[lineCount][col] = 0;
      break;
    }
  }

  // parseing done  
  return 1;
} 
