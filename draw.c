
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

void drawString(const char* str, float camx, float camy, float cams, const Pavec pavec, int w, int h) {
	float* vertdata = NULL;
	for (int i = 0; str[i] != 0; ++i) {
		int pan = 6 * getTriCount(str[i]);
		vertdata = realloc(vertdata, pan * sizeof(float));
		fillGlyph(str[i], vertdata);
		for (int j = 0; j < pan; j += 2)
			vertdata[j] += 4 * i;
		drawWithUniform(pan, vertdata, GL_TRIANGLES,
				0.1, 0.0, 0.2,
				camx, camy, 100, 100.0 * h / w,
				pavec);
	}
	free(vertdata);
}
