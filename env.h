typedef struct entry_* E_entry; 

struct entry_{
	enum{
		var,fun
	}kind; 
	union{
		struct{
			symbol varr;
			A_type ty;  //±äÁ¿ 
			int isconst;
			A_constval val;
		}var; 

 		struct{    //º¯Êı
		 	symbol func; 
			A_argList arglist;	
			A_rettype ty;
		}fun;				
	}u;
}; 

E_entry VarEntry(symbol va,A_type ty,int isconstval,A_constval t);
E_entry FuncEntry(symbol,A_rettype,A_argList);

