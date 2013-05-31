// Hari Ganesan 1/12/13
// Grid functions for jewel-thief

#include "Grid.hpp"

Grid::Grid() : numMoves(0), remove(false) {
	// initialize grid pointers (very inefficient)
	for (int i = 0; i < GRID_HEIGHT; i++) {
		for (int j = 0; j < GRID_WIDTH; j++) {
			// also set coords
			grid[i][j].x = j;
			grid[i][j].y = i;

			// flip vertical axes?
			if (i - 1 < 0) {
				grid[i][j].down = NULL;
			} else {
				grid[i][j].down = &grid[i-1][j];
			}

			if (i + 1 >= GRID_HEIGHT) {
				grid[i][j].up = NULL;
			} else {
				grid[i][j].up = &grid[i+1][j];
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
			grid[i][j].removed = false;
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

Jewel *Grid::selectJewel(int x, int y) {
	for (int i = 0; i < GRID_HEIGHT; i++) {
		for (int j = 0; j < GRID_WIDTH; j++) {
			grid[i][j].selected = false;
		}
	}

	grid[y][x].selected = true;
	return &(grid[y][x]);
}

Jewel *Grid::selectJewel(Jewel *oldJewel, Jewel *jewel) {
	(*oldJewel).selected = false;
	(*jewel).selected = true;

	return jewel;
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

Jewel *Grid::swap(Jewel *locked, Jewel *selected) {
	Jewel temp;

	// for edges
	if (selected == NULL)
		return locked;

	// transfer all properties - color, special, x, y
	// TODO: should just switch boxes and update pointers instead...
	temp.color = (*locked).color;
	temp.special = (*locked).special;
	temp.selected = (*locked).selected;
	temp.locked = (*locked).locked;
	temp.x = (*locked).x;
	temp.y = (*locked).y;

	(*locked).color = (*selected).color;
	(*locked).special = (*selected).special;
	(*locked).selected = (*selected).selected;
	(*locked).locked = (*selected).locked;
	(*locked).x = (*selected).x;
	(*locked).y = (*selected).y;

	(*selected).color = temp.color;
	(*selected).special = temp.special;
	(*selected).selected = temp.selected;
	(*selected).locked = temp.locked;
	(*selected).x = temp.x;
	(*selected).y = temp.y;

	numMoves++;

	checkJewel(selected);
	checkJewel(locked);

	return selected;
}

Jewel *Grid::move(Jewel *selectedJewel, string direction) {
	if (direction == "right") {
		if ((*selectedJewel).right != NULL) {
			return selectJewel(selectedJewel, (*selectedJewel).right);
		}
	} else if (direction == "left") {
		if ((*selectedJewel).left != NULL) {
			return selectJewel(selectedJewel, (*selectedJewel).left);
		}
	} else if (direction == "up") {
		if ((*selectedJewel).up != NULL) {
			return selectJewel(selectedJewel, (*selectedJewel).up);
		}
	} else if (direction == "down") {
		if ((*selectedJewel).down != NULL) {
			return selectJewel(selectedJewel, (*selectedJewel).down);
		}
	} else {
		fprintf(stderr, "error: no direction given");
	}

	return selectedJewel;
}

// checks to see if a Jewel satisfies a requirement in any direction
void Grid::checkJewel(Jewel *original) {
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

	if (right+left >= 3 || up+down >= 3) {
		removeJewels(original, right, left, up, down);
	}
}

void Grid::removeJewels(Jewel *original, int right, int left, int up, int down) {
	;
}
