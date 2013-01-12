// Hari Ganesan 1/12/13
// Grid library file for jewel-thief

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>

using namespace std;

const int GRID_HEIGHT = 10;
const int GRID_WIDTH = 10;
const int JEWEL_DIM = 40;
const int BORDER = 5;

enum {
	WHITE,
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	BLUE,
	PURPLE
};

struct Jewel {
	int color;
	bool special;
	bool selected;
	bool locked;

	Jewel *left;
	Jewel *down;
	Jewel *right;
	Jewel *up;

	// coords in grid
	int x;
	int y;
};

class Grid {
	Jewel grid[GRID_HEIGHT][GRID_WIDTH];
public:
	Grid();
	void fillGrid();
	void displayGrid(int x, int y);
	void pickColor(Jewel jewel);
	void selectJewel(int x, int y);
	void lockJewel(bool locked);
};