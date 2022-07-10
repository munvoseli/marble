

typedef struct {
	call_node* np;
	int col;
	int row;
} call_node_istate;

char* iterateCallNode(void* vp) {
	call_node_istate* sp = (call_node_istate*) vp;
	char* res = &sp->np->string_arr[sp->row * 8];
	++sp->row;
	return res;
}

void iterresetCallNode(void* vp) {
	call_node_istate* sp = (call_node_istate*) vp;
	sp->col = 0;
	sp->row = 0;
}

void drawCallNode(node_t* vp, camact_t ca) {
	call_node* np = (call_node*) vp;
	call_node_istate iter;
	iter.np = np;
	iterresetCallNode(&iter);
	drawTableIter(np->scol, np->srow,
		1, fsig_list[np->fsig_index].args->c,
		fsig_list[np->fsig_index].name,
		iterateCallNode, iterresetCallNode, &iter, ca);
}

void callNodeSetFsig(node_t* np, int fi) {
	int argc = fsig_list[fi].args->c;
	np->call.string_arr = (char*) realloc(np->call.string_arr, 8 * argc);
	np->call.fsig_index = fi;
	for (int i = 0; i < argc * 8; i += 8) {
		np->call.string_arr[i    ] = 'h';
		np->call.string_arr[i + 1] = 0;
	}
}

void keybCallNode(node_t* np, SDL_Event* evp) {
	char c = evp->key.keysym.sym;
	if (evp->key.keysym.sym == SDLK_ESCAPE) return;
	if (c == 'n') {
		callNodeSetFsig(np, np->call.fsig_index + 1);
	} else if (c == 'j') {
		np->call.srow++;
	} else if (c == 'k') {
		np->call.srow--;
	} else {
		np->call.string_arr[np->call.srow * 8] = c;
	}
}

void initCallNode(node_t* np) {
	np->ni.tag = Tag_node_call;
	np->call.scol = 0;
	np->call.srow = 0;
	np->call.string_arr = NULL;
	callNodeSetFsig(np, 0);
}
void freeCallNode(node_t* np) {}

float gethCallNode(node_t* np) {
	return gethTable(fsig_list[np->call.fsig_index].args->c);
}
