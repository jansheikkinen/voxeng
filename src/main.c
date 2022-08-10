/* main.c */

#include <raylib.h>

const unsigned int scrWidth = 500;
const unsigned int scrHeight = 500;

int main(void) {
  InitWindow(scrWidth, scrHeight, "test");
  SetTargetFPS(60);

  while(!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawText("Testin :)", scrWidth / 2, scrHeight / 2, 20, LIGHTGRAY);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
