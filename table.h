typedef struct TAB_table_ *TAB_table;
/*
��¼һ����ֵ�� 
*/
void TAB_enter(TAB_table t,void *key,void* value);

 
void* TAB_look(TAB_table t,void *key);

/*
���һ�������վ�� 
*/
void* TAB_pop(TAB_table t);

/*
չʾ���ű��е�ÿһ����key-value 
*/
void TAB_dump(TAB_table t,void(*show)(void *key,void *value));

