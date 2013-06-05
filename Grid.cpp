// Hari Ganesan 1/12/13
// Grid functions for jewel-thief

#include "Grid.hpp"

Grid::Grid() : numMoves(0), remove(false) {
	fillGrid();
	
	// initialize grid pointers (very inefficient)
	for (int i = 0; i < GRID_WIDTH; i++) {
		for (int j = 0; j < GRID_HEIGHT; j++) {
			// also set coords
			grid[i][j]->x = i;
			grid[i][j]->y = j;

			// only flip axes for display fns!
			if (j - 1 < 0) {
				grid[i][j]->down = NULL;
			} else {
				grid[i][j]->down = grid[i][j-1];
			}

			if (j + 1 >= GRID_HEIGHT) {
				grid[i][j]->up = NULL;
			} else {
				grid[i][j]->up = grid[i][j+1];
			}

			if (i - 1 < 0) {
				grid[i][j]->left = NULL;
			} else {
				grid[i][j]->left = grid[i-1][j];
			}

			if (i + 1 >= GRID_WIDTH) {
				grid[i][j]->right = NULL;
			} else {
				grid[i][j]->right = grid[i+1][j];
			}
		}
	}
}

void Grid::fillGrid() {
	for (int i = 0; i < GRID_WIDTH; i++) {
		for (int j = 0; j < GRID_HEIGHT; j++) {
			grid[i][j] = new Jewel();
		}
	}
}

void Grid::displayGrid(int x, int y) {
	for (int i = 0; i < GRID_WIDTH; i++) {
		for (int j = 0; j < GRID_HEIGHT; j++) {

			pickColor(grid[i][j]);
			
			glBegin(GL_QUADS);

			// we need to flip the vertical axis
			glVertex2f(x+i*JEWEL_DIM+BORDER,
								 y+JEWEL_DIM*GRID_HEIGHT-j*JEWEL_DIM-BORDER);
			glVertex2f(x+(i+1)*JEWEL_DIM-BORDER,
								 y+JEWEL_DIM*GRID_HEIGHT-j*JEWEL_DIM-BORDER);
			glVertex2f(x+(i+1)*JEWEL_DIM-BORDER,
								 y+JEWEL_DIM*GRID_HEIGHT-(j+1)*JEWEL_DIM+BORDER);
			glVertex2f(x+i*JEWEL_DIM+BORDER,
								 y+JEWEL_DIM*GRID_HEIGHT-(j+1)*JEWEL_DIM+BORDER);

			glEnd();

			// selected? locked?
			if (grid[i][j]->selected || grid[i][j]->locked) {
				if (grid[i][j]->locked) {
					glColor4ub(205, 205, 55, 255);
				} else {
					glColor4ub(255,255,255,255);
				}

				glBegin(GL_LINE_LOOP);

				glVertex2f(x+i*JEWEL_DIM,
									 y+JEWEL_DIM*GRID_HEIGHT-j*JEWEL_DIM);
				glVertex2f(x+(i+1)*JEWEL_DIM,
									 y+JEWEL_DIM*GRID_HEIGHT-j*JEWEL_DIM);
				glVertex2f(x+(i+1)*JEWEL_DIM,
									 y+JEWEL_DIM*GRID_HEIGHT-(j+1)*JEWEL_DIM);
				glVertex2f(x+i*JEWEL_DIM,
									 y+JEWEL_DIM*GRID_HEIGHT-(j+1)*JEWEL_DIM);

				glEnd();
			}

		}
	}
}

void Grid::pickColor(Jewel *jewel) {
	int color = jewel->color;

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
	if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
		return;

	for (int i = 0; i < GRID_WIDTH; i++) {
		for (int j = 0; j < GRID_HEIGHT; j++) {
			grid[i][j]->selected = false;
		}
	}

	selectedX = x;
	selectedY = y;
	grid[x][y]->selected = true;
	return;
}

// deprecated
/*
Jewel *Grid::selectJewel(Jewel *oldJewel, Jewel *jewel) {
	(*oldJewel).selected = false;
	(*jewel).selected = true;

	return jewel;
} */

void Grid::toggleLockedJewel(bool locked) {
	if (locked) {
		grid[selectedX][selectedY]->locked = true;
	}	else {
		grid[selectedX][selectedY]->locked = false;
	}
}

// TODO: rewrite for basic pointers.
void Grid::swap(int newX, int newY) {
	Jewel *temp;

	// for edges
	if (newX < 0 || newX >= GRID_WIDTH || newY < 0 || newY >= GRID_HEIGHT)
		return;

	temp = grid[newX][newY];
	grid[newX][newY] = grid[selectedX][selectedY];
	grid[selectedX][selectedY] = temp;

	numMoves++;

	checkJewel(newX, newY);
	checkJewel(selectedX, selectedY);

	selectJewel(newX, newY);

	return;
}

// checks to see if a Jewel satisfies a requirement in any direction
void Grid::checkJewel(int origX, int origY) {
	int right, left, up, down;
	right = left = up = down = 0;
	int currX, currY;
	Jewel *next;

	currX = origX; currY = origY;
	while (currX + 1 < GRID_WIDTH) {
		next = grid[currX+1][currY];
		if (next->color != grid[currX][currY]->color) {	
			break;
		} else {
			right++;
			currX++;
		}
	}

	currX = origX; currY = origY;
	while (currX - 1 >= 0) {
		next = grid[currX-1][currY];
		if (next->color != grid[currX][currY]->color) {	
			break;
		} else {
			left++;
			currX--;
		}
	}

	currX = origX; currY = origY;
	while (currY + 1 < GRID_HEIGHT) {
		next = grid[currX][currY+1];
		if (next->color != grid[currX][currY]->color) {	
			break;
		} else {
			up++;
			currY++;
		}
	}

	currX = origX; currY = origY;
	while (currY - 1 >= 0) {
		next = grid[currX][currY-1];
		if (next->color != grid[currX][currY]->color) {	
			break;
		} else {
			down++;
			currY--;
		}
	}

	removeJewels(origX, origY, right, left, up, down);
}

void Grid::removeJewels(int origX, int origY, int right, int left, int up, int down) {
	int currX, currY;
	int chainX, chainY;

	if (right+left >= 3) {
		currX = origX; currY = origY;

		// move to left
		for (int i = 0; i < left; i++) {
			currX--;
		}

		// move across
		// except for possibility of (up+down)
		for (int i = 0; i < left+right+1; i++) {
			if (currX >= GRID_WIDTH)
				cerr << "error: out of bounds Grid.cpp/252" << endl;

			if (!(up+down >= 3 && i == left)) {
				chainX = currX; chainY = currY;
				while (chainY + 1 < GRID_HEIGHT) {
					grid[chainX][chainY] = grid[chainX][chainY+1];
					chainY++;
				}

				grid[chainX][chainY] = new Jewel(); // copy jewel to grid
			}
			// move across
			currX++;
		}
	}

	if (up+down >= 3) {
		currX = origX; currY = origY;

		// move down
		for (int i = 0; i < down; i++) {
			currY--;
		}

		// move chain up
		chainX = currX; chainY = currY;
		for (int i = 0; i < down+up+1; i++) {
			chainY++;
		}

		// copy chain down to current if possible
		for (int i = 0; currY < GRID_HEIGHT; i++) {
			if (chainY < GRID_HEIGHT) {
				grid[currX][currY] = grid[chainX][chainY]; // copy jewel to grid
				chainY++;
			} else {
				grid[currX][currY] = new Jewel(); // copy jewel to grid
			}

			currY++;
		}
	}
}
