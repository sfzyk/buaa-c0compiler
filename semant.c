#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "util.h" 
#include "c0error.h"
#include "c0lex.h"
#include "table.h"
#include "symbol.h"
#include "absyn.h"
#include "env.h"
#include "semant.h" 

#define PRE_SIZE (3)  //超前查看量 
#define REAL(pointer,offset) (((pointer)+(offset))%PRE_SIZE) 


struct tokenbuffer{
	TokenInfo tkinfo;
	int	tokpos;
}tokenwrap[PRE_SIZE];

static int pointer=-1; 
static int isbuffered=0;
struct tokenbuffer * tokenp;


void prelook(int offset){ // 超前查看 不读入下一个字符 
	assert(offset >=0 && offset <= PRE_SIZE);
	tokenp =  &tokenwrap[REAL(pointer,offset)];
}


void getnext(){//读入下一个字符,丢弃当前的一个字符 （其实读入的和 tokenp 指向的不一样） 
	
	if(isbuffered==0){
		for(int i=0;i<PRE_SIZE-1;i++){
			int ret = -1;
			while(ret==-1)ret=get();
			tokenwrap[i].tkinfo=tokeninfo;
			tokenwrap[i].tokpos=tokPos; 
		}
		isbuffered=1;	
	}
	
	int ret = -1;
	while(ret==-1)ret=get();
	
	tokenwrap[REAL(pointer,PRE_SIZE)].tkinfo=tokeninfo; // 读入的始终是领先PRE_SIZE 超前查看 
	tokenwrap[REAL(pointer,PRE_SIZE)].tokpos=tokPos;
	pointer=REAL(pointer,1);
	tokenp = &tokenwrap[pointer];
}

A_com proc_op(TAB_table);

A_constDecList proc_constdec_list(TAB_table,A_constDecList);
A_constDec proc_const(TAB_table,A_type);
A_varDecList proc_vardec_list(TAB_table,A_varDecList);
A_varDec proc_vardec(TAB_table,A_type);

A_flag proc_flag(TAB_table env);
A_funcDec proc_funcdec(TAB_table env);
A_argList proc_arg_list(TAB_table env);
A_argList proc_args(TAB_table env);
A_cseqs proc_func_body(TAB_table env,A_rettype);

A_main proc_main(TAB_table);

A_seqs proc_seq_list(TAB_table,A_rettype); 

A_seq proc_seq(TAB_table,A_rettype);
A_seq proc_ifseq(TAB_table,A_rettype);
A_seq proc_whileseq(TAB_table,A_rettype);
A_seq proc_return(TAB_table,A_rettype);
A_seq proc_print(TAB_table,A_rettype);
A_seq proc_scanf(TAB_table,A_rettype);
A_seq proc_assign(TAB_table,A_rettype);
A_seq proc_func(TAB_table,A_rettype);

A_f proc_factor(TAB_table);
A_valArgs proc_callarg(TAB_table,A_argList al);// 要接受参数 


void show(){
	printf("--%d--%s\n",tokenp->tkinfo.sym,tokenp->tkinfo.u.str);
}

static void type_check(TAB_table env,A_argList a,A_valArgs);
static A_constval _NULL_;

static A_constval constnull(){
	return _NULL_;
}
static void type_check(TAB_table env,A_argList a,A_valArgs b){
	int count =0;
	while(a && b){
		if(a->head->type !=find_type(env,b->head)){
			error(a->pos,"funciton arg cannot match defined here");
			error(b->pos,"funciton arg cannot match used here");
		}
		a=a->tail;
		b=b->tail;
	}
	
	if(a && !b){
		error(tokenp->tokpos,"too few for function args ");
	}
	
	if(b && !a){
		error(tokenp->tokpos,"too many for funtion args");
	}
	
}


