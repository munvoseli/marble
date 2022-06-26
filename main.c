// gcc main.c -lSDL2 -lGL

#include "data.h"
#include "gl2d.c"


#include "draw.c"
#include <stdlib.h>



int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Window* winp = SDL_CreateWindow("h", 0, 0, 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext* conp = gl_setup(winp);
	Pavec pavec = gl_shaderSetupUniform();

	camact_t ca;
	ca.pavec = pavec;
	ca.camx = 0;
	ca.camy = 0;
	ca.cams = 0;
//	float camx = 0, camy = 0, cams = 1;
	
	for (;;) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				goto exit;
			} else if (event.type == SDL_MOUSEWHEEL) {
				ca.camx -= 0.1 * event.wheel.x;
				ca.camy += 0.1 * event.wheel.y;
			}
		}
		{
			int w, h;
			SDL_GetWindowSize(winp, &w, &h);
			glViewport(0, 0, w, h);
			ca.ww = w; ca.wh = h;
		}
		glClearColor(0.7f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//char* str = "Hello there.";
		//drawString(str, camx, camy, cams, pavec, w, h);
		drawTable(0, 0, 2, 2, "gerald", "a\0b\0c\0d", ca);
		SDL_GL_SwapWindow(winp);
		SDL_Delay(16);
	}
exit:
	SDL_GL_DeleteContext(conp);
	SDL_Quit();
	return 0;
}
