// v0t xry sim / vtxs

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

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
