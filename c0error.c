#include <stdio.h>
#include "c0error.h"
#include "util.h"
typedef struct PosList_ * posList; 
struct PosList_{
	int pos;
	posList tail;
};

static posList PosList(int i,posList rest){
	posList l=checked_malloc(sizeof(struct PosList_));
	l->pos=i;
	l->tail=rest;
	return l;
}

int tokPos=0;
static posList posLi = NULL;
static int lineNum = 0;

void newline(){
	lineNum ++ ;
	posLi = PosList(tokPos,posLi);
}

void error(int pos,char *message){
	posList posLi2 = posLi;
	int num=lineNum;
	
	while(posLi2 && posLi2->pos >=pos ){
		posLi2=posLi2->tail;
		num --;
	}
	
	if(posLi2)fprintf(stderr,"%d line %d pos ",num,pos-posLi2->pos);
	fprintf(stderr,"error :%s\n",message);
	
}
