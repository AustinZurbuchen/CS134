#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<GL/glew.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include"DrawUtils.h"
//#include"bgTile.h"
//#include"bgTile.cpp"

// Set this to true to make the game loop exit.
char shouldExit = 0;

// The previous frame's keyboard state.
unsigned char kbPrevState[SDL_NUM_SCANCODES] = {0};

// The current frame's keyboard state.
const unsigned char* kbState = NULL;

// Position of the sprite.
int spritePos[2] = {10, 10};

// Texture for the sprite.
GLuint spriteTex;
GLuint striteTexRight1;
GLuint striteTexRight2;
GLuint striteTexRight3;
GLuint striteTexLeft1;
GLuint striteTexLeft2;
GLuint striteTexLeft3;
GLuint spriteDirt;
GLuint spriteMagma;
GLuint currentImage;

// Texture Arrays
GLuint rightArray[3];
GLuint leftArray[3];

// Size of the sprite.
int spriteSize[2];

int main(void)
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Could not initialize SDL. ErrorCode=%s\n", SDL_GetError());
        return 1;
    }

    // Create the window and OpenGL context.
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Window* window = SDL_CreateWindow(
            "SDLTemplate",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            800, 600,
            SDL_WINDOW_OPENGL);
    if (!window) {
        fprintf(stderr, "Could not create window. ErrorCode=%s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_GL_CreateContext(window);

    // Make sure we have a recent version of OpenGL.
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        fprintf(stderr, "Could not initialize glew. ErrorCode=%s\n", glewGetErrorString(glewError));
        SDL_Quit();
        return 1;
    }
    if (GLEW_VERSION_2_0) {
        fprintf(stderr, "OpenGL 2.0 or greater supported: Version=%s\n",
                 glGetString(GL_VERSION));
    } else {
        fprintf(stderr, "OpenGL max supported version is too low.\n");
        SDL_Quit();
        return 1;
    }

    // Setup OpenGL state.
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, 800, 600, 0, 0, 100);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Game initialization goes here.
	spriteDirt = glTexImageTGAFile("minecraft_dirt.tga", &spriteSize[0], &spriteSize[1]);
	spriteMagma = glTexImageTGAFile("minecraft_magma.tga", &spriteSize[0], &spriteSize[1]);
	spriteTex = glTexImageTGAFile("MegamanSprite.tga", &spriteSize[0], &spriteSize[1]);
	striteTexRight1 = glTexImageTGAFile("MegamanRightFrame1.tga", &spriteSize[0], &spriteSize[1]);
	striteTexRight2 = glTexImageTGAFile("MegamanRightFrame2.tga", &spriteSize[0], &spriteSize[1]);
	striteTexRight3 = glTexImageTGAFile("MegamanRightFrame3.tga", &spriteSize[0], &spriteSize[1]);
	striteTexLeft1 = glTexImageTGAFile("MegamanLeftFrame1.tga", &spriteSize[0], &spriteSize[1]);
	striteTexLeft2 = glTexImageTGAFile("MegamanLeftFrame2.tga", &spriteSize[0], &spriteSize[1]);
	striteTexLeft3 = glTexImageTGAFile("MegamanLeftFrame3.tga", &spriteSize[0], &spriteSize[1]);

	// assign images to arrays
	rightArray[0] = striteTexRight1;
	rightArray[1] = striteTexRight2;
	rightArray[2] = striteTexRight3;

	leftArray[0] = striteTexLeft1;
	leftArray[1] = striteTexLeft2;
	leftArray[2] = striteTexLeft3;

    // Setting up variables
	kbState = SDL_GetKeyboardState(NULL);
	long x = 385;
	long y = 288;
	int bx = 0;
	int by = 0;
	int counter = 0;
	int direction = 2;
	int currentFrameTime = SDL_GetTicks();
	int deltaFrameTime = 0;
	int pastFrameTime = 0;
	double elapsedTime = 0;
	int startTileX = 0;
	int startTileY = 0;

	struct bgTile {
		long x, y;
		GLuint filename;
	}bg[40][40];

	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			if (i % 2 == 0) {
				bg[i][j].x = (bx + j * spriteSize[0]);
				bg[i][j].y = (by + i * spriteSize[1]);
				bg[i][j].filename = spriteDirt;
			}
			else {
				bg[i][j].x = (bx + j * spriteSize[0]);
				bg[i][j].y = (by + i * spriteSize[1]);
				bg[i][j].filename = spriteMagma;
			}
		}
	}

	// The game loop.
	while (!shouldExit) {
        assert(glGetError() == GL_NO_ERROR);
        memcpy(kbPrevState, kbState, sizeof(kbPrevState));
        
        // Handle OS message pump.
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    shouldExit = 1;
            }
        }

        // Game logic goes here.
		currentFrameTime = SDL_GetTicks();
		deltaFrameTime = currentFrameTime - pastFrameTime;
        if (kbState[SDL_SCANCODE_ESCAPE]) {
            shouldExit = 1;
        }
		if (kbState[SDL_SCANCODE_W]) {
			if (elapsedTime > 100) {
				counter++;
				elapsedTime = 0;
			}
			direction = 0;
			if (y >= 151) { //1
				y -= (1 * deltaFrameTime) / 2;
			}
			else {
				//y = 150; //0
				y -= (1 * deltaFrameTime) / 5;
				if (by < -1) { //-1
					by += (1 * deltaFrameTime) / 2;
				}
				else {
					by = 0; //0
					if (y >= 1) { //1
						y -= (1 * deltaFrameTime) / 2;
					}
					else
						y = 0;
				}
			}
		}
		if (kbState[SDL_SCANCODE_A]) {
			if (elapsedTime > 100) {
				counter++;
				elapsedTime = 0;
			}
			direction = 1;
			if (x >= 151) { //1
				x -= (1 * deltaFrameTime) / 2;
			}
			else {
				//x = 150; //0
				x -= (1 * deltaFrameTime) / 5;
				if (bx < -1) { //-1
					bx += (1 * deltaFrameTime) / 2;
				}
				else {
					bx = 0; //0
					if (x >= 1) { //1
						x -= (1 * deltaFrameTime) / 2;
					}
					else
						x = 0;
				}
			}
		}
		if (kbState[SDL_SCANCODE_S]) {
			if (elapsedTime > 100) {
				counter++;
				elapsedTime = 0;
			}
			direction = 1;
			if (y <= 425) { //575
				y += (1 * deltaFrameTime) / 2;
			}
			else {
				//y = 426; //576
				y += (1 * deltaFrameTime) / 5;
				if (by >= -359) { //-359
					by -= (1 * deltaFrameTime) / 2;
				}
				else {
					by = -360; //-360
					if (y <= 575) { //575
						y += (1 * deltaFrameTime) / 2;
					}
					else
						y = 576;
				}
			}
		}
		if (kbState[SDL_SCANCODE_D]) {
			if (elapsedTime > 100) {
				counter++;
				elapsedTime = 0;
			}
			direction = 0;
			if (x <= 620) { //770
				x += (1 * deltaFrameTime) / 2;
			}
			else {
				//x = 621; //771
				x += (1 * deltaFrameTime) / 5;
				if (bx >= -359) { //-359
					bx -= (1 * deltaFrameTime) / 2;
				}
				else {
					bx = -360; //-360
					if (x <= 770) { //770
						x += (1 * deltaFrameTime) / 2;
					}
					else
						x = 770;
				}
			}
		}

		if (counter > 2) {
			counter = 0;
		}
		else
			if (elapsedTime > 100) {
				counter++;
				elapsedTime = 0;
			}
		if (direction == 0) {
			currentImage = rightArray[counter];
		}
		else if (direction == 1) {
			currentImage = leftArray[counter];
		}
		else
			currentImage = spriteTex;

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

		// Game drawing goes here.
		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 40; j++) {
				if (i % 2 == 0) {
					bg[i][j].x = (bx + j * spriteSize[0]);
					bg[i][j].y = (by + i * spriteSize[1]);
					bg[i][j].filename = spriteDirt;
				}
				else {
					bg[i][j].x = (bx + j * spriteSize[0]);
					bg[i][j].y = (by + i * spriteSize[1]);
					bg[i][j].filename = spriteMagma;
				}
			}
		}

		startTileX = floor((bx * -1) / spriteSize[0]);
		startTileY = floor((by * -1) / spriteSize[1]);
		for (int a = startTileY; a < (startTileY + 28); a++) {
			for (int b = startTileX; b < (startTileX + 30); b++) {
				glDrawSprite(bg[a][b].filename, bg[a][b].x, bg[a][b].y, spriteSize[0], spriteSize[1]);
			}
		}

		/*for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 40; j++) {
				if ((-1 * spriteSize[0]) < bg[i][j].x < 800 && (-1 * spriteSize[1]) < bg[i][j].y < 600) {
					glDrawSprite(bg[i][j].filename, bg[i][j].x, bg[i][j].y, spriteSize[0], spriteSize[1]);
				}
			}
		}*/

        glDrawSprite(currentImage, x, y, spriteSize[0], spriteSize[1]);
		direction = 2;

        // Present the most recent frame.
        SDL_GL_SwapWindow(window);

		pastFrameTime = currentFrameTime;
		elapsedTime += (1 + deltaFrameTime);
    }

    SDL_Quit();

    return 0;
}
