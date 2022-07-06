

void drawStemNode(node_t* np, camact_t ca) {
	float points[] = {
//		0.0,-15.0,
//		13.0,-7.5,
//		13.0,7.5,
//		0.0,15.0,
//		-13.0,7.5,
//		-13.0,-7.5
		 7.0,  7.0,  5.0, 12.0,
		 0.0, 14.0, -5.0, 12.0,
		-7.0,  7.0, -5.0,  2.0,
		 0.0,  0.0,  5.0,  2.0
	};
	drawWithCamact(
		sizeof(points) / sizeof(points[0]), points,
		GL_TRIANGLE_FAN, 0.3, 0.4, 0.4, ca);
//	for (int i = 0; i < sizeof(points) / sizeof(points[0]); ++i)
//		points[i] = points[i] * 7 / 8;
//	drawWithCamact(
//		sizeof(points) / sizeof(points[0]), points,
//		GL_TRIANGLE_FAN, 1.0, 1.0, 1.0, ca);
}

void freeStemNode(node_t* np) {
	free(np->stem.text);
}

void keybStemNode(node_t* np, SDL_Event* evp) {
	char c = (char) evp->key.keysym.sym;
	if (c == 'f') {
		freeStemNode(np);
		initFsigNode(np);
	}
	else if (c == 'c') {
		freeStemNode(np);
		initCallNode(np);
	}
//	np->stem.text = realloc(np->stem.text, np->stem.tlen + 1);
//	np->stem.text[np->stem.tlen] = c;
//	++np->stem.tlen;
}

void initStemNode(node_t* np) {
	np->ni.tag = Tag_node_stem;
	np->stem.text = NULL;
	np->stem.tlen = 0;
}


float gethStemNode(node_t* np) {
	return 14.0;
}
