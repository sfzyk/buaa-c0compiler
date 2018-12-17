typedef struct pentry_* P_entry; 

struct pentry_{
	enum{
		var,fun
	}kind; 
	union{
		struct{
			symbol varr;
			A_type ty;  //±äÁ¿ 
			int isconst;
			A_constval val;
			int lev;
			int offset;
		}var; 

 		struct{    //º¯Êı
		 	symbol func; 
			A_argList arglist;	
			A_rettype ty;
			lev lev;
			int *start;
		}fun;				
	}u;
}; 

P_entry PvarEntry(symbol va,A_type ty,int isconstval,A_constval t,int,int);
P_entry PfuncEntry(symbol,A_rettype,A_argList,lev,int);

