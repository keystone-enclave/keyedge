#include "struct.keg.eapp.h"


void _start(){
	message m[5];
	m[0].t.h = 1;	
	m[0].t.m = 2;	
	m[0].t.s = 3;
	m[0].sender[0] = 'a';
	m[0].sender[1] = 'b';
	m[0].sender[2] = '\0';
	m[0].content[0] = 'c';
	m[0].content[1] = 'd';
	m[0].content[2] = '\0';
	m[1].t.h = 2;	
	m[1].t.m = 3;	
	m[1].t.s = 4;
	m[1].sender[0] = 'e';
	m[1].sender[1] = 'f';
	m[1].sender[2] = '\0';
	m[1].content[0] = 'g';
	m[1].content[1] = 'h';
	m[1].content[2] = '\0';
	print_messages(2, m);
	EAPP_RETURN(0);
}

