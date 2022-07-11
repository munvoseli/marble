// gcc main.c -lSDL2 -lGL

// snes: 2**24 bytes
// nes: 2**16 bytes
// 4 kbby: 2**12 bytes


#include <stdlib.h>
#include <stdio.h>
#include <string.h> // finally caved and used this

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef char s8;
typedef short s16;
typedef int s32;
typedef long s64;


#include "vector.c"
#include "gl2d.c"
#include "draw.c"

#include "data.h"


#include "node.c"
#include "node_stem.c"
#include "node_fsig.c"
#include "node_call.c"
#include "node_bi.c"




void (*af_draw[])(node_t*, camact_t ca) = {
	drawStemNode, drawFsigNode, drawCallNode, drawBiNode
};
void (*af_keyb[])(node_t*, SDL_Event* evp) = {
	keybStemNode, keybFsigNode, keybCallNode, keybBiNode
};
void (*af_init[])(node_t*) = {
	initStemNode, initFsigNode, initCallNode, initBiNode
};
void (*af_free[])(node_t*) = {
	freeStemNode, freeFsigNode, freeCallNode, freeBiNode
};
float (*af_geth[])(node_t*) = {
	gethStemNode, gethFsigNode, gethCallNode, gethBiNode
};

/*
// needs to be updated for multiple-character names
string_list listVariables(int i0, node_t* nodes) {
	string_list sl = newStringList();
	for (int i = i0; i >= 0; i = nodes[i].ni.next_node) {
		printf("h %d\n", i);
		if (nodes[i].ni.tag != Tag_node_call) continue;
		for (int j = 0;
			j < fsig_list[nodes[i].call.fsig_index].args->c; ++j
		) {
			int fi = nodes[i].call.fsig_index;
			if (VikGett(fsig_list[fi].args, j, fsigparam_t)->rw
				== 4) continue;
			unnSzToList(&sl, &(nodes[i].call.string_arr[j * 8]));
		}
	}
	return sl;
}*/

void compileBlock(
	int i0, node_t* nodes, // nodes[i0] should be block info node
	viktor vars // <>var_data
) {
	int parvarc = vars->c;
	int psiz = sizeof(void*);
	FILE* fp = fopen("out.asm", "w");
	for (int i = i0; i >= 0; i = nodes[i].ni.next_node) {
		if (nodes[i].ni.tag != Tag_node_call)
			printf("Parse error: only call allowed for now\n");
		u32 fi = nodes[i].call.fsig_index;
		u32 c = fsig_list[fi].args->c;
		int* offsets = malloc(c * sizeof(int));
		u8* isconst = malloc(c * sizeof(u8));
		for (int j = 0; j < c; ++j) {
			call_input* cip = VikGetp(nodes[i].call.inputs, j);
			if (cip->tag == Callinp_none) {
				printf("Error: empty spot in func call\n");
				fclose(fp);
				return;
			} else if (cip->tag == Callinp_const) {
				isconst[j] = 1;
			} else if (cip->tag == Callinp_var) {
				isconst[j] = 0;
				offsets[j] = cip->var.id * 8;
			}
		}
		switch (fi) {
		case 1: // set; assume var and const for now
			fprintf(fp, "mov [fmem+%d], dword 0x", offsets[0]);
			for (int j = 0; j < nodes[i].call.inputs->c; ++j) {
				u8* hp = VikGetp(nodes[i].call.inputs, j);
				if (*hp < 0xa) fputc(*hp + 0x30, fp);
				else fputc(*hp - 0xa + 'a', fp);
			}
			fputc(10, fp);
			break;
		case 2: // add, assume var and var
			fprintf(fp, "mov rcx, [fmem+%d]\n", offsets[1]);
			fprintf(fp, "mov rdx, [fmem+%d]\n", offsets[2]);
			fprintf(fp, "add rcx, rdx\n");
			fprintf(fp, "mov [fmem+%d], rdx\n\n", offsets[0]);
			break;
		case 3: // print char; assume var
			fprintf(fp, "mov rax, 1\n");
			fprintf(fp, "mov rdi, 1\n");
			fprintf(fp, "mov rdx, 1\n");
			fprintf(fp, "mov rsi, fmem+%d\n", offsets[0]);
			fprintf(fp, "syscall\n\n");
			break;
		}
		free(offsets); free(isconst);
	}
	fclose(fp);
}
/*
// needs to be updated for multiple-character names
void compileFunc(int i0, node_t* nodes) {
	int psiz = sizeof(void*);
	string_list vars = listVariables(i0, nodes);
	int bytespace = vars.c * psiz;
	FILE* fp = fopen("out.asm", "w");
	fprintf(fp, "segment .data\nglobal _start\nsegment .text\n_start:\n");
	for (int i = i0; i >= 0; i = nodes[i].ni.next_node) {
		u32 fi = nodes[i].call.fsig_index;
		int c = fsig_list[fi].args->c;
		int* offsets = (int*) malloc(c * sizeof(int));
		for (int j = 0; j < c; ++j) {
			if (VikGett(fsig_list[fi].args, j, fsigparam_t)->rw
				== 4) continue;
			char* vname = &nodes[i].call.string_arr[j * 8];
			offsets[j] = findInStringList(&vars, vname) * 8;
		}
		printf("%d\n", fi);
		if (fi == 1) { // set to const
			fprintf(
				fp, "mov [fmem+%d], dword %d\n\n",
				offsets[0],
				strtol(&nodes[i].call.string_arr[8], NULL, 16)
			);
		} else if (fi == 2) { // add
			fprintf(fp, "mov rcx, [fmem+%d]\n", offsets[1]);
			fprintf(fp, "mov rdx, [fmem+%d]\n", offsets[2]);
			fprintf(fp, "add rcx, rdx\n");
			fprintf(fp, "mov [fmem+%d], rdx\n\n", offsets[0]);
		} else if (fi == 3) { // print
			fprintf(fp, "mov rcx, [fmem+%d]\n", offsets[0]);
			fprintf(fp, "mov rax, 1\n");
			fprintf(fp, "mov rdi, 1\n");
			fprintf(fp, "mov rdx, 1\n");
			fprintf(fp, "mov rsi, fmem+%d\n", offsets[0]);
			fprintf(fp, "syscall\n\n");
		}
		free(offsets);
	}
	fprintf(fp, "mov eax, 0x3c\nmov edi, 0\nsyscall\n");
	fprintf(fp, "segment .bss\nfmem:\nresb %d\n", bytespace);
	fclose(fp);
}*/

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
		if (evp->key.keysym.sym == SDLK_ESCAPE && *focusp == 1) {
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
		else if (evp->key.keysym.sym == 'l') {
			//compileFunc(0, nodes);
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
		addFsigRow(fsp, 4, "u64", "c");
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
