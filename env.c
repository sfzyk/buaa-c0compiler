//用于管理符号表
#include "util.h" 
#include "table.h"
#include "symbol.h"
#include "absyn.h"
#include "env.h"

E_entry VarEntry(symbol va,A_type ty,int isconst,A_constval val){
	E_entry en=checked_malloc(sizeof(*en));
	en->kind=var;
	en->u.var.ty=ty;
	en->u.var.varr=va;
	en->u.var.isconst=isconst;
	en->u.var.val=val;
	return en;
}

E_entry FuncEntry(symbol func,A_rettype ret,A_argList al){
	E_entry en=checked_malloc(sizeof(*en));
	en->kind=fun;
	en->u.fun.arglist=al;
	en->u.fun.func=func;
	en->u.fun.ty=ret;
	return en; 
}




