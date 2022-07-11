
int getTriCount(unsigned char g) {
	int c = 0;
	static const int b[] = {0, 5, 3, 2};
	c += b[g & 3];
	c += b[(g >> 2) & 3];
	c += 2;
	c += 1 - ((g >> 5) & 1);
	c += (g >> 6) & 1;
	return c;
}

void fillGlyph(unsigned char g, float* points) { // 4.0 x 8.0
	int i = 0;
	static const float gly_0[] = {};
	static const float gly_1[] = {
		-1,-1 , -1,-2 , -2,-3,
		-1,-2 , -1,-3 , -2,-3,
		-2,-5 , -1,-3 , -2,-3,
		-1,-2 , -1,-3 ,  1,-2,
		 2,-3 , -1,-3 ,  1,-2,
	};
	static const float gly_2[] = {
		-2,-2 , 0,-3 , 0,-4,
		 1,-2 , 0,-3 , 0,-4,
		 1,-2 , 2,-3 , 0,-4,
	};
	static const float gly_3[] = {
		//-1,-3 , 0,-3 , 1,-5,
		//-1,-3 , 0,-3 , 0,-2,
		//1,-2 , 0,-3 , 0,-2,
		//1,-2 , 0,-3 , 2,-3
		//1,-2 , 0,-3 , 2,-3,
		//0,-5 , 0,-3 , 2,-3,
		//-1,-2 , 1,-2 , 0,-3
		0,-3 , 1,-4 , -1,-4,
		0,-5 , 1,-4 , -1,-4
	};
	static const float gly_bar[] = {
		1,-2 , 1,2 ,  2,-3,
		2,3  , 1,2 ,  2,-3,
	};
	static const float gly_mt[] = {
		-0.5,0.5 , 0.5,1 ,  -0.5,1.5,
	};
	static const float gly_mb[] = {
		-0.5,-0.5 , 0.5,-1 ,  -0.5,-1.5,
	};
	const float* gly_arr[] = {
		gly_0, gly_1, gly_2, gly_3
	};
	const int gly_nums[] = {
		sizeof gly_0 / sizeof(float),
		sizeof gly_1 / sizeof(float),
		sizeof gly_2 / sizeof(float),
		sizeof gly_3 / sizeof(float),
	};
	for (int j = 0; j < gly_nums[g & 3]; ++j) {
		points[i++] = gly_arr[g & 3][j];
	}
	{
		int gp = (g >> 2) & 3;
		for (int j = 0; j < gly_nums[gp];) {
			points[i++] = gly_arr[gp][j++];
			points[i++] = -gly_arr[gp][j++];
		}
	}
	for (int j = 0; j < 12; ++j)
		points[i++] = gly_bar[j];
	if (g & 0x10)
		for (int j = 0; j < i; j += 2)
			points[j] *= -1;
	if (!(g & 0x20))
		for (int j = 0; j < 6; j++)
			points[i++] = gly_mb[j];
	if (g & 0x40)
		for (int j = 0; j < 6; j++)
			points[i++] = gly_mt[j];
}

void drawViktorHex(viktor v, float x, float y, camact_t ca) {
	ca.camx += x;
	ca.camy += y;
	float vertdata[4 * 3 * sizeof(float) * 2];
	for (int i = 0; i < v->c; ++i) {
		u8* hp = VikGetp(v, i);
		int vi = 0;
		int tric = 0;
		if (*hp & 8) {
			vertdata[vi++] = 0.0; vertdata[vi++] = 0.0;
			vertdata[vi++] = 0.0; vertdata[vi++] = 4.0;
			vertdata[vi++] = 2.0; vertdata[vi++] = 2.0;
			++tric;
		};
		if (*hp & 4) {
			vertdata[vi++] = 0.0; vertdata[vi++] = 4.0;
			vertdata[vi++] = 0.0; vertdata[vi++] = 8.0;
			vertdata[vi++] = 2.0; vertdata[vi++] = 6.0;
			++tric;
		};
		if (*hp & 2) {
			vertdata[vi++] = 2.0; vertdata[vi++] = 6.0;
			vertdata[vi++] = 0.0; vertdata[vi++] = 4.0;
			vertdata[vi++] = 2.0; vertdata[vi++] = 2.0;
			++tric;
		};
		if (*hp & 1) {
			vertdata[vi++] = 2.0; vertdata[vi++] = 6.0;
			vertdata[vi++] = 4.0; vertdata[vi++] = 4.0;
			vertdata[vi++] = 2.0; vertdata[vi++] = 2.0;
			++tric;
		};
		int compc = tric * 6;
		for (int j = 0; j < compc; j += 2)
			vertdata[j] += 4 * i;
		drawWithCamact(compc, vertdata, GL_TRIANGLES,
			1.0, 0.8, 0.0, ca);
	}
}

