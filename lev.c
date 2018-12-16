#include "util.h"
#include "lev.h"

lev NewLev(){
	return checked_malloc(sizeof(struct lev_));
}
