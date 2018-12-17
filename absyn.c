#include <assert.h> 
#include <string.h>

#include "util.h"
#include "table.h"
#include "symbol.h"
#include "absyn.h"
#include "env.h"


A_rettype type2ret(A_type a){
	switch (a){
	case INT_TY:
		return INT_RETTY;
	case FLOAT_TY:
		return FLOAT_RETTY;
	default :
		return -1;
	}
}

A_type ret2type(A_rettype r){
	switch (r){
		case INT_RETTY:
			return  INT_TY;
		case FLOAT_RETTY:
			return FLOAT_TY;
		default:
			return -1;
	}
}

A_prog A_Prog(A_constDecList cdl,A_varDecList vdl,A_funcDecList fdl,A_main am){
	A_prog prog=checked_malloc(sizeof(*prog));
	prog->constdeclist=cdl;
	prog->vardeclist=vdl;
	prog->fundeclist=fdl;
	prog->mainn=am;
	return prog;
}
A_main A_Main(A_argList vas,A_cseqs cs,A_rettype rt){
	A_main m=checked_malloc(sizeof(*m));
	m->cseqs=cs;
	m->ret=rt;
	m->valargs=vas;
	return m;
}
A_ftail  A_Ftail(A_f f,A_oper op,A_ftail tf,int pos){
	A_ftail ft=checked_malloc(sizeof(*ft));
	ft->oper=op;
	ft->ftail=tf;
	ft->f=f;
	ft->pos=pos;
	return ft;
}

A_f  A_Symf(symbol sy,int pos){
	A_f f=checked_malloc(sizeof(*f));
	
	f->kind=A_syf;
	f->u.sym=sy;
	f->pos=pos;
	return f;
}

A_f A_Expf(A_exp exp,int pos){
	A_f f=checked_malloc(sizeof(*f));
	f->kind=A_expf;
	f->u.exp=exp;
	f->pos=pos;
	return f;
}
A_f A_Floatf(float a,int pos){
	A_f f=checked_malloc(sizeof(*f));
	f->kind=A_floatf;
	f->u.floatt=a;
	f->pos=pos;
	return f;
}

A_f A_Intf(int a,int pos){
	A_f f=checked_malloc(sizeof(*f));
	f->kind=A_intf;
	f->u.intt=a;
	f->pos=pos;
	return f;
}

A_f  A_Callf(symbol sym,A_valArgs vals,int pos){
	A_f f=checked_malloc(sizeof(*f));
	f->kind=A_callf; 
	f->u.call.funname=sym;
	f->u.call.vals=vals;
	f->pos=pos;
	return f;	
}

A_t A_T(A_f f,A_ftail ft,int pos){
	A_t t =checked_malloc(sizeof(*t));
	t->f=f;
	t->ftail=ft;
	t->pos=pos;
	return t;
}
A_ttail A_Ttail(A_t t,A_oper op,A_ttail tt,int pos){
	A_ttail tts=checked_malloc(sizeof(*t));
	tts->oper=op;
	tts->t=t;
	tts->ttail=tt;
	tts->pos=pos;
	return tts;
}

A_constDecList A_ConstDecList(A_constDec cd,A_constDecList cdl){
	A_constDecList c=checked_malloc(sizeof(*c));
	c->head=cd;
	c->tail=cdl;
	return c;
}

A_constDec  A_ConstDec(A_type ty,symbol sy,A_constval cv,int pos){
	A_constDec cdc=checked_malloc(sizeof(*cdc));
	cdc->pos=pos;
	cdc->sym=sy;
	cdc->type=ty;
	switch(ty){
		case INT_TY:
			cdc->u.intt=cv.num;
			break;
		case FLOAT_TY:
			cdc->u.floatt=cv.flo;
			break;
		case STRING_TY:
			cdc->u.stringg=cv.stringg;
			break;
	}
	return cdc;
}

A_varDecList A_VarDecList(A_varDec vd,A_varDecList vdd){
	A_varDecList vdl=checked_malloc(sizeof(*vdl));
	vdl->head=vd;
	vdl->tail=vdd;
	return vdl;
}

A_varDec     A_VarDec(A_type ty,symbol sy,int pos){
	A_varDec vd =checked_malloc(sizeof(*vd));
	vd->pos=pos;
	vd->sym=sy;
	vd->type=ty;
	return vd; 	
} 

A_funcDecList A_FuncDecList(A_funcDec fdd,A_funcDecList fddd){
	A_funcDecList fd=checked_malloc(sizeof(*fd));
	fd->head=fdd;
	fd->tail=fddd;
	return fd;
}

A_funcDec    A_FuncDec(A_rettype ty,symbol sym,A_argList al,A_cseqs cq,int pos){
	A_funcDec fd=checked_malloc(sizeof(*fd));
	fd->agrlist=al;
	fd->cseqs=cq;
	fd->rettype=ty;
	fd->pos=pos;
	fd->name=sym;
	return fd; 
}

A_argList     A_ArgList(A_arg a,A_argList al,int pos){
	A_argList ad=checked_malloc(sizeof(*ad));
	ad->head=a;
	ad->tail=al;
	ad->pos=pos;
	return ad;
}
A_arg         A_Arg(A_type ty ,symbol sy,int pos){
	A_arg r=checked_malloc(sizeof(*r));
	r->argsy=sy;
	r->pos=pos;
	r->type=ty;	
}

A_cseqs		  A_Cseqs(A_constDecList cdl,A_varDecList vdl,A_seqs sq){
	A_cseqs cs=checked_malloc(sizeof(*cs));
	cs->constdeclist=cdl;
	cs->vardeclist=vdl;
	cs->seqs=sq;
	return cs;
}
A_seqs        A_Seqs(A_seq sq,A_seqs sqs){
	A_seqs se=checked_malloc(sizeof(*se));
	se->seq=sq;
	se->seqs=sqs;
	return se;
}

