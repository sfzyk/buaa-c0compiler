/*
typedef struct tab_ * tab;
typedef struct btab_* btab;

struct tab_{  //符号表 
	char * name;
	int link;
	enum{
		p_var,p_con,p_typ,p_func
	}obj; //标识符种类 
 	enum{
		p_ints,p_floats,p_strings
	}typ;//标识符类型
	int ref;// 函数指向btab中的指针 
	int lev;// 静态层次
	int normal;//bool 变量
	int adr;// 相对于lev的偏移 
};

struct btab_{//分程序表btab
	int last;    //当前层次中说明的最后一个标识符在tab 表中的位置
	int lastpar; // 当前函数的最后一个参数 在tab表中的位置
	int psize;   // 参数及该分程序内务信息   在运行栈中所占的的存储单元总数
	int vsize;   // 局部变量 参数 内务信息   的总大小 
};

tab Tab(char *name,int link,int obj,int typ,int ref,int lev,int normal,int adr);
btab Btab(int last,int lastpar,int psize,int vsize);
*/ 
void topcode(A_prog);
typedef struct pcode_ * pcode;
enum pty{
	LIT,OPR,OPRF,LOD,STO,CAL,PINT,JMP,JPC,RED,WRT,WRTS,MKS,FLT,WRTF
};


struct pcode_{
	enum pty ptype;
	int *x,*y;
};

pcode Pcode(enum pty,int x,int y);
void getpcode(A_prog prog);
void outputpcode();


