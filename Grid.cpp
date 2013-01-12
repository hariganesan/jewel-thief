//#include <stdio>
#include <string>
#include <math.h>

using namespace std;

enum COLORS {
	BLACK = 0;
	RED = 1;
	ORANGE = 2;
	YELLOW = 3;
	GREEN = 4;
	BLUE = 5;
	PURPLE = 6;
};

struct Jewel {
	int color;
	bool special;

	Jewel *left;
	Jewel *down;
	Jewel *right;
	Jewel *up;
};

class Grid {

	Jewel grid[GRID_HEIGHT][GRID_WIDTH];

	Grid() {
		// initialize grid pointers (very inefficient)
		for (int i = 0; i < GRID_HEIGHT; i++) {
			for (int j = 0; j < GRID_WIDTH; j++) {
				if (i - 1 < 0) {
					grid[i][j]->up = NULL;
				} else {
					grid[i][j]->up = grid[i-1][j];
				}

				if (i + i >= GRID_HEIGHT) {
					grid[i][j]->down = NULL;
				} else {
					grid[i][j]->down = grid[i+1][j];
				}

				if (j - 1 < 0) {
					grid[i][j]->left = NULL;
				} else {
					grid[i][j]->left = grid[i][j-1]
				}

				if (j + 1 >= GRID_WIDTH) {
					grid[i][j]->right = NULL;
				} else {
					grid[i][j]->right = grid[i][j+1];
				}
			}
		}

		fillGrid();
	}

	fillGrid() {
		for (int i = 0; i < GRID_HEIGHT; i++) {
			for (int j = 0; j < GRID_WIDTH; j++) {
				srand(time(NULL));
				grid[i][j].color = rand % 7;
			}
		}
	}


};
