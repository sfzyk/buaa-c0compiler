/*
typedef struct tab_ * tab;
typedef struct btab_* btab;

struct tab_{  //���ű� 
	char * name;
	int link;
	enum{
		p_var,p_con,p_typ,p_func
	}obj; //��ʶ������ 
 	enum{
		p_ints,p_floats,p_strings
	}typ;//��ʶ������
	int ref;// ����ָ��btab�е�ָ�� 
	int lev;// ��̬���
	int normal;//bool ����
	int adr;// �����lev��ƫ�� 
};

struct btab_{//�ֳ����btab
	int last;    //��ǰ�����˵�������һ����ʶ����tab ���е�λ��
	int lastpar; // ��ǰ���������һ������ ��tab���е�λ��
	int psize;   // �������÷ֳ���������Ϣ   ������ջ����ռ�ĵĴ洢��Ԫ����
	int vsize;   // �ֲ����� ���� ������Ϣ   ���ܴ�С 
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