A_prog proc_prog(){
	TAB_table env=S_empty();
	getnext();
//	show(); 
	

	A_constDecList cdl = NULL;
	A_varDecList vdl =NULL; 
	A_funcDecList fdl =NULL;
	A_main m=NULL;
	
 
	while(tokenp->tkinfo.sym==CONSTSY)cdl=proc_constdec_list(env,cdl); // 解析足够多的 const定义 
 
	
	prelook(2);
	while(tokenp->tkinfo.sym==COMMASY ||tokenp->tkinfo.sym == SEMISY||tokenp->tkinfo.sym == EQSY){ //解析足够多的 变量定义 
		prelook(0);
		vdl = proc_vardec_list(env,vdl);
		prelook(2);
	}
 
 
	while(1){
		prelook(1);
 
		if(tokenp->tkinfo.sym==MAINSY){ //解析主函数 
			prelook(0);
	 
			m=proc_main(env);
		 
			return A_Prog(cdl,vdl,fdl,m);
		}else{
			prelook(0);      
			A_funcDec fd=proc_funcdec(env);        //解析函数 
			fdl=A_FuncDecList(fd,fdl);
		}
	}
} 

/*
	constdec_list
	const int a=2,b=3;
*/
A_constDecList proc_constdec_list(TAB_table env,A_constDecList cdl){
	
	if(tokenp->tkinfo.sym==CONSTSY){
		getnext();
	}else{
		assert(0);
	}
	
	A_type ty;
	switch(tokenp->tkinfo.sym){
		case INTSY: 
			ty=INT_TY;
			getnext();
			break;
		case FLOATSY:
			ty=FLOAT_TY;
			getnext();
			break;
		case STRINGSY:
			ty=STRING_TY;
			getnext();
			break;
		default:
			// 默认是 int 类型的 
			ty=INT_TY; 
			//error(tokenp->tokpos,"unknown const type (int flaot string allow)"); 
			break;
	}
	
	int isfirst=1;
	while(tokenp->tkinfo.sym==COMMASY||isfirst==1){
		if(tokenp->tkinfo.sym==COMMASY)getnext();
		
		A_constDec cd=proc_const(env,ty);
		if(cd){
			cdl = A_ConstDecList(cd,cdl);
		}
		isfirst=0;
	}
		
	if(tokenp->tkinfo.sym==SEMISY){
		getnext();
		return cdl;
	}else{
		error(tokPos,"expected ; here\n");
		getnext();
		return cdl;// 先这样返回这 
	}
}
/*
	a="dfsf"
	正确解析 返回 ConstDec 
	错误解析 返回 NULL 
*/
A_constDec proc_const(TAB_table env,A_type ty){
	/*
	接受参数a  
	*/
	symbol a=Symbol(tokenp->tkinfo.u.str);
	
	getnext();
	if(tokenp->tkinfo.sym==ASSSY){
		getnext();
		/*
		类型检查  送入环境 
		*/ 	
		A_constval t;
		switch(tokenp->tkinfo.sym){
			case INT:
				if(ty!=INT_TY){
					error(tokenp->tokpos,"expect int here");
					t.num=0;
				}else{
					t=A_ConstNum(tokenp->tkinfo.u.num); 
				}
				S_enter(env,a,VarEntry(a,INT_TY,1,t));
			
			break;
			case STRING:
				if(ty!=STRING_TY){
					error(tokenp->tokpos,"expect string here");
					t.stringg=(char *)checked_malloc(2);
					
				}else{
					t=A_ConstString(tokenp->tkinfo.u.str);
				}
				S_enter(env,a,VarEntry(a,STRING_TY,1,t));
			break;			
			case FLOAT:
				if(ty!=FLOAT_TY){
					error(tokenp->tokpos,"expect string here");
					t.flo=0;
				}else{
					t=A_ConstFloat(tokenp->tkinfo.u.f);
				}
				
				S_enter(env,a,VarEntry(a,FLOAT_TY,1,t));
			break;
		}
		A_constDec cd= A_ConstDec(ty,a,t,tokenp->tokpos);	
		getnext(); 
		return cd;
	}else{
		error(tokenp->tokpos,"expected = here");
		return NULL;
	}
}

