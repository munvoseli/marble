enum {
	Tag_node_stem,
	Tag_node_fsig,
	Tag_node_call
};

typedef struct {
	u8 tag; // function signature (node), call, for, if, scope...
	u32 prev_node;
	u32 next_node;
	float x;
	float y;
} nodeinfo_t;

typedef struct call_node { nodeinfo_t ni;
	u32 fsig_index;
	char** string_arr;
} call_node;

typedef struct stem_node { nodeinfo_t ni;
	char* text;
	int tlen;
} stem_node;

typedef struct fsig_node { nodeinfo_t ni;
	u32 fsig_index;
} fsig_node;

typedef struct for_node { nodeinfo_t ni;
	char type[16];
	char varname[16];
	char i0[16]; char di[16]; char i2[16];
	u32 repeat_block;
} for_node;

typedef struct if_node { nodeinfo_t ni;
	char cond[16];
	u32 if_block;
	u32 else_block;
} if_node;

typedef struct scope_node { nodeinfo_t ni;
	u32 block;
} scope_node;

// 0: fsig (node)
typedef union { nodeinfo_t ni;
	fsig_node fsig;
	call_node call;
	stem_node stem;
	struct for_node forn;
	struct if_node ifn;
	struct scope_node scope;
} node_t;
