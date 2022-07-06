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


#include "node.c"
#include "node_stem.c"
#include "node_fsig.c"
#include "node_call.c"




void (*af_draw[])(node_t*, camact_t ca) = {
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
float (*af_geth[])(node_t*) = {
	gethStemNode, gethFsigNode, gethCallNode
};


#define HandleEvent() handle_event(&event, nodes, &ca, &selnode, &nodec, &focus)
char handle_event(
	SDL_Event* evp, node_t* nodes, camact_t* cap,
	int* snp, int* ncp, char* focusp
) {
	switch (evp->type) {
	case SDL_QUIT:
		return 1;
		break;
	case SDL_MOUSEWHEEL:
		cap->camx -= 0.5 * evp->wheel.x;
		cap->camy += 0.5 * evp->wheel.y;
		break;
	case SDL_KEYDOWN:
		if (evp->key.keysym.sym == SDLK_ESCAPE) {
			*focusp = 0;
			break;
		}
		if (*focusp) {
			(af_keyb[nodes[*snp].ni.tag])
			(&nodes[*snp], evp);
			break;
		}
		if (evp->key.keysym.sym == 'i') {
			*focusp = 1;
		}
		else if (evp->key.keysym.sym == 'k') {
			if (nodes[*snp].ni.prev_node == -1) {
				nodes[*ncp].ni.next_node
				= *snp;
				nodes[*ncp].ni.prev_node = -1;
				initStemNode(&nodes[*ncp]);
				nodes[*snp].ni.prev_node
				= *ncp;
				++*ncp;
			}
			*snp = nodes[*snp].ni.prev_node;
			//ca.camy -= 0;
		}
		else if (evp->key.keysym.sym == 'j') {
			if (nodes[*snp].ni.next_node == -1) {
				nodes[*ncp].ni.prev_node
				= *snp;
				nodes[*ncp].ni.next_node = -1;
				initStemNode(&nodes[*ncp]);
				nodes[*snp].ni.next_node
				= *ncp;
				++*ncp;
			}
			*snp = nodes[*snp].ni.next_node;
			//ca.camy += 0;
		}
		else {
			(af_keyb[nodes[*snp].ni.tag])
			(&nodes[*snp], evp);
		}
		break;
	}
	return 0;
}

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
	{
		fsig_t* fsp = createFsigGlobal("hello");
		addFsigRow(fsp, 2, "s32", "x");
		addFsigRow(fsp, 1, "s32", "y");
	}
	{
		fsig_t* fsp = createFsigGlobal("set");
		addFsigRow(fsp, 1, "u64", "x");
		addFsigRow(fsp, 2, "u64", "c");
	}
	{
		fsig_t* fsp = createFsigGlobal("add");
		addFsigRow(fsp, 1, "u64", "z");
		addFsigRow(fsp, 2, "u64", "x");
		addFsigRow(fsp, 2, "u64", "y");
	}
	{
		fsig_t* fsp = createFsigGlobal("print");
		addFsigRow(fsp, 2, "u64", "x");
	}
	int selnode = 0;
	char focus = 0;
	node_t nodes[32];
	int nodec = 1;
	nodes[0].ni.prev_node = -1;
	nodes[0].ni.next_node = -1;
	nodes[0].ni.x = 0;
	nodes[0].ni.y = 0;
	initStemNode(&nodes[0]);
	for (;;) {
		SDL_Event event;
		SDL_WaitEvent(&event);
		if (HandleEvent()) goto exit;
		while (SDL_PollEvent(&event))
			if (HandleEvent()) goto exit;
		{
			int w, h;
				SDL_GetWindowSize(winp, &w, &h);
				glViewport(0, 0, w, h);
			ca.ww = w; ca.wh = h;
		}
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float cay = ca.camy;
		for (int i = selnode; i != -1; i = nodes[i].ni.prev_node) {
			u8 t = nodes[i].ni.tag;
			(af_draw[t])(&nodes[i], ca);
			ca.camy += (af_geth[t])(&nodes[i]) + 1.0;
		}
		ca.camy = cay;
		for (int i = nodes[selnode].ni.next_node;
		i != -1; i = nodes[i].ni.next_node) {
			u8 t = nodes[i].ni.tag;
			ca.camy -= (af_geth[t])(&nodes[i]) + 1.0;
			(af_draw[t])(&nodes[i], ca);
		}
		ca.camy = cay;
		SDL_GL_SwapWindow(winp);
		SDL_Delay(16);
	}
exit:
	SDL_GL_DeleteContext(conp);
	SDL_Quit();
	return 0;
}
