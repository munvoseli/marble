

int drawStemNode(node_t* np, camact_t ca) {
	float points[] = {
		0.0,-15.0,
		13.0,-7.5,
		13.0,7.5,
		0.0,15.0,
		-13.0,7.5,
		-13.0,-7.5
	};
	drawWithCamact(
		sizeof(points) / sizeof(points[0]), points,
		GL_TRIANGLE_STRIP, 0.0, 0.0, 0.0, ca);
}

void keybStemNode(node_t* np, SDL_Event* evp) {
	char c = (char) evp->key.keysym.sym;
	np->stem.text = realloc(np->stem.text, np->stem.tlen + 1);
	np->stem.text[np->stem.tlen] = c;
	++np->stem.tlen;
}

void initStemNode(node_t* np) {
	np->ni.tag = 0;
	np->stem.text = NULL;
	np->stem.tlen = 0;
}

void freeStemNode(node_t* np) {
	free(np->stem.text);
}