/*
a="dffd"（舍弃了，报错） 或者 a  因为牵扯到符号表管理，先暂停 
*/
A_varDec proc_vardec(TAB_table env,A_type ty){
	/*
	接受 a 
	*/
	symbol a=Symbol(tokenp->tkinfo.u.str);
	S_enter(env,a,VarEntry(a,ty,0,constnull()));
	A_varDec vd=A_VarDec(ty,a,tokenp->tokpos); 
	
	getnext();
	if(tokenp->tkinfo.sym==ASSSY){
		getnext();
		/*
		接受 a 的值 
		*/
		error(tokenp->tokpos,"var define cannot be init");
		getnext(); 
	}
	return vd;
}
// int a,b,c=2,d;

A_varDecList proc_vardec_list(TAB_table env,A_varDecList vdl){
	A_type ty;
	switch(tokenp->tkinfo.sym){
		case INTSY:
			ty=INT_TY;break;
		case FLOATSY:
			ty=FLOAT_TY;break;
		case STRINGSY:
			ty=STRING_TY;break;
		default:
			error(tokenp->tokpos,"unknown type (int string flaot allowed)");
	}
	getnext();
	/*
		符号表管理定义变量的环境在这里 todo
	*/
	
	int isfirst=1;
	while(tokenp->tkinfo.sym==COMMASY||isfirst==1){
		if(tokenp->tkinfo.sym==COMMASY)getnext();
		
		A_varDec vd= proc_vardec(env,ty);
		vdl =A_VarDecList(vd,vdl);
		
		isfirst=0;
	}
	if(tokenp->tkinfo.sym ==SEMISY){
		getnext();
		return vdl;
	}else{
		error(tokenp->tokpos,"unknow char in variable define");
		getnext();
		return vdl;
	}
}

A_funcDec proc_funcdec(TAB_table env){
	A_rettype ret;
	switch(tokenp->tkinfo.sym){
		case INTSY:
			ret=INT_RETTY;break;
		case FLOATSY:
			ret=FLOAT_RETTY;break;
		case STRINGSY: 
			ret=STRING_RETTY;break;
		case VOIDSY:
			ret=VOID_RETTY;break;
		default:
			error(tokenp->tokpos,"unknown return type for function");
	}
	getnext();
	/*
	接受函数名称 
	*/
	symbol funname=Symbol(tokenp->tkinfo.u.str);
	getnext();
	A_argList al=proc_arg_list(env);
	S_enter(env,funname,FuncEntry(funname,ret,al));
	
	A_cseqs cseqs=proc_func_body(env,ret);	
	
	return A_FuncDec(ret,funname,al,cseqs,tokenp->tokpos);
}

// void main(a,323,44){
//                                                                
//
A_main proc_main(TAB_table env){
	A_rettype ret;

	switch(tokenp->tkinfo.sym){
		case VOIDSY:
			ret = VOID_RETTY;break;
		case INTSY:
			ret=INT_RETTY;break;
		default:
			error(tokenp->tokpos,"invaild return type for main function");
			ret = VOID_RETTY;break;
	}
 
	getnext();
	getnext();
	A_argList als =proc_arg_list(env); 
	/*
		函数信息入栈 
	*/
	A_cseqs cseqs =proc_func_body(env,ret); 
	return A_Main(als,cseqs,ret);
}

/*
(int a,int b,float c)
*/
A_argList proc_arg_list(TAB_table env){
	A_argList al=NULL;
	
	if(tokenp->tkinfo.sym!=LPSY){
		error(tokenp->tokpos,"( expected here");
	}
	
	getnext();
	
	al=proc_args(env);
	
	if(tokenp->tkinfo.sym!=RPSY){
		error(tokenp->tokpos,") expected here");
	}
	getnext();
	
	return al;
}
/*
int a 
float b
*/
A_arg proc_arg(TAB_table env){
	symbol a;
	A_arg arg;
	switch(tokenp->tkinfo.sym){
		case INTSY:
			getnext();
			a=Symbol(tokenp->tkinfo.u.str); 
			arg=A_Arg(INT_TY,a,tokenp->tokpos);
			
			S_enter(env,a,VarEntry(a,INT_TY,0,constnull()));
			
			getnext();
			break;
		case FLOATSY:
			getnext();
			a=Symbol(tokenp->tkinfo.u.str); 
			arg=A_Arg(FLOAT_TY,a,tokenp->tokpos);			
			S_enter(env,a,VarEntry(a,FLOAT_TY,0,constnull()));
			getnext();
			break;
		case STRINGSY:
			getnext();
			a=Symbol(tokenp->tkinfo.u.str); 
			arg=A_Arg(STRING_TY,a,tokenp->tokpos);
			S_enter(env,a,VarEntry(a,STRING_TY,0,constnull()));
			getnext();
			break;
		default:   
			error(tokenp->tokpos,"unknown type in arg");
	}                                       
	return arg;                
}

