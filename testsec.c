#include <stdio.h>

#include "util.h"
#include "c0error.h"
#include "table.h"
#include "symbol.h"
#include "absyn.h"
#include "semant.h"
#include "print.h"
#include "topcode.h"

int main(int argc,char *argv[]){
	freopen(argv[1],"r",stdin);
	A_prog p=proc_prog();	
	getpcode(p);
	outputpcode();
}
