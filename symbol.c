#include <string.h>
#include <stdlib.h>
#include "table.h"
#include "symbol.h"
#include "util.h"
#define SIZE (109)

struct symbol_ {
	char * name;
	symbol next;
};

static symbol mksymbol(char *name ,symbol next){
	symbol s= checked_malloc(sizeof(*s)); 
	s->name=checked_malloc(strlen(name)+3);// 手动分配内存 
	memset(s->name,0,sizeof(s->name));
	strcpy(s->name,name);
	s->next=next;
	return s;
}

static symbol hashtable[SIZE];// 处理所有的符号 
static unsigned int hash(char *s){
	unsigned int ha=0;
	for(char* i=s;*i;i++){
		ha=ha*65599+*i;
	}
	return ha;
} 


symbol Symbol(char *name ){
	int h =hash(name)%SIZE;
	symbol syms = hashtable[h];
	for(symbol sym=syms;sym;sym=sym->next){
		if(!strcmp(sym->name,name)){
			return sym;
		}
	}
	symbol sym=mksymbol(name,hashtable[h]);
	hashtable[h]=sym;
	return sym;
}

// 给定一个 symbol 返回对应的 char * 
char * name(symbol sym){
	return sym->name;
}

// 符号表的管理 
TAB_table S_empty(){
	return TAB_empty();
}

// 对hash table 的一层封装 
void S_enter(TAB_table t,symbol sym,void* value){
	TAB_enter(t,sym,value); 
}

void *S_look(TAB_table t,symbol sym){
	return TAB_look(t,sym);
}

static struct symbol_ marksym = {"<mark>", 0};
 
// 最关键的函数block 结构管理 
void beginScope(TAB_table t){
	S_enter(t,&marksym,NULL);
}


void endScope(TAB_table t){
	symbol s;
  	do s=TAB_pop(t);
	while (s != &marksym);
}
