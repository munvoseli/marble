
typedef struct iterFsigState {
	fsig_t* fsigp;
	int row;
	int attr;
} ifs_t;

char* rwstrs[] = { "v", "w", "r", "rw" };
char* iterateFsigCell(void* ifsv) {
	struct iterFsigState* ifs = (struct iterFsigState*) ifsv;
	char* res;
	fsigparam_t* fspp = VikGetp(ifs->fsigp->args, ifs->row);
	switch (ifs->attr) {
	case 0:
		res = rwstrs[fspp->rw];
		break;
	case 1:
		res = fspp->vartype;
		break;
	case 2:
		res = fspp->varname;
		break;
	case 3:
		res = fspp->longname;
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
	drawTableIter(x, y, 4, fsp->args->c, fsp->name, iterateFsigCell, iterresetFsigCell, &ifs, ca);
}

void drawFsigNode(node_t* v, camact_t ca) {
	fsig_node* np = (fsig_node*) v;
	fsig_t* fsp = &fsig_list[np->fsig_index];
	ifs_t ifs;
	ifs.fsigp = fsp;
	ifs.row = 0; ifs.attr = 0;
	drawTableIter(v->fsig.scol, v->fsig.srow, 4, fsp->args->c, fsp->name, iterateFsigCell, iterresetFsigCell, &ifs, ca);
}

void keybFsigNode(node_t* v, SDL_Event* evp) {
	switch (evp->key.keysym.sym) {
	case 'j': ++v->fsig.srow; break;
	case 'k': --v->fsig.srow; break;
	case 'l': ++v->fsig.scol; break;
	case 'h': --v->fsig.scol; break;
	}
}
void freeFsigNode(node_t* v) {}
void initFsigNode(node_t* v) {
	v->ni.tag = Tag_node_fsig;
	v->fsig.fsig_index = 0;
	v->fsig.srow = 0;
	v->fsig.scol = 0;
}

float gethFsigNode(node_t* np) {
	return gethTable(fsig_list[np->fsig.fsig_index].args->c);
}