/*
int a,int b,float c
*/
A_argList proc_args(TAB_table env){
	
	int isfirst =1;
	A_argList al=NULL;
	while(tokenp->tkinfo.sym!=RPSY){
		A_arg a= proc_arg(env);
		al=A_ArgList(a,al,tokenp->tokpos); 
		if(tokenp->tkinfo.sym !=COMMASY && tokenp->tkinfo.sym !=RPSY){
			error(tokenp->tokpos,"expected , or ) here");	
			getnext();
			continue;
		}
		
		if(tokenp->tkinfo.sym ==COMMASY){
				getnext();
		}
	}
	return al;
}

A_cseqs proc_func_body(TAB_table env,A_rettype ret){
	A_constDecList cdl=NULL;
	A_varDecList vdl=NULL;
	A_seqs seqs=NULL;
	A_cseqs cseqs=NULL; 
	
	if(tokenp->tkinfo.sym == LPKSY){
		getnext();
		
	 
		while(tokenp->tkinfo.sym==CONSTSY){
			cdl=proc_constdec_list(env,cdl); // 解析多个 const定义 
		}
	 
		while(tokenp->tkinfo.sym==INTSY||tokenp->tkinfo.sym==FLOATSY||tokenp->tkinfo.sym==STRINGSY){
			vdl=proc_vardec_list(env,vdl);
		}
 
 
		seqs=proc_seq_list(env,ret);
	 
		cseqs = A_Cseqs(cdl,vdl,seqs); 
	 
		
		if(tokenp->tkinfo.sym ==RPKSY){
			getnext();
			return cseqs;
		}else{
			error(tokenp->tokpos,"func body } misssing ");
			getnext();
			return cseqs;
		}
		
	}else{
		error(tokenp->tokpos,"unknown function deimilter"); 
		return cseqs;
	}
}

// 一个{} 算作一个层次 局部变量的区间 
// 返回一个seqs :代表 
A_seqs proc_seq_list(TAB_table env,A_rettype ret){
	beginScope(env);
	
	A_seqs seqs=NULL;
	int count =0;
	
	while(tokenp->tkinfo.sym!= RPKSY){
		count=count+1;
//		printf("%d\n",count);
		A_seq seq=proc_seq(env,ret);
//		printf("%d end\n",count);
		seqs =A_Seqs(seq,seqs);
	}

	endScope(env); 

	return seqs;
}

A_seq proc_seq(TAB_table env,A_rettype rettype){
	int hasse=1;
	A_seq seq=NULL; 
	switch(tokenp->tkinfo.sym){
		case IFSY:
			seq = proc_ifseq(env,rettype);
			hasse=0;
			break;
		case WHILESY:
			seq =proc_whileseq(env,rettype);
			hasse=0;
			break;			
		case LPKSY:
			hasse=0;
			getnext();
			A_seqs s=proc_seq_list(env,rettype);
			seq= A_SeqsSeq(s,tokenp->tokpos);
						 	
			if(tokenp->tkinfo.sym ==RPKSY){
				getnext();
				return seq;
			}else{
				error(tokenp->tokpos,"seq body } misssing ");
				getnext();
				return seq;
			}
			break;
		case RETURNSY:
			seq=proc_return(env,rettype);break;
		case PRINTFSY:
		 
			seq=proc_print(env,rettype);break;
		case SCANFSY:
			seq=proc_scanf(env,rettype);break;
		default:
				prelook(1);// 真香
				switch(tokenp->tkinfo.sym){
					case ASSSY:
						prelook(0);
						seq=proc_assign(env,rettype);
						break;
					case LPSY:
						prelook(0);
						seq=proc_func(env,rettype); 
						break;
					default:
						prelook(0);
						error(tokenp->tokpos,"unknown seq here");
				}
	}
	
	if(tokenp->tkinfo.sym!=SEMISY &&hasse){
		error(tokenp->tokpos,"; expected here");
	}
	
	if(hasse)getnext();
	return seq;
}

