// gcc main.c -lSDL2 -lGL

// snes: 2**24 bytes
// nes: 2**16 bytes
// 4 kbby: 2**12 bytes

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

#include <stdlib.h>
#include <stdio.h>



#include "gl2d.c"
#include "draw.c"

#include "data.h"

fsig_t fsig_list[32];
int fsigc = 0;

#include "node.c"
#include "node_stem.c"
#include "node_fsig.c"
#include "node_call.c"




int  (*af_draw[])(node_t*, camact_t ca) = {
	drawStemNode, drawFsigNode, drawCallNode
};
void (*af_keyb[])(node_t*, SDL_Event* evp) = {
	keybStemNode, keybFsigNode, keybCallNode
};
void (*af_init[])(node_t*) = {
	initStemNode, initFsigNode, initCallNode
};
void (*af_free[])(node_t*) = {
	freeStemNode, freeFsigNode, freeCallNode
};



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
	ca.cams = 50.0;
	fsig_t* fsp = createFsig("hello");
	node_t nodes[32];
	int nodec = 1;
	nodes[0].ni.prev_node = -1;
	nodes[0].ni.next_node = -1;
	nodes[0].ni.x = 0;
	nodes[0].ni.y = 0;
	initStemNode(&nodes[0]);
	addFsigRow(fsp, 2, "s32", "x");
	addFsigRow(fsp, 1, "s32", "y");
	for (;;) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				goto exit;
			} else if (event.type == SDL_MOUSEWHEEL) {
				ca.camx -= 0.5 * event.wheel.x;
				ca.camy += 0.5 * event.wheel.y;
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
		drawFsig(fsp, 0, 0, ca);
		SDL_GL_SwapWindow(winp);
		SDL_Delay(16);
	}
exit:
	free(fsp);
	SDL_GL_DeleteContext(conp);
	SDL_Quit();
	return 0;
}
