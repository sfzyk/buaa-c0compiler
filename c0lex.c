#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "c0lex.h"
#include "c0error.h"

#define MAX_TOKEN_SIZE (100)

TokenInfo tokeninfo;
static char ch;
static char token[MAX_TOKEN_SIZE];
static int  tp=0;

void escapeChar(){
	if(ch=='n')ch='\n';
	else if(ch=='t')ch='\t';
	else if(ch=='a')ch='\a';
	else if(ch=='0')ch='\0';
	else if(ch=='r')ch='\r';
	else if(ch=='f')ch='\f';
	else if(ch=='v')ch='\v';
	else if(ch=='\'')ch='\'';
	else if(ch=='"')ch='\"';
	else if(ch=='?')ch='\?';
	else if(ch=='\\')ch='\\';
}

void getch(){
	ch = getchar();
	tokPos++;
}

void retract(){
	ungetc(ch,stdin);
	tokPos--;
}

int isLetter(){
	return isalpha(ch);
}
void clearToken(){
	tp=0;
	memset(token,0,sizeof(token));
}

void clearTokenInfo(){
	memset(tokeninfo.u.str,0,sizeof(tokeninfo.u.str));
}

void catToken(){
	if(tp>=MAX_TOKEN_SIZE){
		error(tokPos,"token size over flow");
		return;
	}
	
	token[tp]=ch;
	tp++;
}

int isEnd(){
	return (int)ch==-1;
}

int isSpace(){
	return ch==' ';
}

int isTab(){
	return ch=='\t';
}

int isNewline(){
	if(ch=='\n'){
		newline();
	}
	
	return ch=='\n';
}
int isDigit(){
	return isdigit(ch);
}


int reserver(){
	if(strcmp("const",token)==0){
		return CONSTSY;
	}
	if(strcmp("int",token)==0){
		return INTSY;
	}
	if(strcmp("if",token)==0){
		return IFSY;
	}
	if(strcmp("else",token)==0){
		return ELSESY;
	}
	if(strcmp("void",token)==0){
		return VOIDSY;
	}
	if(strcmp("while",token)==0){
		return WHILESY;
	}
	if(strcmp("main",token)==0){
		return MAINSY;
	}
	if(strcmp("return",token)==0){
		return RETURNSY;
	}
	if(strcmp("printf",token)==0){
		return PRINTFSY;
	}
	if(strcmp("scanf",token)==0){
		return SCANFSY;
	}
	if(strcmp("float",token)==0){
		return FLOATSY;
	}
	if(strcmp("string",token)==0){
		return STRINGSY;
	}
	
	return 0;
}

int transNum(){
	return atoi(token);
}
float transFloat(){
	return atof(token);
}

int isQuo(){
	return ch=='"';
}

int isLpark(){
	return ch=='{';
} 
int isRpark(){
	return ch=='}';
} 
int isGt(){
	return ch=='>';
}
int isLt(){
	return ch=='<';
}

int isNe(){
	return ch=='!';
} 
int isUnl(){
	return ch=='_';
}

int isEqu(){
	return ch=='=';
}
int isComma(){
	return ch==',';
}
int isPlus(){
	return ch=='+';
}
int isMinus(){
	return ch=='-';
}
int isSemi(){
	return ch==';';
}
int isMul(){
	return ch=='*';
}
int isBack(){
	return ch=='\\';
}

int isLpar(){
	return ch=='(';
}
int isRpar(){
	return ch==')';
}
int isStar(){
	return ch=='*';
}
int isDivi(){ //这个主要判断注释 
	return ch=='/';
}
int isDot(){
	return ch=='.';
}
int isRet(){
	return ch=='\r';
}

int get(){
	ch = ' '; 
	clearToken();
	clearTokenInfo();
	while(isSpace() | isNewline() | isTab()|isRet()){
		getch();
	}
	
	if(isEnd()){
		tokeninfo.sym=EOFSY;
		return 0; // 返回0表示正常结束 
	}
	else if(isLetter()|isUnl()){
		while(isLetter()||isDigit()||isUnl()){
			catToken();
			getch();
		}
		
		retract(); 
		int resultValue=reserver();
		if(resultValue==0){
			tokeninfo.sym = IDSY;
			strcat(tokeninfo.u.str,token);
		}
		else tokeninfo.sym=resultValue;		
	}
	else if(isDigit()){
		while(isDigit()){
			catToken();
			getch();
		}
		
		if(isDot()){
			catToken();
			getch();
			while(isDigit()){
				catToken();
				getch();
			}
			retract();
			tokeninfo.u.f =transFloat();
			tokeninfo.sym =FLOAT;
		}
		else{
			retract();
			tokeninfo.u.num=transNum(token);
			tokeninfo.sym = INT;
		}
	}
	else if(isGt()){
		getch();
		if(isEqu()){
			tokeninfo.sym=GESY;
		}else{
			retract();
			tokeninfo.sym=GSY;
		}
	}
	else if(isLt()){
		getch();
		if(isEqu()){
			tokeninfo.sym=LESY;
		}else{
			retract();
			tokeninfo.sym=LSY;
		}
	}
	else if(isNe()){
		getch();
		if(isEqu()){
			tokeninfo.sym=NESY;
		}else{
			error(tokPos,"expected = here (not !=)");
		}
		
	}else if(isEqu()){
		getch();
		if(isEqu()){
			tokeninfo.sym=EQSY;
		}else{
			tokeninfo.sym=ASSSY;
			retract();
		}
	}
	else if(isMinus())tokeninfo.sym = MINUSSY;
	else if(isPlus())tokeninfo.sym= ADDSY;

	
	else if(isLpar())tokeninfo.sym= LPSY;
	else if(isRpar())tokeninfo.sym= RPSY; //括号 
	
	else if(isLpark())tokeninfo.sym= LPKSY;
	else if(isRpark())tokeninfo.sym= RPKSY;//大括号 
	
	else if(isComma())tokeninfo.sym=COMMASY;
	else if(isSemi())tokeninfo.sym=SEMISY;
	else if(isMul())tokeninfo.sym=MUSY;
	else if(isDivi()){
		getch();
		if(isStar()){
			do{
				do{
					getch();
				}while(!isStar());
				
				do{
					getch();
					if(isDivi())return -1;
				}while(isStar());
				
			}while(!isStar());
		}
		retract();
		tokeninfo.sym=DISY;
	}else if(isQuo()){
		getch();
		while(!isQuo()){
			if(isBack()){
				getch();
				escapeChar();
				catToken();
			}else{
				catToken();
			}
			getch();
			if(isEnd()){
				error(tokPos,"string doesn't end");
				return 0;
			}
		}
		tokeninfo.sym=STRING;
		strcpy(tokeninfo.u.str,token); 
	}
	else{
		char mes[20]="unknown char ";
		mes[13]=ch;mes[14]='\0';
		error(tokPos,mes);
		return -1;
	}
	return 1;
}
