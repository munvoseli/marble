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

fsig_t* createFsig(char* fname) {
	fsig_t* fsp = malloc(sizeof(fsig_t));
	fsp->name = fname;
	fsp->argc = 0;
	fsp->argv = malloc(0);
	return fsp;
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