// v0t xry sim / vtxs

// favorite super powers:
// tryna/iden stuff, so that i don't have to let go
// olmak, for travel
// fv5, for obvious reasons
// time reset, for fixing mistakes
// sorcery is also great, if you can learn the other things with it
// basically: tedd, five, klaus


// node-table types



// function signature

typedef struct fsigparam_t {
	u8 rw; // v w r rw
	char* vartype;
	char* longname;
	char* varname;
} fsigparam_t;

typedef struct fsig_t {
	u8* name;
	u32 argc;
	struct fsigparam_t* argv;
} fsig_t;

fsig_t fsig_list[32];
int fsigc = 0;

fsig_t* createFsig(char* fname) {
	fsig_t* fsp = malloc(sizeof(fsig_t));
	fsp->name = fname;
	fsp->argc = 0;
	fsp->argv = malloc(0);
	return fsp;
}

fsig_t* createFsigGlobal(char* fname) {
	fsig_list[fsigc].name = fname;
	fsig_list[fsigc].argc = 0;
	fsig_list[fsigc].argv = malloc(0);
	++fsigc;
	return &fsig_list[fsigc - 1];
}

void createFsigInarr(int* lp, fsig_t** fsigap, char* fname) {
	*fsigap = realloc(*fsigap, (*lp + 1) * sizeof(fsig_t));
	(*fsigap)[*lp].name = fname;
	(*fsigap)[*lp].argc = 0;
	(*fsigap)[*lp].argv = malloc(0);
}

void addFsigRow(fsig_t* fsp, u8 rw, char* vartype, char* varname) {
	fsp->argv = realloc(fsp->argv, (fsp->argc + 1) * sizeof(fsigparam_t));
	fsp->argv[fsp->argc].rw = rw;
	fsp->argv[fsp->argc].vartype = vartype;
	fsp->argv[fsp->argc].varname = varname;
	fsp->argv[fsp->argc].longname = "";
	++fsp->argc;
}










typedef struct {
	int c;
	char** strings;
} string_list;

string_list newStringList() {
	string_list h;
	h.c = 0;
	h.strings = malloc(0);
	return h;
}

void appSzToList(string_list* slp, char* str) {
	int c = strlen(str);
	slp->strings = realloc(slp->strings, (slp->c + 1) * sizeof(void*));
	slp->strings[slp->c] = malloc(c + 1);
	strncpy(slp->strings[slp->c], str, c + 1);
	++slp->c;
}

int findInStringList(string_list* slp, char* str) {
	for (int i = 0; i < slp->c; ++i)
		if (strcmp(slp->strings[i], str) == 0) return i;
	return -1;
}

void unnSzToList(string_list* slp, char* str) {
	if (findInStringList(slp, str) == -1)
		appSzToList(slp, str);
}
