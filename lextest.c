#include <assert.h> 
#include <stdio.h>
#include "c0lex.h"

void printb(int a){
	int m=a%2;
	a=a/2;
	if(a)printb(a);
	printf("%d",m);
}

void analyse(){
	Symbol sym=tokeninfo.sym;
	if(CONSTSY<=sym && sym<=RETURNSY){
		printf("关键字               ");
		switch(sym){
			case CONSTSY:
				printf("const");break;
			case INTSY:
				printf("int");break;
			case FLOATSY:
				printf("flaot");break;
			case STRINGSY:
				printf("string");break;
			case VOIDSY:
				printf("void");break;
			case IFSY:
				printf("if");break;
			case ELSESY:
				printf("else");break;
			case WHILESY:
				printf("while");break;
			case MAINSY:
				printf("main");break;
			case SCANFSY:
				printf("scanf");break;
			case ASSSY:
				printf("=");break;
			case RETURNSY:
				printf("return");break;
			case PRINTFSY:
				printf("printf");break;
		}
	}else if(ADDSY<=sym && sym<=MINUSSY){
		printf("加法运算符           "); 
		switch(sym){
			case ADDSY:
				printf("+");break;
			case MINUSSY:
				printf("-");break;
		}
	}else if(MUSY<=sym && sym<=DISY){
		printf("乘法运算符           "); 
		switch(sym){
			case MUSY:
				printf("*");break;
			case DISY:
				printf("/");break;
		}
	}else if(EQSY<=sym && sym<=NESY){
		printf("比较运算符           "); 
		switch(sym){
			case EQSY:
				printf("==");break;
			case GESY:
				printf(">=");break;
			case LESY:
				printf("<=");break;
			case LSY:
				printf("<");break;
			case GSY:
				printf(">");break;
			case NESY:
				printf("!=");break;
		}
	}else if(LPSY<=sym && sym<=SEMISY){
		printf("分界符               ");
		switch(sym){
			case LPSY:
				printf("(");break;
			case RPSY:
				printf(")");break;
			case LPKSY:
				printf("{");break;
			case RPKSY:
				printf("}");break;
			case COMMASY:
				printf(",");break;
			case SEMISY:
				printf(";");break;		
			case ASSSY:
				printf("=");break;
		} 
	}else if(sym==IDSY){
		printf("标识符               ");printf(tokeninfo.u.str);
	}else if(sym==INT){
		printf("整数                 ");printb(tokeninfo.u.num);// 不知道为什么要二进制数据
		printf("(二进制)"); 
	}else if(sym==FLOAT){
		printf("浮点数               ");printf("%lf",tokeninfo.u.f); 
	}else if(sym==STRING){
		printf("字符串               ");printf("%s",tokeninfo.u.str);
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	assert(argc == 2);
	freopen(argv[1],"r",stdin);
	printf("类型                值\n");
	int ret;
	while(ret=get()){
		if(ret==1){
			analyse();
		}
	} 
	return 0;
}





