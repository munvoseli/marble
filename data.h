// v0t xry sim / vtxs

// favorite super powers:
// tryna/iden stuff, so that i don't have to let go
// olmak, for travel
// fv5, for obvious reasons
// time reset, for fixing mistakes
// sorcery is also great, if you can learn the other things with it
// basically: tedd, five, klaus

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

// node-table types

typedef struct {
	u8 tag; // function signature (node), call, for, if, scope...
	u32 prev_node;
	u32 next_node;
	float x;
	float y;
} nodeinfo_t;

struct fsig_node {
	nodeinfo_t ni;
	u32 fsig_index;
};

struct call_node {
	nodeinfo_t ni;
	u32 fsig_index;
	u32 string_i0;
};

struct for_node {
	nodeinfo_t ni;
	char type[16];
	char varname[16];
	char i0[16];
	char di[16];
	char i2[16];
	u32 repeat_block;
};

struct if_node {
	nodeinfo_t ni;
	char cond[16];
	u32 if_block;
	u32 else_block;
};

struct scope_node {
	nodeinfo_t ni;
	u32 block;
};

typedef union {
	nodeinfo_t ni;
	struct fsig_node fsig;
	struct for_node forn;
	struct if_node ifn;
	struct scope_node scope;
} node_t;







enum vtype {
	Tu8,
	Ts8,
	Tu16,
	Ts16,
	Tu32,
	Ts32,
	Tu64,
	Ts64,
	Tf32,
	Tf64,
	Tp,
	Tvar
};

enum vlinsty {
	VLcall,
	VLvar,
	VLconst
};

enum paramsigmut {
	PSmut,
	PScon,
	PSval
};

// value instance

struct call_t {
	u8 type; // 0
	float x;
	float y;
	u32 fsig_i;
	union valinst_t* argv;
};

struct var_t {
	u8 type; // 1
	float x;
	float y;
	u8 vartype;
	u8* name_szp;
};

struct const_t {
	u8 type; // 2
	float x;
	float y;
	u8 vartype;
	u8* name_szp;
};

union valinst_t {
	u8 type;
	struct call_t call;
	struct var_t var;
	struct const_t con;
};

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

typedef struct iterFsigState {
	fsig_t* fsigp;
	int row;
	int attr;
} ifs_t;

char* rwstrs[] = { "v", "w", "r", "rw" };
char* iterateFsigCell(void* ifsv) {
	struct iterFsigState* ifs = (struct iterFsigState*) ifsv;
	char* res;
	switch (ifs->attr) {
	case 0:
		res = rwstrs[ifs->fsigp->argv[ifs->row].rw];
		break;
	case 1:
		res = ifs->fsigp->argv[ifs->row].vartype;
		break;
	case 2:
		res = ifs->fsigp->argv[ifs->row].varname;
		break;
	case 3:
		res = ifs->fsigp->argv[ifs->row].longname;
		break;
	}
	++ifs->attr;
	if (ifs->attr == 4) {
		ifs->attr = 0;
		++ifs->row;
	}
	return res;
}
void iterresetFsigCell(void* ifsv) {
	struct iterFsigState* ifsp = (struct iterFsigState*) ifsv;
	ifsp->row = 0; ifsp->attr = 0;
}

fsig_t* createFsig(char* fname) {
	fsig_t* fsp = malloc(sizeof(fsig_t));
	fsp->name = fname;
	fsp->argc = 0;
	fsp->argv = malloc(0);
	return fsp;
}

void addFsigRow(fsig_t* fsp, u8 rw, char* vartype, char* varname) {
	fsp->argv = realloc(fsp->argv, (fsp->argc + 1) * sizeof(fsigparam_t));
	fsp->argv[fsp->argc].rw = rw;
	fsp->argv[fsp->argc].vartype = vartype;
	fsp->argv[fsp->argc].varname = varname;
	fsp->argv[fsp->argc].longname = "";
	++fsp->argc;
}
