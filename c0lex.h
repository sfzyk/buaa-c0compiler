/*
1  ��������
0  �ļ�����
-1 ���ν��������� ���������ע�� 
*/
int get();

typedef enum SYMBOL{
	PAD,
	
	CONSTSY,INTSY,FLOATSY,STRINGSY,VOIDSY,IFSY,ELSESY,WHILESY,MAINSY,RETURN,PRINTFSY,SCANFSY,RETURNSY, //13
	 
	ADDSY,MINUSSY, //15
	
	MUSY,DISY, //17
	
	EQSY,GESY,LESY,LSY,GSY,NESY, //23
	
	LPSY,RPSY,LPKSY,RPKSY,COMMASY,ASSSY,SEMISY,//30
	
	IDSY, //31
	
	INT,FLOAT,STRING,//34
	
	EOFSY //35
}Symbol_;

typedef struct TOKENINFO{
	Symbol_ sym;
	union{
		int   num;
		float f;
		char  str[100];
	}u;
}TokenInfo;

extern TokenInfo tokeninfo;


