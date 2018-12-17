#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "util.h" 
#include "c0error.h"
#include "c0lex.h"
#include "table.h"
#include "symbol.h"
#include "absyn.h"
#include "lev.h" 
#include "penv.h"
#include "topcode.h"


#define STRING_SPACE (10)
#define MAX_INT_LEN  (2000)
#define OP_RET (0)
#define OP_ADD (2)
#define OP_MINUS (3)
#define OP_TIMES (4)
#define OP_DIV (5)
#define OP_NEQ (9)
#define OP_EQ (8)
#define OP_LT (10)
#define OP_GT (12)
#define OP_LE (12)
#define OP_GE (11)

 /* 
#define MAX_LEV (100)
#define MAX_TAB (200)
#define MAX_BTAB (50)
tab Tab(char *name,int link,int obj,int typ,int ref,int lev,int normal,int adr){
	tab s=checked_malloc(sizeof(*s));
	s->name=checked_malloc(strlen(name)+2);
	strcpy(s->name,name);
	s->lev=lev;
	s->link=link;
	s->obj=obj;
	s->normal=normal;
	s->adr=adr;
	s->ref=ref;
	s->typ=typ;
	return s;}
btab Btab(int last,int lastpar,int psize,int vsize){
	btab s=checked_malloc(sizeof(*s));
	s->last=last;
	s->lastpar=lastpar;
	s->psize=psize;
	s->vsize=vsize;
	return s;}
static tab p_Tab[MAX_TAB];
static Tab_top =0;
static btab p_Btab[MAX_BTAB];
static Btab_top=0;
static int display[MAX_LEV]; //分程序索引表 
*/

static A_constval _NULL_;
static A_constval constnull(){
	return _NULL_;
}


static int ip=0;
static int start_ip=-1;
static pcode intr[MAX_INT_LEN];
static void display(pcode pc){
	switch(pc->ptype){
		case LIT:printf("LIT  ");break;
		case OPR:printf("OPR  ");break;
		case OPRF:printf("OPRF ");break;
		case LOD:printf("LOD  ");break;
		case STO:printf("STO  ");break;
		case CAL:printf("CAL  ");break;
		case PINT:printf("INT  ");break;
		case JMP:printf("JMP  ");break;
		case JPC:printf("JPC  ");break;
		case RED:printf("RED  ");break;
		case WRT:printf("WRT  ");break;
		case WRTS:printf("WRTS ");break;
		case MKS:printf("MKS  ");break;
		case FLT:printf("FLT  ");break;
	}
	printf("%d %d\n",*pc->x,*pc->y);
}

void outputpcode(){
	for(int i=0;i<ip;i++){
		printf("%d :",i);
		display(intr[i]);
	}
	printf("starting address is %d \n",start_ip);
}
static void EnterConst(TAB_table env,lev l,A_constDecList ccdl){
 
	for(A_constDecList cdl=ccdl;cdl;cdl=cdl->tail){
	 
		A_constval t;
		switch(cdl->head->type){
			case INT_TY:
				t= A_ConstNum(cdl->head->u.intt);break;		
			case FLOAT_TY:
				t= A_ConstFloat(cdl->head->u.floatt);break;
			case STRING_TY:
				t=A_ConstString(cdl->head->u.stringg);break;
		}
		P_entry pe=PvarEntry(cdl->head->sym,cdl->head->type,1,t,l->lev,l->offset); //const不分配栈帧空间 
		S_enter(env,cdl->head->sym,pe);		
	}
}

static void AllocVar(TAB_table env,lev l,A_type t,symbol sym){
	P_entry pe;
		switch (t){
			case INT_TY:{
				pe=PvarEntry(sym,INT_TY,0,constnull(),l->lev,l->offset);
				l->offset++;
				break;
			}
			
			case FLOAT_TY:{
				pe=PvarEntry(sym,FLOAT_TY,0,constnull(),l->lev,l->offset);
				l->offset++;
			}
			
			case STRING_TY:{
				pe=PvarEntry(sym,STRING_TY,0,constnull(),l->lev,l->offset);
				l->offset+=STRING_SPACE;
				break;
			}
		}
		S_enter(env,sym,pe);
}

static pcode outpcode(enum pty pp,int x,int y){
	pcode p=Pcode(pp,x,y);
	intr[ip]=p;
	ip++;
	return p;
}

static int NumOfArg(A_argList a){
	int count=0;
	while(a){
		count++;
		a=a->tail;
	}
	return count;
} 
void pproc_exp(TAB_table env,A_exp exp,lev l,A_type ty);

