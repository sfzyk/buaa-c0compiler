typedef struct symbol_ * symbol;

// ����һ��char * ����һ�� Symobl 
// һ��string pool ��ͬ��char *������ͬ�����ݻ᷵����ͬ������
symbol Symbol(char *);

// ����һ�� symbol ���ض�Ӧ�� char * 
char * name(symbol);

// ���ű�Ĺ��� 
TAB_table S_empty();

// ��hash table ��һ���װ 
void S_enter(TAB_table t,symbol sym,void* value);

void *S_look(TAB_table t,symbol sym);
 
// ��ؼ��ĺ���block �ṹ���� 
void beginScope(TAB_table t);

void endScope(TAB_table t);
