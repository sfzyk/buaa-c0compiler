//用于管理符号表
#include "util.h" 
#include "table.h"
#include "symbol.h"
#include "absyn.h"
#include "penv.h"
P_entry PvarEntry(symbol va,A_type ty,int isconst,A_constval val,int lev,int offset){
	P_entry en=checked_malloc(sizeof(*en));
	en->kind=var;
	en->u.var.ty=ty;
	en->u.var.varr=va;
	en->u.var.isconst=isconst;
	en->u.var.val=val;
	en->u.var.lev=lev;
	en->u.var.offset=offset;
	return en;
}

P_entry PfuncEntry(symbol func,A_rettype ret,A_argList al,int lev,int start){
	P_entry en=checked_malloc(sizeof(*en));
	en->kind=fun;
	en->u.fun.arglist=al;
	en->u.fun.func=func;
	en->u.fun.ty=ret;
	en->u.fun.lev=lev;
	
	en->u.fun.start=checked_malloc(sizeof(int));
	*en->u.fun.start=start;
	return en; 
}