pcode Pcode(enum pty pp,int x,int y){
	pcode p=checked_malloc(sizeof(*p));
	p->x=checked_malloc(sizeof(int));
	p->y=checked_malloc(sizeof(int));
	*(p->x)=x;
	*(p->y)=y;
	p->ptype=pp;
	return p; 
}

void pproc_factor(TAB_table env,A_f f,lev l, A_type ty){//A_syf,A_expf,A_intf,A_floatf,A_callf
	switch(f->kind){
		case A_syf:{
			P_entry pe=S_look(env,f->u.sym); 
			if(pe->u.var.isconst==1){
				if(ty==INT_TY){//常量 
					int a;
					if(pe->u.var.ty==FLOAT_TY) a=(int)(pe->u.var.val.flo);
					else if(pe->u.var.ty==INT_TY) a=pe->u.var.val.num;
					outpcode(LIT,0,a);
				}
				
				else if(ty==FLOAT){
					int *a;
					if(pe->u.var.ty==FLOAT_TY) a=(int *)&(pe->u.var.val.flo);
					else {
						float t=(float )pe->u.var.val.num;
						if(pe->u.var.ty==INT_TY) a=(int *)&t;
					}
					outpcode(LIT,0,*a);
				}
			}
			else{// 非常量 
				int diffl=pe->u.var.lev - l->lev;
				int offset=pe->u.var.offset;
				outpcode(LOD,diffl,offset);
				
				if(pe->u.var.ty==INT_TY && ty==FLOAT_TY){ // int 2 float 
					outpcode(FLT,0,0);
				}
				
				if(pe->u.var.ty==FLOAT_TY && ty==INT_TY){//float 2 int
					outpcode(FLT,0,1);
				}
			}		
			break;
		}
		case A_expf:{
			pproc_exp(env,f->u.exp,l,ty);
			break;
		}
		case A_intf:{
			outpcode(LIT,0,f->u.intt);
			break;
		}
		case A_floatf:{
			outpcode(LIT,0,*(int *)&(f->u.floatt));
			break;
		}
		case A_callf:{
			P_entry pe=S_look(env,f->u.call.funname);
			
			int size=NumOfArg(pe->u.fun.arglist);
			outpcode(MKS,0,size);
			int count=0;
			
			A_argList agls=pe->u.fun.arglist;
			for(A_valArgs agl =f->u.call.vals;agl;agl=agl->tail){
				pproc_exp(env,agl->head,l,agls->head->type);//置于栈顶
				outpcode(STO,0,count);	
				count++;
				agls=agls->tail;
			}
			int diffl=pe->u.fun.lev - l->lev;
			pcode cal=outpcode(CAL,diffl,*(pe->u.fun.start));
			cal->y = pe->u.fun.start;			
			break;
		}
	}
}
void pproc_item(TAB_table env,A_t t,lev l,A_type ty){
	pproc_factor(env,t->f,l,ty);
	A_ftail ft=t->ftail;
	A_oper op;
	
	while(ft){
		pproc_factor(env,ft->f,l,ty);
		int o=-1;
		op=ft->oper;
		switch(op){
			case A_mul:
				o=OP_TIMES;break;
			case A_div: 
				o=OP_DIV;break;
			default:
				assert(0);
		}
		if(ty==INT_TY){
			outpcode(OPR,0,o);
		}else if(ty==FLOAT_TY){
			outpcode(OPRF,0,o);
		}
		ft=ft->ftail;
	}
}

void pproc_exp(TAB_table env,A_exp exp,lev l,A_type ty){
	if(exp->oper==A_minus){
		outpcode(LIT,0,0);
	}
	
	pproc_item(env,exp->t,l,ty);
	
	if(exp->oper==A_minus){
		if(ty==INT_TY){
			outpcode(OPR,0,OP_MINUS);	
		}
		if(ty==FLOAT_TY){
			outpcode(OPRF,0,OP_MINUS);
		}
	}
	
	A_ttail tt=exp->ttail;
	A_oper op;
	
	while(tt){
		pproc_item(env,tt->t,l,ty);
		int o=-1;
		op=tt->oper;
		switch(op){
			case A_add:
				o=OP_ADD;break;
			case A_minus: 
				o=OP_MINUS;break;
			default:
				assert(0);
		}
		
		if(ty==INT_TY){
			outpcode(OPR,0,o);
		}else if(ty==FLOAT_TY){
			outpcode(OPRF,0,o);
		}
		
		tt=tt->ttail;
	}
	

}