A_seq         A_IfSeq(A_flag fl,A_seq th,A_seq el,int pos){
	A_seq se=checked_malloc(sizeof(*se));
	se->kind=A_if;
	se->u.iff.flag=fl;
	se->u.iff.thenn=th;
	se->u.iff.elsee=el;
	se->pos=pos;
	return se;
}
A_seq   A_WhileSeq(A_flag fl,A_seq seq,int pos){
	A_seq se=checked_malloc(sizeof(*se));
	se->kind=A_while;
	se->u.whilee.flag=fl;
	se->u.whilee.seq=seq;
	se->pos=pos;
	return se;
}
A_seq    A_SeqsSeq(A_seqs sqs, int pos){
	A_seq se=checked_malloc(sizeof(*se));
	se->kind=A_seqss;
	se->u.sqs=sqs;
	se->pos=pos;
	return se;	
}

A_seq         A_CallSeq(symbol sy,A_valArgs va,int pos){
	A_seq se=checked_malloc(sizeof(*se));
	se->kind=A_call;
	se->pos=pos;
	se->u.call.func=sy;
	se->u.call.valargs=va;
	return se;
}

A_valArgs     A_ValArgs(A_exp exp,A_valArgs va,int pos){
	A_valArgs se=checked_malloc(sizeof(*se));
	se->head=exp;
	se->pos=pos;
	se->tail=va;
	return se;	
}
A_seq         A_AssSeq(symbol sy,A_exp exp,int pos){
	A_seq se=checked_malloc(sizeof(*se));
	se->kind=A_ass;
	se->pos=pos;
	se->u.ass.exp=exp;
	se->u.ass.sym=sy;
	return se;
}

A_seq    A_RetSeq(A_exp exp,int pos){
	A_seq se=checked_malloc(sizeof(*se));
	se->kind=A_ret;
	se->pos=pos;
	se->u.ret.exp=exp;
	return se;	
}

A_seq A_ScanSeq(symbol sy,int pos){
	A_seq se=checked_malloc(sizeof(*se));
	se->kind=A_scan;
	se->pos=pos;
	se->u.scan.sym=sy;
	return se;
}

A_seq  A_PrintSeq(char* s,A_exp exp,int pos){
	A_seq se=checked_malloc(sizeof(*se));
	se->kind=A_print;
	se->pos=pos;
	se->u.print.exp=exp;
	if(s){
		se->u.print.stringg=checked_malloc((strlen(s)+3));
		strcpy(se->u.print.stringg,s);
	}else{
		se->u.print.stringg=NULL;
	}
	return se;
}

A_flag        A_Flag(A_exp left,A_com com,A_exp right,int pos){
	A_flag f=checked_malloc(sizeof(*f));
	f->com=com;
	f->left=left;
	f->right=right;
	f->pos=pos;
	return f;
}

A_exp  A_Exp(A_oper oper,A_t t,A_ttail tt,int pos){
	A_exp ex=checked_malloc(sizeof(*ex));
	ex->pos=pos;
	ex->t=t;
	ex->ttail=tt;
	ex->oper=oper;
	return ex;	
}

A_constval A_ConstNum(int a){
	/*
	认为union 类型是传值的 
	*/
	A_constval t;
	t.num=a;
	return t;
}
A_constval A_ConstFloat(float a){
	A_constval t;
	t.flo=a;
	return t;	
}
A_constval A_ConstString(char *s){
	A_constval t;
	t.stringg=checked_malloc(50);// 最长的常量字节是50 
	strcpy(t.stringg,s);
	return t; 
}

A_type find_type_f(TAB_table env,A_f f){
 
	switch(f->kind){
		case A_expf:
			return find_type(env,f->u.exp);
		case A_intf:
			return INT_TY;
		case A_floatf: 
			return FLOAT_TY;
		case A_syf:
			{
				E_entry entry=S_look(env,f->u.sym);
				if(entry &&entry->kind==var){
					return entry->u.var.ty; 
				}
				return -1; 
			}
		case A_callf:
			{
				E_entry entry=S_look(env,f->u.call.funname);
				if(entry &&entry->kind==fun){
					A_type ty=ret2type(entry->u.fun.ty);
					
					if(ty==-1){
						return -1;
					}
					
					if(ty==FLOAT_TY||ty==INT_TY){
						return ty;
					} 
				}else{
					assert(0);
				}
			}
		default :
			return -1;		
	}
} 

 A_type find_type_t(TAB_table env,A_t t){
	A_type ty= find_type_f(env,t->f);
	if(ty==-1){
		return -1;
	}
	
	if(ty==FLOAT_TY){
		return FLOAT_TY;
	}
	
	A_ftail ft=t->ftail;
	while(ft){
		A_type ty2 = find_type_f(env,ft->f);
		if(ty2==-1){
			return -1;
		}
		
		if(ty2==FLOAT_TY){
			return FLOAT_TY;
		}
		
		ft=ft->ftail;
	} 
	return INT_TY;
}

A_type find_type(TAB_table env,A_exp exp){
	A_type ty=find_type_t(env,exp->t);
	
	if(ty==-1){
		return -1;
	}
	
	if(ty==FLOAT_TY){
		return FLOAT_TY;
	}
	
	if(exp->ttail){
		A_ttail p=exp->ttail;
		while(p){
			A_type ty2=find_type_t(env,p->t);
			
			if(ty2==-1){
				return -1;
			}
			
			if(ty2==FLOAT_TY){
				return FLOAT_TY;
			}
			p=p->ttail; 
		}
	}
	return INT_TY;
}
