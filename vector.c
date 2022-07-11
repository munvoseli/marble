struct viktorr {
	int bype;
	int c;
	void* s;
};

typedef struct viktorr* viktor;

viktor newVector(int bype) {
	viktor v = (viktor) malloc(sizeof(struct viktorr));
	v->c = 0;
	v->s = NULL;
	v->bype = bype;
	return v;
}

void appendElementToVector(viktor v, void* el) {
	v->c++;
	v->s = realloc(v->s, v->c * v->bype);
	memcpy(&((char*)v->s)[v->c - 1], el, v->bype);
}

void* getElementFromVector(viktor v, int i) {
	return &((char*)v->s)[i * v->bype];
}

void dropVector(viktor v) {
	free(v->s);
	free(v);
}

void* expandVector(viktor v) {
	v->c++;
	v->s = realloc(v->s, v->c * v->bype);
	return getElementFromVector(v, v->c-1);
}

void shrinkVector(viktor v) {
	if (v->c > 0) {
		v->c--;
		v->s = realloc(v->s, v->c * v->bype);
	}
}



#define VikNew(type) newVector(sizeof(type))
#define VikApp(v, e) appendElementToVector(v, e)
#define VikGetp(v, i) getElementFromVector(v, i)
#define VikGett(v, i, t) ((t*) getElementFromVector(v, i))
#define VikDrop(v) dropVector(v)
#define VikExp(v) expandVector(v)
#define VikShrink(v) shrinkVector(v)

void VikAppvik(viktor v, viktor vo) {
	for (int i = 0; i < vo->c; ++i) {
		void* d = getElementFromVector(vo, i);
		appendElementToVector(v, d);
	}
}

