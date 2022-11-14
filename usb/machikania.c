#include "machikania.h"

int* g_r5_8[4];

void machikania_init(void){
	asm("push {r0,r1}");
	asm("ldr r1,=g_r5_8");
	asm("str r5,[r1,#0]");
	asm("str r6,[r1,#4]");
	asm("str r7,[r1,#8]");
	asm("movs r0,r8");
	asm("str r0,[r1,#12]");
	asm("pop {r0,r1}");
}

int machikania_lib(int r0, int r1, int r2, int r3){
	int (*f)(int r0, int r1, int r2, int r3) = (void*)g_r8;
	return f(r0,r1,r2,r3);
}

void* machikania_malloc(int bytes){
	return (void*) machikania_lib(251,bytes,0,LIB_SYSTEM);
}

void* machikania_calloc(int bytes){
	return (void*) machikania_lib(250,bytes,0,LIB_SYSTEM);
}

void machikania_free(void* addr){
	machikania_lib(252,(int)addr,0,LIB_SYSTEM);
}

void machikania_exit(int num){
	printstr("\nexit(");
	printint(num);
	printstr(")\n");
	machikania_lib(0,0,0,LIB_END);
}

void printstr(char* str){
	machikania_lib((int)str,0x11,0,LIB_PRINT);
}

void printint(int val){
	machikania_lib(val,0x10,0,LIB_PRINT);
}

void _printhex(int val, int width){
	int str=machikania_lib(width,val,0,LIB_HEX);
	machikania_lib(str,0x11,0,LIB_PRINT);
	machikania_lib(253,str,0,LIB_SYSTEM);
}

void printhex(int val){
	_printhex(val,0);
}

void printhex8(int val){
	_printhex(val,2);
}

void printhex16(int val){
	_printhex(val,4);
}

void printhex32(int val){
	_printhex(val,8);
}
