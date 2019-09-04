#include "array.keg.eapp.h"

char messages[5][50] = {"Hello World!", "Hello Keystone!", "Hello Keyedge!"};

void _start(){
	print_messages(3, messages);
	EAPP_RETURN(0);
}

