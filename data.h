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
	u8 type; // 1; in signature: mut(3) / con(4) / val(5)
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

struct fsig_t {
	u8* name;
	u8 rettype;
	u32 argc;
	struct var_t* argv;
};