void drawString(const char* str, float x, float y, camact_t ca) {
	float* vertdata = NULL;
	ca.camx += x;
	ca.camy += y;
	for (int i = 0; str[i] != 0; ++i) {
		int pan = 6 * getTriCount(str[i]);
		vertdata = realloc(vertdata, pan * sizeof(float));
		fillGlyph(str[i], vertdata);
		for (int j = 0; j < pan; j += 2)
			vertdata[j] += 5 * i;
		drawWithCamact(pan, vertdata, GL_TRIANGLES,
				1.0, 0.8, 0.0, ca);
	}
	free(vertdata);
}

void drawRect(float x0, float y0, float x2, float y2, camact_t ca) {
	float vertdata[8];
	vertdata[0] = x0;
	vertdata[1] = y0;
	vertdata[2] = x2;
	vertdata[3] = y0;
	vertdata[4] = x0;
	vertdata[5] = y2;
	vertdata[6] = x2;
	vertdata[7] = y2;
	drawWithCamact(8, vertdata, GL_TRIANGLE_STRIP, 0.8, 0.4, 0.0, ca);
}

void drawTableIter(int hix, int hiy, int tw, int th, char* title,
		char* (*iterf)(void*), void (*iterreset)(void*), void* iterstate,
		camact_t ca) {
	ca.camy += 11.0 * th + 9.0;
	drawString(title, 4.0, 0, ca);
	int* widths = malloc(sizeof(int) * (tw + 1));
		memset(widths, 0, sizeof(int) * tw);
		for (int y = 0; y < th; ++y)
		for (int x = 0; x < tw; ++x) {
		int l = 0;
		int si = 0;
		char* celltext = iterf(iterstate);
		while (celltext[si]) {
			++si; ++l;
		}
		if (l > widths[x]) widths[x] = l;
		}
	int bodycc = 0;
		for (int i = 0; i < tw; ++i)
		bodycc += widths[i];
	int titlecc = 0;
		for (; title[titlecc]; ++titlecc);
	float* xstr = (float*) widths; // assumes that sizeof f32 == s32
	iterreset(iterstate);
	{
		float x0a = 0;
		float x0b = x0a + 1.0;
		float x2 = x0a;
		float x2atop = x0a + titlecc * 5.0 + 2.0;
		float y0a = 7;
		float y0b = y0a - 1;
		float y2a = -6;
		float y2b = y2a - 1;
		float y4a = -8 - 11 * th;
		float y4b = y4a - 1;
		// bars to left of cols
		for (int i = 0; i < tw; ++i) {
			int w = widths[i];
			xstr[i] = x2 + 4.0;
			drawRect(x2, y2b, x2 + 1.0, y4a, ca);
			x2 += w * 5.0;
			x2 += 2.0;
		}
		xstr[tw] = x2 + 4.0;
		// bar to left of title
		drawRect(x0a, y0b, x0b, y2a, ca);
		// bar to right of title
		drawRect(x2atop, y0b, x2atop + 1.0, y2b, ca);
		// bar to right of cols
		drawRect(x2, y2b, x2 + 1.0, y4b, ca);
		// top bar
		drawRect(x0a,   6, x2atop + 1.0,   7, ca);
		// middle bar
		drawRect(x0a,  -6, (x2 > x2atop ? x2 : x2atop) + 1.0,  -7, ca);
		// bottom bar
		drawRect(x0a, y4a, x2 + 1.0, y4b, ca);
		if (hix != -1) {
			drawRect(
				xstr[hix] - 2.0,     y2b - 11.0 * hiy -  1.0,
				xstr[hix + 1] - 5.0, y2b - 11.0 * hiy - 11.0,
				ca
			);
		}
	}
	{
		for (int y = 0; y < th; ++y) {
		for (int x = 0; x < tw; ++x) {
		char* celltext = iterf(iterstate);
		drawString(celltext, xstr[x], -11.0 * (y + 1) - 2, ca);
		}}
	}
	free(widths);
}
float gethTable(int th) {
	return 11.0 * th + 16.0;
}



//void drawHex(float x, float y, camact_t) {
	
//}