void pproc_flag(TAB_table env,A_flag fl,lev l){
	A_type lef=find_type(env,fl->left);
	A_type rig=find_type(env,fl->right);
	A_type t=INT_TY;
	
	if(lef==FLOAT_TY || rig==FLOAT_TY){
		t=FLOAT_TY;	
	}
	
	pproc_exp(env,fl->left,l,t);
	pproc_exp(env,fl->right,l,t);
	
	int o=-1;
	switch (fl->com){
		case A_GE:
			o=OP_GE;break;
		case A_LE:
			o=OP_LE;break;
		case A_NE:
			o=OP_NEQ;break;
		case A_EQ:
			o=OP_EQ;break;
		case A_L:
			o=OP_LT;break;
		case A_G:
			o=OP_GT;break;
	}
	
	if(t==INT_TY){	
		outpcode(OPR,0,o);
	}
	
	if(t==FLOAT_TY){
		outpcode(OPRF,0,o);
	}
	
	
}

void pproc_seq(TAB_table env,A_seq seq,lev l){
	switch(seq->kind){
		case A_if:{
			pproc_flag(env,seq->u.iff.flag,l);// 结果放到栈顶			
			int *f;
			pcode jpc=outpcode(JPC,0,0);
			f=jpc->y;
			pproc_seq(env,seq->u.iff.thenn,l);
			
			int *t;
			if(seq->u.iff.elsee){
				pcode jup=outpcode(JMP,0,0);
				t=jup->y;
			}
			
			*f=ip;//回填技术
			if(seq->u.iff.elsee){
				pproc_seq(env,seq->u.iff.elsee,l);
				*t=ip;
			}
		
			break;
		}
		case A_ass:{
			P_entry pe = S_look(env,seq->u.ass.sym);
			int diffl  = l->lev - pe->u.var.lev;
			int offset = pe->u.var.offset;
			
			pproc_exp(env,seq->u.ass.exp,l,pe->u.var.ty);	
			
			pcode sto= outpcode(STO,diffl,offset); 
			break;
		}
		case A_call:{
			P_entry pe=S_look(env,seq->u.call.func);
			int size=NumOfArg(pe->u.fun.arglist);
			outpcode(MKS,0,size);
			int count=0;
			A_argList agls =pe->u.fun.arglist; 
			for(A_valArgs agl = seq->u.call.valargs;agl;agl=agl->tail){
				pproc_exp(env,agl->head,l,agls->head->type);//置于栈顶 // 默认转换类型 
				outpcode(STO,0,count);	
				count++;
				agls=agls->tail;
			}
			int diffl=pe->u.fun.lev - l->lev;
			
			pcode cal=outpcode(CAL,diffl,*(pe->u.fun.start));
			cal->y = pe->u.fun.start;
			
			break;
		}
		case A_print:{
			if(seq->u.print.stringg){
				int len=strlen(seq->u.print.stringg);
				for(int i=0;i<len;i++){
					outpcode(LIT,0,(int)(seq->u.print.stringg[i]));
					outpcode(WRTS,0,0);
				}
			}
			if(seq->u.print.exp){
				A_type t=find_type(env,seq->u.print.exp);
				pproc_exp(env,seq->u.print.exp,l,t);					
				outpcode(WRT,0,0);
			}
			break;
		}
		case A_ret:{
			if(seq->u.ret.exp){
				A_type t=find_type(env,seq->u.ret.exp);
				pproc_exp(env,seq->u.ret.exp,l,t);
				outpcode(STO,0,0);
			}
			outpcode(OPR,0,0);
			break;
		}
		case A_scan:{
			P_entry pe=S_look(env,seq->u.scan.sym);
			int diffl =pe->u.var.lev - l->lev;
			int offset =pe->u.var.offset;
			outpcode(RED,diffl,offset);
			break;
		}
		case A_while:{
			int *e=checked_malloc(sizeof(int));
			int *s=checked_malloc(sizeof(int));
			*s= ip;
			pproc_flag(env,seq->u.whilee.flag,l);
			
			outpcode(LIT,0,0);
			outpcode(OPR,0,OP_NEQ);
			
			pcode jpc =outpcode(JPC,0,*e);
			e=jpc->y;
			pproc_seq(env,seq->u.whilee.seq,l);
			
			pcode jmp =outpcode(JMP,0,0);
			jmp->y=s;
			*e=ip;
			
			break;
		}
		case A_seqss:{
			beginScope(env);
			A_seqs ss=NULL;
			for(A_seqs s=seq->u.sqs;s;s=s->seqs){//reverse
				ss=A_Seqs(s->seq,ss);
			}
			for(;ss;ss=ss->seqs){
				pproc_seq(env,ss->seq,l);	
			}
			endScope(env);
			break;
		} 
	}	
} 

