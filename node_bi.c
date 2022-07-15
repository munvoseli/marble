


void drawBiNode(node_t* np, camact_t ca) {
	drawRect(0.0, 12.0 + 8.0 * np->bi.vars->c, 1.0, 0.0, ca);
	drawRect(
		1.0, 2.0 + 8.0 * (np->bi.vars->c - np->bi.cy),
		2.0, 2.0 + 8.0 * (np->bi.vars->c - np->bi.cy) + 8.0, ca);
	for (int i = 0; i < np->bi.vars->c; ++i) {
		var_data* vdp = VikGetp(np->bi.vars, i);
		drawViktorHex(vdp->name, 2.0, 8.0 * (np->bi.vars->c - i) + 2.0, ca);
	}
}

void keybBiNode(node_t* np, SDL_Event* evp) {
	u8 c = evp->key.keysym.sym & 255;
	switch (evp->key.keysym.sym) {
	case 'j': np->bi.cy++; break;
	case 'k': if (np->bi.cy > 0) np->bi.cy--; break;
	case SDLK_ESCAPE: break;
	default:
		if (np->bi.cy == np->bi.vars->c) {
			var_data* vdp = VikExp(np->bi.vars);
			vdp->name = VikNew(u8);
		}
		var_data* vdp = VikGetp(np->bi.vars, np->bi.cy);
		if (c >= '0' && c <= '9') {
			u8* hp = VikExp(vdp->name);
			*hp = c - '0';
		} else if (c >= 'a' && c <= 'f') {
			u8* hp = VikExp(vdp->name);
			*hp = c - 'a' + 0xa;
		} else {
			VikShrink(vdp->name);
		}
	}
}

void initBiNode(node_t* np) {
	np->ni.tag = Tag_node_bi;
	np->bi.cx = 0;
	np->bi.cy = 0;

	np->bi.fsig = NULL;
	np->bi.firstNode = -1;
	np->bi.vars = VikNew(var_data);
}

void freeBiNode(node_t* np) {

}

float gethBiNode(node_t* np) {
	return 12.0 + 8.0 * np->bi.vars->c;
}
