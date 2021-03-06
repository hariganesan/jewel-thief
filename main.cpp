// Hari Ganesan 1/12/13
// Jewel-thief

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "SDL_ttf/SDL_ttf.h"

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include "Grid.hpp"

// window dimensions
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int GRID_X = 100;
const int GRID_Y = 400;

using namespace std;

void runGame();
void render(Grid *g);

int main(int argc, char **argv) {
	// initialize SDL, TTF, and OpenGL
	SDL_Init(SDL_INIT_EVERYTHING);
	//glutInit(&argc, argv);
	TTF_Init();

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// initialize window properties
	SDL_WM_SetCaption("Jewel Zoo", NULL);
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_OPENGL);
	glClearColor(0, 0, 0, 1); // RGBA
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // viewable part of the window
	glShadeModel(GL_SMOOTH); // add a gradient
	glMatrixMode(GL_PROJECTION); // 2D drawing
	glLoadIdentity(); // save state
	glDisable(GL_DEPTH_TEST); // disable 3D drawing

	srand(time(NULL));
	runGame();

	TTF_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}

void runGame() {
	bool isRunning = true;
	bool lockedToggle = false;
	bool locked = false;
	Grid *g = new Grid();
	g->selectJewel(JEWEL_DEF_X,JEWEL_DEF_Y);
	Keys keys = Keys();

	SDL_Event event;	

	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || 
				 (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_q)) {
				isRunning = false;
			// lock a tile
			// TODO: hold both shift?
			}

			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RIGHT) {
				keys.right = true;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_LEFT) {
				keys.left = true;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP) {
				keys.up = true;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN) {
				keys.down = true;
			}

			if (event.type == SDL_KEYUP && 
				 (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)) {
				lockedToggle = true;
				locked = false;
			} else if (event.type == SDL_KEYDOWN && 
				 (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)) {
				lockedToggle = true;
				locked = true;
			}
		}

		// LOGIC
		if (locked) {
			if (keys.right) {
				g->swap(g->selectedX+1, g->selectedY);
				keys.right = false;
			} else if (keys.left) {
				g->swap(g->selectedX-1, g->selectedY);
				keys.left = false;
			} else if (keys.up) {
				g->swap(g->selectedX, g->selectedY+1);
				keys.up = false;
			} else if (keys.down) {
				g->swap(g->selectedX, g->selectedY-1);
				keys.down = false;
			}
		} else {
			if (keys.right) {
				g->selectJewel(g->selectedX+1, g->selectedY);
				keys.right = false;
			} else if (keys.left) {
				g->selectJewel(g->selectedX-1, g->selectedY);
				keys.left = false;
			} else if (keys.up) {
				g->selectJewel(g->selectedX, g->selectedY+1);
				keys.up = false;
			} else if (keys.down) {
				g->selectJewel(g->selectedX, g->selectedY-1);
				keys.down = false;
			}
		}

		if (lockedToggle) {
			g->toggleLockedJewel(locked);
			lockedToggle = false;
		}

		render(g);
	}

	return;
}

void render(Grid *g) {
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	// TODO: change to 0,1 for depth
	glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1); // set matrix

	////////////////
	// BEGIN DRAWING
	////////////////

	// GRID

	// gray color for grid
	glColor4ub(50, 50, 50, 255);

	// horizontal lines
	for (int i = 0; i <= GRID_HEIGHT; i++) {
		glBegin(GL_LINES);

		glVertex2f(GRID_X, (GRID_Y-25*GRID_HEIGHT)+i*JEWEL_DIM);
		glVertex2f(GRID_X+GRID_WIDTH*JEWEL_DIM,
							(GRID_Y-25*GRID_HEIGHT)+i*JEWEL_DIM);

		glEnd();
	}

	// vertical lines
	for (int i = 0; i <= GRID_WIDTH; i++) {
		glBegin(GL_LINES);

		glVertex2f(GRID_X+i*JEWEL_DIM, (GRID_Y-25*GRID_HEIGHT));
		glVertex2f(GRID_X+i*JEWEL_DIM,
							(GRID_Y-25*GRID_HEIGHT)+GRID_HEIGHT*JEWEL_DIM);

		glEnd();
	}

	g->displayGrid(100, 150);

	////////////////
	// END DRAWING
	////////////////

	glPopMatrix();
	SDL_GL_SwapBuffers();
	SDL_Delay(33); // frame rate 30ms
	return;
}