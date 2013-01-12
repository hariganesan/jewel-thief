// Hari Ganesan 1/12/13
// Grid functions for jewel-thief

#include "Grid.h"

Grid::Grid() {
	// initialize grid pointers (very inefficient)
	for (int i = 0; i < GRID_HEIGHT; i++) {
		for (int j = 0; j < GRID_WIDTH; j++) {
			// also set coords
			grid[i][j].x = j;
			grid[i][j].y = i;

			if (i - 1 < 0) {
				grid[i][j].up = NULL;
			} else {
				grid[i][j].up = &grid[i-1][j];
			}

			if (i + i >= GRID_HEIGHT) {
				grid[i][j].down = NULL;
			} else {
				grid[i][j].down = &grid[i+1][j];
			}

			if (j - 1 < 0) {
				grid[i][j].left = NULL;
			} else {
				grid[i][j].left = &grid[i][j-1];
			}

			if (j + 1 >= GRID_WIDTH) {
				grid[i][j].right = NULL;
			} else {
				grid[i][j].right = &grid[i][j+1];
			}
		}
	}

	fillGrid();
}

void Grid::fillGrid() {
	srand(time(NULL));
	
	for (int i = 0; i < GRID_HEIGHT; i++) {
		for (int j = 0; j < GRID_WIDTH; j++) {
			grid[i][j].color = rand() % 7;
			grid[i][j].selected = false;
			grid[i][j].locked = false;
			grid[i][j].special = (rand() % 30 == 0) ? true : false;
		}
	}
}

void Grid::displayGrid(int x, int y) {
	for (int i = 0; i < GRID_HEIGHT; i++) {
		for (int j = 0; j < GRID_WIDTH; j++) {

			pickColor(grid[i][j]);
			
			glBegin(GL_QUADS);

			// we need to flip the vertical axis
			glVertex2f(x+j*JEWEL_DIM+BORDER,
								 y+JEWEL_DIM*GRID_HEIGHT-i*JEWEL_DIM-BORDER);
			glVertex2f(x+(j+1)*JEWEL_DIM-BORDER,
								 y+JEWEL_DIM*GRID_HEIGHT-i*JEWEL_DIM-BORDER);
			glVertex2f(x+(j+1)*JEWEL_DIM-BORDER,
								 y+JEWEL_DIM*GRID_HEIGHT-(i+1)*JEWEL_DIM+BORDER);
			glVertex2f(x+j*JEWEL_DIM+BORDER,
								 y+JEWEL_DIM*GRID_HEIGHT-(i+1)*JEWEL_DIM+BORDER);

			glEnd();

			// selected? locked?
			if (grid[i][j].selected || grid[i][j].locked) {
				if (grid[i][j].locked) {
					glColor4ub(205, 205, 55, 255);
				} else {
					glColor4ub(255,255,255,255);
				}

				glBegin(GL_LINE_LOOP);

				glVertex2f(x+j*JEWEL_DIM,
									 y+JEWEL_DIM*GRID_HEIGHT-i*JEWEL_DIM);
				glVertex2f(x+(j+1)*JEWEL_DIM,
									 y+JEWEL_DIM*GRID_HEIGHT-i*JEWEL_DIM);
				glVertex2f(x+(j+1)*JEWEL_DIM,
									 y+JEWEL_DIM*GRID_HEIGHT-(i+1)*JEWEL_DIM);
				glVertex2f(x+j*JEWEL_DIM,
									 y+JEWEL_DIM*GRID_HEIGHT-(i+1)*JEWEL_DIM);

				glEnd();
			}

		}
	}
}

void Grid::pickColor(Jewel jewel) {
	int color = jewel.color;



	if (color == RED) {
		glColor4ub(205, 0, 0, 255);
	} else if (color == ORANGE) {
		glColor4ub(255, 105, 0, 255);
	} else if (color == YELLOW) {
		glColor4ub(255, 255, 0, 255);
	} else if (color == GREEN) {
		glColor4ub(0, 155, 55, 255);
	} else if (color == BLUE) {
		glColor4ub(0, 0, 255, 255);
	} else if (color == PURPLE) {
		glColor4ub(155, 0, 155, 255);
	} else { // color is white
		glColor4ub(155, 155, 155, 255);
	}

	// sparkles
	/*
	if (jewel.special && (rand() % 100 == 0)) {
		glColor4ub(210, 210, 210, 255);
	} 
	*/


}

void Grid::selectJewel(int x, int y) {
	for (int i = 0; i < GRID_HEIGHT; i++) {
		for (int j = 0; j < GRID_WIDTH; j++) {
			grid[i][j].selected = false;
		}
	}

	grid[y][x].selected = true;
}

void Grid::lockJewel(bool locked) {
	for (int i = 0; i < GRID_HEIGHT; i++) {
		for (int j = 0; j < GRID_WIDTH; j++) {
			if (grid[i][j].selected) {
				if (locked) {
					grid[i][j].locked = true;
				} else {
					grid[i][j].locked = false;
				}
			}
		}
	}
}

