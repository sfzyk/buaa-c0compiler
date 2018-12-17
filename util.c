#include <stdlib.h>
#include <stdio.h>
void *checked_malloc(int len)
{
	void *p = malloc(len);
 
 	if (!p) {
    	fprintf(stderr,"\n failed malloc \n");
    	exit(1);
 	}
 	return p;
}

