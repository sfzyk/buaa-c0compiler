//抽象语法树 有助于较晚阶段的编译实现 
typedef struct A_prog_* A_prog;
typedef struct A_constDecList_* A_constDecList;
typedef struct A_constDec_* A_constDec; 
typedef struct A_varDecList_* A_varDecList;
typedef struct A_varDec_* A_varDec;
typedef struct A_funcDecList_* A_funcDecList;
typedef struct A_funcDec_* A_funcDec;
typedef struct A_argList_* A_argList;

typedef struct A_arg_* A_arg;
typedef struct A_cseqs_* A_cseqs;
typedef struct A_seqs_* A_seqs;
typedef struct A_seq_* A_seq;
typedef struct A_flag_* A_flag;
typedef struct A_valArgs_* A_valArgs;
typedef struct A_exp_* A_exp;

typedef struct A_ttail_* A_ttail;
typedef struct A_t_* A_t;
typedef struct A_f_* A_f;
typedef struct A_ftail_* A_ftail;
typedef struct A_main_* A_main;

typedef enum{
	INT_TY,FLOAT_TY,STRING_TY
}A_type;

typedef enum{
	INT_RETTY,STRING_RETTY,FLOAT_RETTY,VOID_RETTY
}A_rettype;

typedef enum{
	A_add,A_minus,A_mul,A_div 
}A_oper;

typedef enum{
		A_GE,A_LE,A_NE,A_EQ,A_L,A_G
}A_com;

typedef union{
	int num;
	float flo;
	char *stringg; 
}A_constval; 

A_type ret2type(A_rettype);
A_rettype    type2ret(A_type);

A_prog    A_Prog(A_constDecList,A_varDecList,A_funcDecList,A_main);
A_main    A_Main(A_argList,A_cseqs,A_rettype);

A_f       A_Symf(symbol,int);
A_f 	  A_Expf(A_exp,int);
A_f		  A_Intf(int,int);
A_f		  A_Floatf(float,int);
A_f       A_Callf(symbol,A_valArgs,int);

A_ftail   A_Ftail(A_f,A_oper,A_ftail,int);

A_t 	  A_T(A_f,A_ftail,int);
A_ttail   A_Ttail(A_t,A_oper,A_ttail,int);

A_constDecList A_ConstDecList(A_constDec,A_constDecList);
A_constDec     A_ConstDec(A_type,symbol,A_constval,int);

A_varDecList A_VarDecList(A_varDec,A_varDecList);
A_varDec     A_VarDec(A_type,symbol,int); 

A_funcDecList A_FuncDecList(A_funcDec,A_funcDecList);
A_funcDec     A_FuncDec(A_rettype,symbol sym,A_argList,A_cseqs,int);

A_argList     A_ArgList(A_arg,A_argList,int);
A_arg         A_Arg(A_type,symbol,int);

A_cseqs		  A_Cseqs(A_constDecList,A_varDecList,A_seqs);
A_seqs        A_Seqs(A_seq,A_seqs);

A_seq         A_IfSeq(A_flag,A_seq,A_seq,int);
A_seq         A_WhileSeq(A_flag,A_seq,int);
A_seq         A_SeqsSeq(A_seqs,int);
A_seq         A_CallSeq(symbol,A_valArgs,int);
A_valArgs     A_ValArgs(A_exp,A_valArgs,int);
A_seq         A_AssSeq(symbol,A_exp,int);
A_seq         A_RetSeq(A_exp,int);
A_seq         A_ScanSeq(symbol,int);
A_seq         A_PrintSeq(char*,A_exp,int);

A_flag        A_Flag(A_exp,A_com,A_exp,int);
A_exp         A_Exp(A_oper,A_t,A_ttail,int);

A_constval A_ConstNum(int a);
A_constval A_ConstFloat(float a);
A_constval A_ConstString(char *s);

struct A_prog_{ //程序 
	A_constDecList constdeclist;
	A_varDecList vardeclist;
	A_funcDecList fundeclist; 
	A_main mainn;
};

struct A_constDecList_{
	A_constDec head;
	A_constDecList tail;
};

struct A_constDec_{
	A_type type;
	symbol sym;
	union{
		int intt;
		float floatt;
		char *stringg;
	}u;
	int pos;
};

struct A_varDecList_{
	A_varDec head;
	A_varDecList tail;
};

struct A_varDec_{
	A_type type;
	symbol sym;
	int pos;
};



struct A_funcDecList_{
	A_funcDec head;
	A_funcDecList tail;	
};

struct A_funcDec_{
	A_rettype rettype;
	symbol name;
	A_argList agrlist;
	A_cseqs cseqs;// 复合语句 
	int pos;
};

struct A_argList_{
	A_arg head;
	A_argList tail;	
	int pos;		
};

struct A_arg_{
	A_type type;
	symbol argsy;
	int pos;
};

struct A_cseqs_{
	A_constDecList constdeclist;
	A_varDecList vardeclist;
	A_seqs seqs;
};


struct A_seqs_{
	A_seq seq;
	A_seqs seqs; 
};

struct A_seq_{
	enum{
		A_if,A_while,A_seqss,A_call,A_ass,A_ret,A_scan,A_print,A_null
	}kind;
	
	union{
		struct{
			A_flag flag;
			A_seq thenn;
			A_seq elsee;
		}iff;
		
		struct{
			A_flag flag;
			A_seq seq;
		}whilee;
		
		struct{
			symbol func;
			A_valArgs valargs;
		}call;
		
		struct{
			symbol sym;
		}scan;
		
		struct{
			char * stringg;
			A_exp exp;			
		}print;
		
		struct{
			symbol sym;
			A_exp exp;
		}ass;
		
		struct{
			A_exp exp;
		}ret;
		
		A_seqs sqs;
	}u;
	int pos;
};
// right ==null 也是允许的 


struct A_flag_{
	A_exp left;
	A_com com;
	A_exp right;
	int pos;
};

struct A_valArgs_{
	A_exp head;
	A_valArgs tail;
	int pos;
}; 

struct A_exp_{
	A_oper oper;//only be add or minus
	A_t t;
	A_ttail ttail;
	int pos;
};

struct A_ttail_{
	A_oper oper;//only be mul or div
	A_t t;
	A_ttail ttail;
	int pos;
};


struct A_t_{
	A_f f;
	A_ftail ftail;
	int pos;
};


struct A_f_{
	enum A_factor{A_syf,A_expf,A_intf,A_floatf,A_callf}kind;
	union{
		symbol sym;
		A_exp exp;
		int intt;
		float floatt;
		struct {
			symbol funname;
			A_valArgs vals;
		}call;
	}u;
	int pos;
};

struct A_ftail_{
	A_oper oper;
	A_f f;
	A_ftail ftail;
	int pos;
}; 

struct A_main_{
	A_rettype ret;
	A_argList valargs;
	A_cseqs cseqs;
};
A_type find_type_f(TAB_table,A_f); 
A_type find_type_t(TAB_table,A_t);
A_type find_type(TAB_table,A_exp);
