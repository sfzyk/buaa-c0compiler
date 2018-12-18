#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
 
#define MAX_LEN (200)

void *checked_malloc(int len)
{
	void *p = malloc(len);
 
 	if (!p) {
    	fprintf(stderr,"\n failed malloc \n");
    	exit(1);
 	}
 	return p;
}

typedef struct pcode_ * pcode;


enum pty{
	LIT,OPR,OPRF,LOD,STO,CAL,PINT,JMP,JPC,RED,WRT,WRTS,MKS,FLT,WRTF
};

struct pcode_{
	enum pty ptype;
	int *x,*y;
};

pcode Pcode(enum pty pp,int x,int y){
	pcode p=checked_malloc(sizeof(*p));
	p->x=checked_malloc(sizeof(int));
	p->y=checked_malloc(sizeof(int));
	*(p->x)=x;
	*(p->y)=y;
	p->ptype=pp;
	return p; 
}
static pcode intr[2000];
static int S[50000];
static int ip;
static int  b=0,t=0;
char buffer[MAX_LEN];

int doit(){
	pcode p =intr[ip];
	int x=*p->x;
	int y=*p->y;
	switch(p->ptype){
		case LIT:{
			S[t]=y;
			t++;
			break;
		}
		case LOD:{
			int pb=b;
			while(x){
				pb=S[pb+2]; 
				x--;
			}
			S[t]=S[pb+y];
			t=t+1;
			break;
		}
		case STO:{
			int pb=b;
			while(x){
				pb=S[pb+2]; 
				x--;
			}
			S[pb+y]=S[t-1];
			t=t-1;
			break;
		}
		case MKS:{
			S[t+1]=b; //动态链
			S[t+2]=b; //看起来很奇怪，但是是必要的 
			b=t;
			t=t+4+y;
			break;
		}
		case CAL:{
			int pb=b;
			while(x>-2){
				pb=S[pb+2]; 
				x--;
			}
			S[b+3]=ip+1;
			S[b+2]=pb;
			ip=y;
			return 1;
			//to do
			break;
		}
		case PINT:{
			t=t+y;
			break;
		}
		case JMP:{
			ip=y;
			return 1;
		}
		case JPC:{
			if(S[t-1]==0){
				ip=y;
				t=t-1;
				return 1;
			}
			t=t-1;
			break;
		}
		case RED:{
			scanf("%s",buffer);
			int l=strlen(buffer);
			int f=0;
			for(int i=0;i<l;i++){
				if(buffer[i]=='.'){
					f=1;
				}
			}
			
			int pb=b;
			while(x){
				pb=S[pb+2]; 
				x--;
			}
		 
			
			float te;
			int ta;
			if(f==1){
				te=atof(buffer);
				S[pb+y]=*(int *)&te;
			}else{
				ta=atoi(buffer);
				S[pb+y]=ta;
			}
			break;
		}
		case WRT:{
			printf("%d",S[t-1]);
			t=t-1;
			break;
		}
		case WRTF:{
			printf("%f",*(float *)&S[t-1]);
			t=t-1;
			break;
		}
		case WRTS:{
			printf("%c",S[t-1]);
			t=t-1;
			break;
		}
		case FLT:{
			if(x==0){
				float te=S[t-1];
				S[t-1]=*(int*)&te;
			}else{
				int a=*(float *)&S[t-1];
				S[t-1]=a;
			}
			break;
		}
		case OPR:{
			switch(y){
				case 0:{
					S[b]=S[t-1];
					t=b+1;
					ip=S[b+3];
					b=S[b+1];
					
					if (ip==-1 || ip==0 ){
						return 0;
					}
					return 1;
				}
				case 2:{
					S[t-2]=S[t-2]+S[t-1];
					t=t-1;
					break;
				}
				case 3:{
					S[t-2]=S[t-2]-S[t-1];
					t=t-1;
					break;
				}
				case 4:{
					S[t-2]=S[t-2]*S[t-1];
					t=t-1;
					break;
				}
				case 5:{
					S[t-2]=S[t-2]/S[t-1];
					t=t-1;
					break;
				} 
				case 9:{
					if(S[t-2]==S[t-1]){
						S[t-2]=0;
					}
					else S[t-2]=1;
					t=t-1;
					break;
				}
				case 8:{
					if(S[t-2]!=S[t-1]){
						S[t-2]=0;
					}
					else S[t-2]=1;
					t=t-1;
					break;
				}
				case 10:{
					if(S[t-2]>=S[t-1]){
						S[t-2]=0;
					}
					else S[t-2]=1;
					t=t-1;
					break;	
				}
				case 11:{
					if(S[t-2]<=S[t-1]){
						S[t-2]=0;
					}
					else S[t-2]=1;
					t=t-1;
					break;
				}
				case 12:{
					if(S[t-2]<S[t-1]){
						S[t-2]=0;
					}
					else S[t-2]=1;
					t=t-1;		
					break;
				}
				case 13:{
					if(S[t-2]>S[t-1]){
						S[t-2]=0;
					}
					else S[t-2]=1;
					t=t-1;
					break;
				}
			}
			break;
		}
		case OPRF:{
		switch(y){
			case 0:{
				printf("OPRF not ret intr");
				assert(0);
				return 1;
			}
			case 2:{
				float te=(*(float *)&S[t-2]+*(float *)&S[t-1]);
				S[t-2]=*(int *)&te;
				t=t-1;
				break;
			}
			case 3:{
				float te=(*(float *)&S[t-2]-*(float *)&S[t-1]);
				S[t-2]=*(int *)&te;
				t=t-1;
				break;
			}
			case 4:{
				float te=( (*(float *)&S[t-2])*(*(float *)&S[t-1]));
				S[t-2]=*(int *)&te;
				t=t-1;
				break;
			}
			case 5:{
				float te=((*(float *)&S[t-2])/(*(float *)&S[t-1]));
				S[t-2]=*(int *)&te;
				t=t-1;
				break;
			} 
			case 9:{
				if(S[t-2]==S[t-1]){
					S[t-2]=0;
				}
				else S[t-2]=1;
				t=t-1;
				break;
			}
			case 8:{
				if(S[t-2]!=S[t-1]){
					S[t-2]=0;
				}
				else S[t-2]=1;
				t=t-1;
				break;
			}
			case 10:{
				if(*(float *)&S[t-2]>=*(float *)&S[t-1]){
					S[t-2]=0;
				}
				else S[t-2]=1;
				t=t-1;
				break;	
			}
			case 11:{
				if(*(float *)&S[t-2]<=*(float *)&S[t-1]){
					S[t-2]=0;
				}
				else S[t-2]=1;
				t=t-1;
				break;
			}
			case 12:{
				if(*(float *)&S[t-2]<*(float *)&S[t-1]){
					S[t-2]=0;
				}
				else S[t-2]=1;
				t=t-1;		
				break;
			}
			case 13:{
				if(*(float *)&S[t-2]>*(float *)&S[t-1]){
					S[t-2]=0;
				}
				else S[t-2]=1;
				t=t-1;
				break;
			}
		}
		break;
	}
	}
	ip++;
	return 1;
 }

