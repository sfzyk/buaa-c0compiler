/*
ջʽ hash ���ű�ʵ�� 
*/
#include <assert.h>
#include <stdlib.h>

#include "util.h"
#include "table.h" 
#define TAB_SIZE (137)

typedef struct entry_* entry;



struct TAB_table_{
	entry hashtable[TAB_SIZE];
	void* top;
};
 
struct entry_{	
	void *key;
	void *value; 
	
	void *next;    //���hash��ͻ��ָ��  
	void *prevtop; //ά��ջʹ�õ�ָ�� 
};
static entry Entry(void* key,void* value,void* next,void* prevtop){
	entry ent = checked_malloc(sizeof(*ent));
	ent->key=key;
	ent->value=value;
	ent->next=next;
	ent->prevtop=prevtop;
	return ent;
}



TAB_table TAB_empty(){
	TAB_table t=checked_malloc(sizeof(*t));
	for(int i=0;i<TAB_SIZE;i++){
		t->hashtable[i]=NULL;
	}
	t->top=NULL;
	return t;
}


void TAB_enter(TAB_table t,void *key,void * value){
	int hash =(unsigned )key % TAB_SIZE;
	entry ent = Entry(key,value,t->hashtable[hash],t->top);
	
	t->hashtable[hash]=ent;
 
	t->top=key; //����Ҫע��Լ�� 
} 

/*
���ز��ҵ��� value ������NULL (û�ҵ��������) 
*/
void* TAB_look(TAB_table t,void *key){
	int hash =(unsigned )key % TAB_SIZE;	
	entry p = t->hashtable[hash];
	
	while(p&&p->key!=key){
		p=p->next; 
	}

	if(p){
		return p->value;
	}else{
		return NULL;
	}
}


void *TAB_pop(TAB_table t) {
  void *k; entry b; int index;
  assert (t);
  k = t->top;
 
  assert (k);
  
  index = ((unsigned)k) % TAB_SIZE;

  b = t->hashtable[index];
  
  assert(b);
  t->hashtable[index] = b->next;
  
  t->top=b->prevtop; 
  return b->key;
}




