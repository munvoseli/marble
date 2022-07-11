enum {
	Tag_node_stem,
	Tag_node_fsig,
	Tag_node_call,
	Tag_node_bi
};

typedef struct {
	u8 tag; // function signature (node), call, for, if, scope...
	u32 prev_node;
	u32 next_node;
} nodeinfo_t;

typedef struct call_node { nodeinfo_t ni;
	int fsig_index;
	int srow;
	int scol;
	viktor inputs; // <>call_input
} call_node;

typedef struct stem_node { nodeinfo_t ni;
	char* text;
	int tlen;
} stem_node;

typedef struct fsig_node { nodeinfo_t ni;
	int fsig_index;
	int srow;
	int scol;
} fsig_node;

typedef struct bi_node { nodeinfo_t ni;
	struct bi_node* parentBlock; // NULL if lestmost
	fsig_node* fsig; // NULL if not leftmost
	int firstNode;
	viktor vars; // <>var_data
} bi_node;

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
	bi_node bi;
	struct for_node forn;
	struct if_node ifn;
	struct scope_node scope;
} node_t;


void drawStemNode(node_t* np, camact_t ca);
void drawFsigNode(node_t* np, camact_t ca);
void drawCallNode(node_t* np, camact_t ca);
void drawBiNode(node_t* np, camact_t ca);
void keybStemNode(node_t* np, SDL_Event* evp);
void keybFsigNode(node_t* np, SDL_Event* evp);
void keybCallNode(node_t* np, SDL_Event* evp);
void keybBiNode(node_t* np, SDL_Event* evp);
void initStemNode(node_t* np);
void initFsigNode(node_t* np);
void initCallNode(node_t* np);
void initBiNode(node_t* np);
void freeStemNode(node_t* np);
void freeFsigNode(node_t* np);
void freeCallNode(node_t* np);
void freeBiNode(node_t* np);
float gethStemNode(node_t* np);
float gethFsigNode(node_t* np);
float gethCallNode(node_t* np);
float gethBiNode(node_t* np);

