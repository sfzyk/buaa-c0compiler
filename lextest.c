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
		printf("�ؼ���               ");
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
		printf("�ӷ������           "); 
		switch(sym){
			case ADDSY:
				printf("+");break;
			case MINUSSY:
				printf("-");break;
		}
	}else if(MUSY<=sym && sym<=DISY){
		printf("�˷������           "); 
		switch(sym){
			case MUSY:
				printf("*");break;
			case DISY:
				printf("/");break;
		}
	}else if(EQSY<=sym && sym<=NESY){
		printf("�Ƚ������           "); 
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
		printf("�ֽ��               ");
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
		printf("��ʶ��               ");printf(tokeninfo.u.str);
	}else if(sym==INT){
		printf("����                 ");printb(tokeninfo.u.num);// ��֪��ΪʲôҪ����������
		printf("(������)"); 
	}else if(sym==FLOAT){
		printf("������               ");printf("%lf",tokeninfo.u.f); 
	}else if(sym==STRING){
		printf("�ַ���               ");printf("%s",tokeninfo.u.str);
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	assert(argc == 2);
	freopen(argv[1],"r",stdin);
	printf("����                ֵ\n");
	int ret;
	while(ret=get()){
		if(ret==1){
			analyse();
		}
	} 
	return 0;
}