void pproc_seqs(TAB_table env,A_seqs seqs,lev l){
	A_seqs ss=NULL;
	for(A_seqs s=seqs;s;s=s->seqs){//reverse
		ss=A_Seqs(s->seq,ss);
	}
	
	for(;ss;ss=ss->seqs){
		pproc_seq(env,ss->seq,l);	
	}
	
}

//
void pproc_func(TAB_table env,A_funcDec fd,lev l){
 
	P_entry pe=PfuncEntry(fd->name,fd->rettype,fd->agrlist,l->lev,-1);
	S_enter(env,fd->name,pe); //函数头需要最先处理 是唯一在这个层次外可以访问的东西 
	 
	beginScope(env);	
	for(A_argList a=fd->agrlist;a;a=a->tail){
		AllocVar(env,l,a->head->type,a->head->argsy);
	}
 
	
	EnterConst(env,l,fd->cseqs->constdeclist);
 
	int oldoffset=l->offset;
	A_varDecList nvdl=NULL;
	for(A_varDecList vdl=fd->cseqs->vardeclist;vdl;vdl=vdl->tail){ //reverse not necessary
		nvdl=A_VarDecList(vdl->head,nvdl);
	}
	for(;nvdl;nvdl=nvdl->tail){
		AllocVar(env,l,nvdl->head->type,nvdl->head->sym);
	}
	int newoffset=l->offset;
	
	if(newoffset-oldoffset!=0) 
	outpcode(PINT,0,newoffset-oldoffset);
// note 目前不需要考虑静态链信息 
// 如果有程序的子函数定义(目前没有) ，那么就应该在这里进行管理
// to do
	//之前的函数地址已经确定  
 
	*pe->u.fun.start=ip;
	pproc_seqs(env,fd->cseqs->seqs,l);
	endScope(env);
	outpcode(OPR,0,OP_RET);
}

void getpcode(A_prog prog){
	/*
	Tab_top=0; 
	Btab_top=0;
	
	p_Btab[Btab_top]=Btab(1,1,0,0);
	Btab_top++;
	
	int lev=1;    //当前静态深度 
 	display[0]=1; // 0 是预留层 
	display[1]=2; // 1 是当前层主函数	 
	*/
 
	TAB_table env=S_empty();
	
	lev l=NewLev(); 
	l->lev=0;
	l->offset=3; 
	EnterConst(env,l,prog->constdeclist);
 
	
	int oldoffset = l->offset;
	A_varDecList nvdl=NULL;
	for(A_varDecList vdl=prog->vardeclist;vdl;vdl=vdl->tail){ //reverse not necessary
		nvdl=A_VarDecList(vdl->head,nvdl);
	}
	for(;nvdl;nvdl=nvdl->tail){
		AllocVar(env,l,nvdl->head->type,nvdl->head->sym);
	}
	
	int newoffset= l->offset;
	if(newoffset-oldoffset!=0) 
	outpcode(PINT,0,newoffset-oldoffset);
	
	A_funcDecList nfdl=NULL; 
	for(A_funcDecList fdl=prog->fundeclist;fdl;fdl=fdl->tail){  //reverse
		nfdl=A_FuncDecList(fdl->head,nfdl);
	}
	int co=0;
	for(;nfdl;nfdl=nfdl->tail){
		
		lev nl=NewLev();
		nl->lev=l->lev+1;
		nl->offset=4;
 
		pproc_func(env,nfdl->head,nl); 	
 
	}

	// main 函数处理过程
	
	beginScope(env);	

	for(A_argList a=prog->mainn->valargs;a;a=a->tail){
		AllocVar(env,l,a->head->type,a->head->argsy);
	}
	symbol mainsym=Symbol("Main");
 	P_entry pe=PfuncEntry(mainsym,prog->mainn->ret,prog->mainn->valargs,l->lev,-1);
	S_enter(env,mainsym,pe);
	EnterConst(env,l,prog->mainn->cseqs->constdeclist);

	A_varDecList nvdls=NULL;
	for(A_varDecList vdl=prog->mainn->cseqs->vardeclist;vdl;vdl=vdl->tail){ //reverse not necessary
		nvdls=A_VarDecList(vdl->head,nvdls);
	}
	for(;nvdls;nvdls=nvdls->tail){
		AllocVar(env,l,nvdls->head->type,nvdls->head->sym);
	}
	
	start_ip =ip;
	*pe->u.fun.start=ip;
	
	pproc_seqs(env,prog->mainn->cseqs->seqs,l);
	endScope(env); 	
	outpcode(OPR,0,OP_RET);
}
