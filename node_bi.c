


void drawBiNode(node_t* np, camact_t ca) {
	for (int i = 0; i < np->bi.vars->c; ++i) {
		var_data* vdp = VikGetp(np->bi.vars, i);
		drawViktorHex(vdp->name, 0, 0, ca);
	}
}

void keybBiNode(node_t* np, SDL_Event* evp) {
	
}

void initBiNode(node_t* np) {
	
}

void freeBiNode(node_t* np) {

}

float gethBiNode(node_t* np) {
	return 8.0;
}