A_t proc_item(TAB_table env){
	
	A_f f=proc_factor(env);
	A_t t=A_T(f,NULL,tokenp->tokpos);
	
	A_ftail ft=NULL,ft2=NULL;
	
	ft=A_Ftail(NULL,0,NULL,tokenp->tokpos);
	
	t->ftail=ft;
	
	while(tokenp->tkinfo.sym==MUSY || tokenp->tkinfo.sym==DISY){
		switch(tokenp->tkinfo.sym){
			case MUSY:
				getnext();
				ft->oper=A_mul;
				break;
			case DISY:
				getnext();
				ft->oper=A_div;
				break;
		}
		ft->f = proc_factor(env);	
		
		ft->ftail=A_Ftail(NULL,0,NULL,tokenp->tokpos);
		ft2=ft;
		ft=ft->ftail;
	}
	
	if(ft2){
		ft2->ftail=NULL;
	}else{
		t->ftail=NULL;
	}
	
	return t;
}

/*
 这里的ttail 是正序的 
*/
A_exp proc_exp(TAB_table env){
	A_oper op=0;
	
	A_t t=NULL;
	A_ttail tt=NULL,tt2=NULL;
	
	if(tokenp->tkinfo.sym==ADDSY || tokenp->tkinfo.sym==MINUSSY){ // 获取符号 
		switch(tokenp->tkinfo.sym){
			case ADDSY:
				getnext();
				op=A_add;
				break;
			case MINUSSY:
				getnext();
				op=A_minus;
				break;
		}
	}
	
	t= proc_item(env);
		
	A_exp exp =A_Exp(op,t,NULL,tokenp->tokpos);
	int count =0;
	
	tt=A_Ttail(NULL,0,NULL,0);
	exp->ttail=tt;
	
	while(tokenp->tkinfo.sym==ADDSY ||tokenp->tkinfo.sym==MINUSSY){
		switch(tokenp->tkinfo.sym){
			case ADDSY:
				getnext();
				tt->oper=A_add;
				break;
			case MINUSSY:
				getnext();
				tt->oper=A_minus;
				break;		
		}
		tt->t=proc_item(env);	
		tt->ttail=A_Ttail(NULL,0,NULL,0);
		tt2=tt;
		tt=tt->ttail;
	}
	
	if(tt2)tt2->ttail=NULL;
	else exp->ttail=NULL;
	
	return exp; 
}

// to do 
A_com proc_op(TAB_table env){
	A_com op=0;
	switch(tokenp->tkinfo.sym){
		case EQSY:
			op=A_EQ; 
			break;
		case NESY:
			op=A_NE;
			break;
		case GESY:
			op=A_GE;
			break;
		case LESY:
			op=A_LE;
			break;
		case GSY:
			op=A_G;
			break;
		case LSY:
			op=A_L;
			break;
		default:
			break;
	}
	return op;
}

/*

处理条件
 
*/
A_flag proc_flag(TAB_table env){
	A_flag fl=NULL;
	
	A_com op=0;
	A_exp expl=NULL;
	A_exp expr=NULL;
	
	expl= proc_exp(env);
	
	if(tokenp->tkinfo.sym==RPSY){
		return A_Flag(expl,op,expr,tokenp->tokpos);
	}
	
	getnext();
	op= proc_op(env);
	expr= proc_exp(env);
	return A_Flag(expl,op,expr,tokenp->tokpos);
}

