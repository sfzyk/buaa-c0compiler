#sfti 编译器简介#

+ 使用递归下降子程序法
+ 处理c0文法
+ 生成pcode
+ 其他若干特征

##关于pcode 的若干问题##
	使用教材 pxx 的pcode 并不能完全表示c0语法的特征，所以我针对指令集进行了一点~~微小~~的**修改**
	并且对运行模型进行了一定程度的修改，下面就我使用的指令给与说明
	b 栈基址
	t 栈顶指针
	S 栈区
	ip 当前指令指针
	LIT  x,y  将常数y置于栈顶,并t=t+1
	LOD  x,y  将层次差x,offset 为y的变量载入栈顶 t=t+1
	STO  x,y  将栈顶的值存入层次差x,offset为y 的变量中 t=t-1
	MKS  x,y  栈标记指令, y是新栈帧中分配给参数的空间量 完成的动作如下
		S(t+1)=b  //动态连设置
		b=t
		t=t+4+n   //分配相应的参数和栈帧空间
	CAL  x,y 调用指令，
		S(b+3)=ip+1
		S(b+2)=SL
		SL 的获取方式是不断沿着静态链上寻直到找到调用的函数
		ip=y
	INT  x,y  将栈顶增加y
		t=t+y
	JMP  x,y  无条件跳转
		ip=y
	JPC  x,y  有条件跳转
		if S(t-1) == 0 ip = y
	RED  x,y  等待输入，并将输入填入层次差为x offset 为y 的变量
	WRT  x,y  输出栈顶数字
	WRTS x,y  输出栈顶的字符
	FLT  x,y  浮点数转换
		if y==1 将栈顶浮点转换为整形
		if y==0 将栈顶整形转换为浮点
	OPR  x,y  根据y执行不同的运算操作，如下
		 case y==0 :
		 	执行返回指令，将栈顶元素作为返回值返回
		 	S(b)=S(t-1)
		 	t=b+1
		 	ip=S(b+3)
		 	b=S(b+1)
		 case y==2 :
		 	执行add 操作 
		 	S(t-2)=S(t-2)+S(t-1)
		 	t=t-1
		 case y==3 :
		 	执行minus操作
		 	S(t-2)=S(t-2)-S(t-1)
		 	t=t-1
		 case y==4 :
		 	执行乘法操作
		 	S(t-2)=S(t-2)*S(t-1)
		 	t=t-1
		 case y==5 :
		 	执行除法操作
		 	S(t-2)=S(t-2)/S(t-1)
		 	t=t-1
		 case y==8 :
		 	等于 同下
		 case y==9 :
		 	不等于
		 	if S(t-2)==S(t-1)
		 		S(t-2)=0
		 	else 
		 		S(t-2)=1
		 case y==10: 
		  	小于
		  	ifS(t-2) < S(t-1)
		  	 	S(t-2)=1
		  	 else 
		  	 	S(t-1)=0
		 case y=11 
		 	大于 同上
		 case y=12
		 	大于等于
		 case y=13
		 	小于等于


##运行模型##
