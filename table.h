typedef struct TAB_table_ *TAB_table;
/*
记录一个键值对 
*/
void TAB_enter(TAB_table t,void *key,void* value);

 
void* TAB_look(TAB_table t,void *key);

/*
清除一个最后入站的 
*/
void* TAB_pop(TAB_table t);

/*
展示符号表中的每一个对key-value 
*/
void TAB_dump(TAB_table t,void(*show)(void *key,void *value));