A_seq proc_ifseq(TAB_table env,A_rettype rettype){
	assert(tokenp->tkinfo.sym==IFSY);
	A_flag flag=NULL;
	A_seq thenn=NULL;
	A_seq elsee=NULL;
	
	getnext();
	if(tokenp->tkinfo.sym!=LPSY){
		error(tokenp->tokpos,"if seq ( missing");
	}
	getnext();
	flag = proc_flag(env);
	
	if(tokenp->tkinfo.sym!=RPSY){
		error(tokenp->tokpos,"if seq ) missing");
	}
	
	getnext();
	thenn=proc_seq(env,rettype);
	
	if(tokenp->tkinfo.sym==ELSESY){ // else 分支可以有 
		getnext();
		elsee=proc_seq(env,rettype);
	}
	
	return A_IfSeq(flag,thenn,elsee,tokenp->tokpos);
}


A_seq proc_whileseq(TAB_table env,A_rettype rettype){
	assert(tokenp->tkinfo.sym==WHILESY);
	A_flag flag =NULL;
	A_seq body =NULL;
	
	getnext();
	
	if(tokenp->tkinfo.sym!=LPSY){
		error(tokenp->tokpos,"if seq ( missing");
	}
	
	getnext();
	flag = proc_flag(env);
	
	if(tokenp->tkinfo.sym!=RPSY){
		error(tokenp->tokpos,"if seq ) missing");
	}
	getnext();
	body = proc_seq(env,rettype);
	
	return A_WhileSeq(flag,body,tokenp->tokpos);
}

/*
等待具体的处理 
*/
A_f proc_factor(TAB_table env){
	A_f f;
	switch(tokenp->tkinfo.sym){
		case LPSY:
			{
				A_exp exp =proc_exp(env);
				if(tokenp->tkinfo.sym != RPSY){
					error(tokenp->tokpos,"exp ) missing");
				}
				getnext();
				f=A_Expf(exp,tokenp->tokpos);
				return f;
			}
			break;
		case INT:
			f=A_Intf(tokenp->tkinfo.u.num,tokenp->tokpos);
			getnext();
			return f;
			break;
			
		case FLOAT:
			f=A_Floatf(tokenp->tkinfo.u.f,tokenp->tokpos);
			getnext();
			return f;
			break;
			
		case IDSY:
			{
				/*
				要判断是不是函数调用 要需要进一步获取函数参数等信息 
				*/
				E_entry ent = S_look(env,Symbol(tokenp->tkinfo.u.str));
				if(ent==NULL){
					error(tokenp->tokpos,"unknown id:by defaylt is 0");
					return A_Intf(0,tokenp->tokpos);
				}
				
				if(ent->kind==var){
					getnext();
					return A_Symf(ent->u.var.varr,tokenp->tokpos);
				}else if(ent->kind==fun){ //这里需要手动写 
					getnext();// 越过函数名
					getnext();// 越过左括号 
					A_valArgs vals=proc_callarg(env,ent->u.fun.arglist);
					assert(tokenp->tkinfo.sym == RPSY);
					getnext();//越过右括号 
					return A_Callf(ent->u.fun.func,vals,tokenp->tokpos); 
				}
				
				getnext();
			}
			break;
		default:
			error(tokenp->tokpos,"unknown factor");
			getnext(); 
			return NULL;
	}
}

A_seq proc_return(TAB_table env,A_rettype rettype){
	assert(tokenp->tkinfo.sym==RETURNSY);
	getnext();	
	A_exp exp;
	if(tokenp->tkinfo.sym!=SEMISY){

		if(tokenp->tkinfo.sym!=LPSY){
			error(tokenp->tokpos,"( missing at begin of return exp");
		}
		getnext();
		
		exp= proc_exp(env);
		
		if(tokenp->tkinfo.sym!=RPSY){
			error(tokenp->tokpos,") missing at end of return exp");
		}
		else getnext();
		
		A_type acttype = find_type(env,exp);
		A_rettype ret=ret2type(acttype);
		
		if(ret != rettype){
			error(tokenp->tokpos , "rettype error :no match");
		}
	}
	
	return A_RetSeq(exp,tokenp->tokpos);
}

