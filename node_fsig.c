
typedef struct iterFsigState {
	fsig_t* fsigp;
	int row;
	int attr;
} ifs_t;

char* rwstrs[] = { "v", "w", "r", "rw" };
char* iterateFsigCell(void* ifsv) {
	struct iterFsigState* ifs = (struct iterFsigState*) ifsv;
	char* res;
	switch (ifs->attr) {
	case 0:
		res = rwstrs[ifs->fsigp->argv[ifs->row].rw];
		break;
	case 1:
		res = ifs->fsigp->argv[ifs->row].vartype;
		break;
	case 2:
		res = ifs->fsigp->argv[ifs->row].varname;
		break;
	case 3:
		res = ifs->fsigp->argv[ifs->row].longname;
		break;
	}
	++ifs->attr;
	if (ifs->attr == 4) {
		ifs->attr = 0;
		++ifs->row;
	}
	return res;
}

void iterresetFsigCell(void* ifsv) {
	struct iterFsigState* ifsp = (struct iterFsigState*) ifsv;
	ifsp->row = 0; ifsp->attr = 0;
}

void drawFsig(struct fsig_t* fsp, float x, float y, camact_t ca) {
	ifs_t ifs;
	ifs.fsigp = fsp;
	ifs.row = 0; ifs.attr = 0;
	drawTableIter(x, y, 4, fsp->argc, fsp->name, iterateFsigCell, iterresetFsigCell, &ifs, ca);
}

void drawFsigNode(node_t* v, camact_t ca) {
	fsig_node* np = (fsig_node*) v;
	fsig_t* fsp = &fsig_list[np->fsig_index];
	drawFsig(fsp, 0, 0, ca);
}

void keybFsigNode(node_t* v, SDL_Event* evp) {}
void freeFsigNode(node_t* v) {}
void initFsigNode(node_t* v) {
	v->ni.tag = Tag_node_fsig;
	v->fsig.fsig_index = 0;
}

float gethFsigNode(node_t* np) {
	return gethTable(fsig_list[np->fsig.fsig_index].argc);
}
