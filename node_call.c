

typedef struct {
	call_node* np;
	int col;
	int row;
} call_node_istate;

char* iterateCallNode(void* vp) {
	call_node_istate* sp = (call_node_istate*) vp;
	char* res = VikGetp(sp->np->inputs, sp->row * 8);
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
	np->call.inputs = VikNew(call_input);
	np->call.fsig_index = fi;
	for (int i = 0; i < argc; i++) {
		call_input* cip = VikExp(np->call.inputs);
		cip->tag = Callinp_none;
	}
}

void keybCallNode(node_t* np, SDL_Event* evp) {
	char c = evp->key.keysym.sym;
	if (evp->key.keysym.sym == SDLK_ESCAPE) return;
	if (c == 'n') {
		VikDrop(np->call.inputs);
		callNodeSetFsig(np, np->call.fsig_index + 1);
	} else if (c == 'j') {
		np->call.srow++;
	} else if (c == 'k') {
		np->call.srow--;
	} else {
		call_input* cip = VikGetp(np->call.inputs, np->call.srow);
		if (cip->tag == Callinp_const) {
			if (c >= '0' && c <= '9') {
				u8* hp = VikExp(cip->con.text);
				*hp = c - '0';
			} else if (c >= 'a' && c <= 'f') {
				u8* hp = VikExp(cip->con.text);
				*hp = c - 'a' + 0xa;
			} else {
				VikShrink(cip->con.text);
			}
		}
	}
}

void initCallNode(node_t* np) {
	np->ni.tag = Tag_node_call;
	np->call.scol = 0;
	np->call.srow = 0;
	callNodeSetFsig(np, 0);
}
void freeCallNode(node_t* np) {}

float gethCallNode(node_t* np) {
	return gethTable(fsig_list[np->call.fsig_index].args->c);
}
