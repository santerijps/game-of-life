#include "raylib.h"
#include "main.h"


int main(void) {
  TraceLog(LOG_INFO, "GAME OF LIFE IS RUNNING");
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Conway's Game of Life [raylib]");
  SetTargetFPS(FPS);

  int CELL_SIZE = gcd(WINDOW_WIDTH, WINDOW_HEIGHT);
  while (CELL_SIZE % 2 == 0) CELL_SIZE /= 2;

  int ROW_COUNT = WINDOW_HEIGHT / CELL_SIZE;
  int COL_COUNT = WINDOW_WIDTH / CELL_SIZE;

  bool grid[ROW_COUNT][COL_COUNT], grid_next[ROW_COUNT][COL_COUNT];
  for (int row = 0; row < ROW_COUNT; row++) {
    for (int col = 0; col < COL_COUNT; col++) {
      grid[row][col] = DEAD;
    }
  }

  bool paused = true;
  while (!WindowShouldClose()) {
    switch (GetKeyPressed()) {
      // Toggle pause/play
      case KEY_SPACE:
        paused = !paused;
        break;
      // Clear the cells if paused
      case KEY_R:
        if (paused) {
          for (int row = 0; row < ROW_COUNT; row++) {
            for (int col = 0; col < COL_COUNT; col++) {
              grid[row][col] = DEAD;
            }
          }
        }
        break;
    }

    // Insert cell at mouse position if paused
    if (paused && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      int row = GetMouseY() / CELL_SIZE;
      int col = GetMouseX() / CELL_SIZE;
      grid[row][col] = !grid[row][col];
    }

    // Each tick when not paused
    if (!paused && tick()) {
      // Copy grid to grid_new
      for (int row = 0; row < ROW_COUNT; row++) {
        for (int col = 0; col < COL_COUNT; col++) {
          grid_next[row][col] = grid[row][col];
        }
      }

      // Calculate neighbors for each cell.
      // Update the grid_new 2d array.
      for (int row = 0; row < ROW_COUNT; row++) {
        for (int col = 0; col < COL_COUNT; col++) {
          bool tl = row > 0 && col > 0 ? grid[row - 1][col - 1] : false;
          bool t = row > 0 ? grid[row - 1][col] : false;
          bool tr = row > 0 && col + 1 < COL_COUNT ? grid[row - 1][col + 1] : false;
          bool l = col > 0 ? grid[row][col - 1] : false;
          bool r = col + 1 < COL_COUNT ? grid[row][col + 1] : false;
          bool bl = row + 1 < ROW_COUNT && col > 0 ? grid[row + 1][col - 1] : false;
          bool b = row + 1 < ROW_COUNT ? grid[row + 1][col] : false;
          bool br = row + 1 < ROW_COUNT && col + 1 < COL_COUNT ? grid[row + 1][col + 1] : false;
          int neighbor_count = tl + t + tr + l + r + bl + b + br;
          if (grid[row][col]) {
            if (neighbor_count != 2 && neighbor_count != 3) {
              grid_next[row][col] = DEAD;
            }
          } else {
            if (neighbor_count == 3) {
              grid_next[row][col] = LIVE;
            }
          }
        }
      }

      // Copy grid_new to grid (next generation)
      for (int row = 0; row < ROW_COUNT; row++) {
        for (int col = 0; col < COL_COUNT; col++) {
          grid[row][col] = grid_next[row][col];
        }
      }

    }

    BeginDrawing();
    ClearBackground(WHITE);

    // Draw cells
    for (int row = 0; row < ROW_COUNT; row++) {
      for (int col = 0; col < COL_COUNT; col++) {
        if (grid[row][col]) {
          DrawRectangle(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
        }
      }
    }

    // Draw grid lines if paused
    if (paused) {
      for (int x = 0; x < WINDOW_WIDTH; x += CELL_SIZE) {
        DrawLine(x, 0, x, WINDOW_HEIGHT, LIGHTGRAY);
      }
      for (int y = 0; y < WINDOW_HEIGHT; y += CELL_SIZE) {
        DrawLine(0, y, WINDOW_WIDTH, y, LIGHTGRAY);
      }
    }

    // Draw pause/play text
    DrawText(paused ? "PAUSED" : "PLAY", 5, 5, 20, paused ? ORANGE : GREEN);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}


bool tick(void) {
  static double t0 = 0;
  double t1 = GetTime();
  if (t1 - t0 >= TICK_RATE) {
    t0 = t1;
    return true;
  } else {
    return false;
  }
}


int gcd(int a, int b) {
  while(a != b) {
    if(a > b) a -= b;
    else b -= a;
  }
  return a;
}
