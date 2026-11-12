#include "raylib.h"
#include "game_state.h"

static size_t currentMenuItem = 0;  

static char *menu[] = {
  "Start new game",
  "Load saved game",
  "Level editor",
  "Settings"
};

static size_t menuLength = sizeof(menu) / sizeof(menu[0]);

void StartMenu(GameState *gameState)
{
  BeginDrawing();
  ClearBackground(BLACK);

  // TODO: center menu items properley
  for (size_t i = 0; i < menuLength; ++i) {
    DrawText(menu[i], (WIDTH / 2) - 100, (i + 1) * 100, 25, currentMenuItem == (i + 1) ? BLUE : PURPLE);	  
  }

  if (IsKeyPressed(KEY_ENTER)) {
    switch (currentMenuItem) {
    case 1:
      *gameState = LOADING_LEVEL;
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
}