int main(int argc , char *argv[]){
	
	if(argc !=2){
		printf("plase input a file \n");
		return 0;
	}
	
	freopen(argv[1],"r",stdin);
	fgets(buffer,MAX_LEN,stdin);
	if(buffer[0]!='s'){
		printf("no starting adderss avalible\n");
		return 0;
	}
	char num[5];
	strcpy(num,buffer+20);
	int start=atoi(num);
	ip=0;
	
	while(!feof(stdin)){
		fgets(buffer,MAX_LEN,stdin);
		if(feof(stdin))break;
		int x=atoi(buffer+12);
		int y=atoi(buffer+20);
  
		switch(buffer[7]){
			case 'L':{
				if(buffer[8]=='I'){
					intr[ip]=Pcode(LIT,x,y);
				}else if(buffer[8]=='O'){
					intr[ip]=Pcode(LOD,x,y);
				}
				break;
			}
			case 'O':{
				if(buffer[10]=='F'){
					intr[ip]=Pcode(OPRF,x,y);
				}else{
					intr[ip]=Pcode(OPR,x,y);
				} 
				break;
			}
			case 'S':{
				intr[ip]=Pcode(STO,x,y);
				break;
			}
			case 'C':{
				intr[ip]=Pcode(CAL,x,y);
				break;
			}
			case 'I':{
				intr[ip]=Pcode(PINT,x,y);
				break;
			}
			case 'W':{
				if(buffer[10]=='S'){
					intr[ip]=Pcode(WRTS,x,y);
				}
				else if(buffer[10]=='F'){
					intr[ip]=Pcode(WRTF,x,y);
				}else{
					intr[ip]=Pcode(WRT,x,y);
				}
				break;
			}
			case 'J':{
				if(buffer[9]=='C'){
					intr[ip]=Pcode(JPC,x,y); 
				} 
				else if(buffer[9]=='P'){
					intr[ip]=Pcode(JMP,x,y);
				}
				break;
			}
			case 'R':{
				intr[ip]=Pcode(RED,x,y);
				break;
			}
			case 'M':{
				intr[ip]=Pcode(MKS,x,y);
				break;
			}
			case 'F':{
				intr[ip]=Pcode(FLT,x,y);
				break;
			}				
		}
		ip++;
	}
	freopen("con","r",stdin);
	ip=start;
	S[0]=-1;S[3]=0;S[6]=0;
	S[1]=-1;S[4]=0;
	S[2]=-1;S[5]=3;//全局的静态链指向自己 
	//未使用
	b=3;// 基址
	t=7;// 3-7是初始栈帧 
	while(doit());
}




