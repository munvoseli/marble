
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

void fillGlyph(unsigned char g, float* points) {
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
				0.1, 0.2, 0.2, ca);
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
	drawWithCamact(8, vertdata, GL_TRIANGLE_STRIP, 0.1, 0.2, 0.2, ca);
}


void drawTable(float tx, float ty, int tw, int th, char* title, char* boxes, camact_t ca) {
	drawString(title, tx, ty, ca);
	int si = 0;
	int* widths = malloc(sizeof(int) * tw);
	memset(widths, 0, sizeof(int) * tw);
	for (int y = 0; y < th; ++y)
	for (int x = 0; x < tw; ++x) {
		int l = 1;
		while (boxes[si]) {
			++si; ++l;
		}
		if (l > widths[x]) widths[x] = l;
		++si;
	}
	{
		int sum = tw - 1;
		for (int i = 0; i < tw; ++i) {
			if (i != tw - 1)
				drawRect(tx + sum * 5, ty - 7, tx + sum * 5 + 1, ty - 7 * (th + 1) - 5, ca);
			sum += widths[i];
		}
		int si = 0;
		while (title[si]) ++si;
		if (si > sum) sum = si;
		drawRect(tx - 3, ty - 6, tx + si * 5 - 2, ty - 7, ca);
	}
	si = 0;
	for (int y = 0; y < th; ++y) {
	int xo = 0;
	for (int x = 0; x < tw; ++x) {
		drawString(&boxes[si], 5.0 * xo + tx, -11.0 * (y + 1) + ty - 2, ca);
		xo += widths[x];
		while (boxes[si]) ++si; ++si;
	}}
	free(widths);
}

void drawTableIter(float tx, float ty, int tw, int th, char* title,
		char* (*iterf)(void*), void (*iterreset)(void*), void* iterstate,
		camact_t ca) {
	drawString(title, tx, ty, ca);
	int* widths = malloc(sizeof(int) * tw);
	memset(widths, 0, sizeof(int) * tw);
	for (int y = 0; y < th; ++y)
	for (int x = 0; x < tw; ++x) {
		int l = 1;
		int si = 0;
		char* celltext = iterf(iterstate);
		while (celltext[si]) {
			++si; ++l;
		}
		if (l > widths[x]) widths[x] = l;
	}
	iterreset(iterstate);
	{
		int sum = 0;
		float y0a = ty + 7;
		float y0b = y0a - 1;
		float y2a = ty - 6;
		float y2b = y2a - 1;
		float y4a = ty - 8 - 11 * th;
		float y4b = y4a - 1;
		drawRect(
			tx + -5.5,
			y0b,
			tx + -4.5,
			y4a, ca);
		for (int i = 0; i < tw; ++i) {
			sum += widths[i];
			drawRect(
				tx + sum * 5 - 5.5,
				y2b,
				tx + sum * 5 - 4.5,
				y4a, ca);
		}
		drawRect(
			tx + sum * 5 - 5.5,
			y0b,
			tx + sum * 5 - 4.5,
			y2b, ca);
		int si = 0;
		while (title[si]) ++si;
		if (si > sum) sum = si;
		drawRect(tx - 4.5, ty + 6, tx + sum * 5 - 5.5, ty + 7, ca);
		drawRect(tx - 4.5, ty - 6, tx + sum * 5 - 5.5, ty - 7, ca);
		drawRect(
			tx - 4.5,
			y4a,
			tx + sum * 5 - 5.5,
			y4b, ca);
	}
	for (int y = 0; y < th; ++y) {
	int xo = 0;
	for (int x = 0; x < tw; ++x) {
		char* celltext = iterf(iterstate);
		drawString(celltext, 5.0 * xo + tx, -11.0 * (y + 1) + ty - 2, ca);
		xo += widths[x];
	}}
	free(widths);
}



//void drawHex(float x, float y, camact_t) {
	
//}