A_seq proc_print(TAB_table env,A_rettype rettype){
	assert(tokenp->tkinfo.sym==PRINTFSY);
	A_seq seq=NULL;
	A_exp exp=NULL; 
	getnext();
	if(tokenp->tkinfo.sym!=LPSY){
		error(tokenp->tokpos,"( missing at begin of printf");
	}
	getnext();
	char buffer[50];
	while(tokenp->tkinfo.sym!=RPSY){
		 
		int needc =0;
		
		if(tokenp->tkinfo.sym==STRING){
			/*
			获取值 
			*/
	 
			memset(buffer,0,sizeof(buffer));
			strcpy(buffer,tokenp->tkinfo.u.str);

			getnext();
		 
			needc=1; 
		}
 
		/*
		获取 exp 
		*/
		if(tokenp->tkinfo.sym==COMMASY || needc==0){
			if(tokenp->tkinfo.sym==COMMASY)getnext();
			
			exp=proc_exp(env);
		}
 
	}
	
	seq=A_PrintSeq(buffer,exp,tokenp->tokpos);
	
	getnext();	
	return seq;
}

A_seq proc_scanf(TAB_table env,A_rettype rettype){
	assert(tokenp->tkinfo.sym==SCANFSY);
	
	getnext();
	
	if(tokenp->tkinfo.sym!=LPSY){
		error(tokenp->tokpos,"( misssing at end of scanf");
	}
	
	getnext(); 
	symbol sym=Symbol(tokenp->tkinfo.u.str);
 
	E_entry entry =S_look(env,sym);
	
	if(entry==NULL){
		error(tokenp->tokpos,"unknown varivable");
	}
 
	/*
	接受这个标识符  这里可以通过entry 获得这个sym的类型 
 	*/
	getnext();
	
	if(tokenp->tkinfo.sym!=RPSY){
		error(tokenp->tokpos,") missing at end of scanf");
	}
	else getnext();
	
	return A_ScanSeq(sym,tokenp->tokpos); 
}

A_seq proc_func(TAB_table env,A_rettype rettype){
	A_seq seq=NULL;
	/*
	接受函数标识符 
	*/
	symbol funcsym =Symbol(tokenp->tkinfo.u.str);
	
	E_entry entry =S_look(env,funcsym);
	if(!entry || entry->kind!=fun){
		error(tokenp->tokpos,"this cannot be called");
	}
	
	getnext();		
	if(tokenp->tkinfo.sym!=LPSY){
		error(tokenp->tokpos,"( missing at func call");
	}
	
	getnext();
	A_valArgs val = proc_callarg(env,entry->u.fun.arglist);// 要接受参数 
	seq =A_CallSeq(funcsym,val,tokenp->tokpos);
	
	if(tokenp->tkinfo.sym!=RPSY){
		error(tokenp->tokpos,"） missing at func call");
	}
	getnext();
	return seq;
}
/*
	有可能返回 0(NULL) 
*/ 

A_seq proc_assign(TAB_table env,A_rettype rettype){
	/*
	接受标识符 
	*/
	A_seq seq=NULL;
	symbol lsym = Symbol(tokenp->tkinfo.u.str);
	E_entry entry =S_look(env,lsym);
	
	
	if(!entry || entry->kind!=var){
		error(tokenp->tokpos,"not valid left value"); 
		return seq;
	}
	
	if(entry->u.var.isconst==1){
		error(tokenp->tokpos,"const cannot be left value");
	}
		
	getnext();
	if(tokenp->tkinfo.sym!=ASSSY){
		assert(0);
	}
	getnext();
	
	A_exp exp=proc_exp(env);// 接受表达式
	
	A_type ty=find_type(env,exp);
	
	if(ty!=entry->u.var.ty){
		error(tokenp->tokpos,"assign type cannot match");
	}
	
	return A_AssSeq(entry->u.var.varr,exp,tokenp->tokpos);
}



/*
1,3,a+5
*/
A_valArgs proc_callarg(TAB_table env,A_argList al){
	A_exp exp=NULL;
	A_valArgs vgs=NULL;
	
	while(tokenp->tkinfo.sym!=RPSY){
		exp= proc_exp(env);
		vgs =A_ValArgs(exp,vgs,tokenp->tokpos);
		
		if(tokenp->tkinfo.sym==COMMASY){
			getnext();
		}	
	}
	
	type_check(env,al,vgs);
	return 	vgs;
}

