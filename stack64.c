


void evalStack(char** vars, char** ops) {
	
}

void evalStackStr() {
	char* vars[] = {
		"x",
		"y",
		"z"
	};
	int vn = sizeof(vars) / sizeof(char*);
	char* ops[] = {
		"2", "x", "set",
		"5", "y", "set",
		"x", "y", "mul", "z", "set"
	};
	int on = sizeof(ops) / sizeof(char*);
}
