typedef struct symbol_ * symbol;

// 给定一个char * 返回一个 Symobl 
// 一个string pool 不同的char *但是相同的内容会返回相同的内容
symbol Symbol(char *);

// 给定一个 symbol 返回对应的 char * 
char * name(symbol);

// 符号表的管理 
TAB_table S_empty();

// 对hash table 的一层封装 
void S_enter(TAB_table t,symbol sym,void* value);

void *S_look(TAB_table t,symbol sym);
 
// 最关键的函数block 结构管理 
void beginScope(TAB_table t);

void endScope(TAB_table t);
