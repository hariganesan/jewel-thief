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

			// flip vertical axes?
			// only do for display fns!
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
	srand(time(NULL));
	
	for (int i = 0; i < GRID_WIDTH; i++) {
		for (int j = 0; j < GRID_HEIGHT; j++) {
			grid[i][j] = new Jewel();

			grid[i][j]->color = rand() % 7;
			grid[i][j]->selected = false;
			grid[i][j]->locked = false;
			grid[i][j]->special = (rand() % 30 == 0) ? true : false;
			grid[i][j]->removed = false;
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

void Grid::lockJewel(bool locked) {
	for (int i = 0; i < GRID_WIDTH; i++) {
		for (int j = 0; j < GRID_HEIGHT; j++) {
			if (grid[i][j]->selected) {
				if (locked) {
					grid[i][j]->locked = true;
				} else {
					grid[i][j]->locked = false;
				}
			}
		}
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
void Grid::checkJewel(int x, int y) {
	Jewel *original = grid[x][y];
	int right, left, up, down;
	right = left = up = down = 0;
	Jewel *current, *next;

	current = original;
	while ((next = current->right) != NULL) {
		if (next->color != current->color) {
			break;
		} else {
			right++;
			current = next;
		}
	}

	current = original;
	while ((next = current->left) != NULL) {
		if (next->color != current->color) {
			break;
		} else {
			left++;
			current = next;
		}
	}

	current = original;
	while ((next = current->up) != NULL) {
		if (next->color != current->color) {
			break;
		} else {
			up++;
			current = next;
		}
	}

	current = original;
	while ((next = current->down) != NULL) {
		if (next->color != current->color) {
			break;
		} else {
			down++;
			current = next;
		}
	}

	cout << "right: " << right << endl;
	cout << "left: " << left << endl;
	cout << "up: " << up << endl;
	cout << "down: " << down << endl;

	//removeJewels(x, y, right, left, up, down);
}

void Grid::removeJewels(int x, int y, int right, int left, int up, int down) {
	Jewel *original = grid[x][y];
	Jewel *current, *chain;

	if (right+left >= 3) {
		current = original;

		// move to left
		for (int i = 0; i < left; i++) {
			if (current->left != NULL) {
				current = current->left;
			}
		}

		// move across, switching pointers
		// except for possibility of (up+down)
		for (int i = 0; i < left+right; i++) {
			if (!(up+down < 3 && i == left)) {
				chain = current;
				while (chain->up != NULL) {
					chain = chain->up; // copy jewels to grid
				}

				chain->up = new Jewel(); // copy jewel to grid
			}
			// move across
			if (current->right != NULL)
				current = current->right;
		}
	}

	if (up+down >= 3) {
		current = original;

		// move down
		for (int i = 0; i < down; i++) {
			if (current->down != NULL) {
				current = current->down;
			}
		}

		// move chain up
		chain = current;
		for (int i = 0; i < down+up; i++) {
			if (chain->up != NULL) {
				chain = chain->up;
			}
		}

		// copy chain down to current if possible
		for (int i = 0; i < down+up; i++) {
			if (chain != NULL) {
				current = chain; // copy jewel to grid
				chain = new Jewel(); // copy jewel to grid
				chain = chain->up;
			} else {
				current = new Jewel(); // copy jewel to grid
			}

			current = current->up;
		}
	}
}
