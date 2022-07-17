






// v0t xry sim / vtxs
// i have no idea what this means anymore
// but it says "the opposite of three symbols"
// best guess is some sort of half-life/portal reference
// oooh i think maybe it's to C as v0tgil is to gil
// yeah, it was an early name for the project

// favorite super powers:
// tryna/iden stuff, so that i don't have to let go
// olmak, for travel
// fv5, for obvious reasons
// time reset, for fixing mistakes
// sorcery would be great, if you can learn the other things with it
//  but that feels like something the developers would patch out
//  and then you get nothing
// basically: ellen, five, klaus


// node-table types



// function signature

typedef struct fsigparam_t {
	u8 rw; // v w r rw
	char* vartype;
	char* longname;
	char* varname;
} fsigparam_t;

typedef struct fsig_t {
	s8* name;
	viktor args; //<>fsigparam_t
} fsig_t;

fsig_t fsig_list[32];
int fsigc = 0;

fsig_t* createFsig(char* fname) {
	fsig_t* fsp = (fsig_t*) malloc(sizeof(fsig_t));
	fsp->name = fname;
	fsp->args = VikNew(fsigparam_t);
	return fsp;
}

fsig_t* createFsigGlobal(char* fname) {
	fsig_list[fsigc].name = fname;
	fsig_list[fsigc].args = VikNew(fsigparam_t);
	++fsigc;
	return &fsig_list[fsigc - 1];
}

void createFsigInarr(int* lp, fsig_t** fsigap, char* fname) {
	*fsigap = realloc(*fsigap, (*lp + 1) * sizeof(fsig_t));
	(*fsigap)[*lp].name = fname;
	(*fsigap)[*lp].args = VikNew(fsigparam_t);
}

void addFsigRow(fsig_t* fsp, u8 rw, char* vartype, char* varname) {
	fsigparam_t* fspp = VikExp(fsp->args);
	fspp->rw = rw;
	fspp->vartype = vartype;
	fspp->varname = varname;
	fspp->longname = "";
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

void popStringList(string_list* slp) {
	--slp->c;
	free(slp->strings[slp->c]);
	slp->strings = realloc(slp->strings, slp->c * sizeof(void*));
}


enum {
	Callinp_var,
	Callinp_const,
	Callinp_none
};

typedef struct {
	u8 tag;
	viktor name; // <>u8
} var_data;

typedef struct {
	u8 tag;
	viktor text; // <>u8
} const_data;

typedef struct {
	u8 tag;
	u32 id; // index into array of var_data
} varvar_data;

typedef union {
	u8 tag;
	varvar_data var;
	const_data con;
} call_input;








