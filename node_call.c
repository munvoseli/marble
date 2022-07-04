

typedef struct {
	call_node* np;
	int col;
	int row;
} call_node_istate;

char* iterateCallNode(void* vp) {
	call_node_istate* sp = (call_node_istate*) vp;
	char* res = sp->np->string_arr[sp->row];
	++sp->row;
	return res;
}

void iterresetCallNode(void* vp) {
	call_node_istate* sp = (call_node_istate*) vp;
	sp->col = 0;
	sp->row = 0;
}

int drawCallNode(node_t* vp, camact_t ca) {
	call_node* np = (call_node*) vp;
	call_node_istate iter;
	iter.np = np;
	iterresetCallNode(&iter);
	drawTableIter(0, 0, 1, fsig_list[np->fsig_index].argc, "call",
		iterateCallNode, iterresetCallNode, &iter, ca);
}

void keybCallNode(node_t* np, SDL_Event* evp) {}
void initCallNode(node_t* np) {
	np->call.fsig_index = 0;
}
void freeCallNode(node_t* np) {}
