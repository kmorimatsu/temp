/*
	KM-BASIC Tiny
	Copyright (c) 2022- Katsumi Morimatsu
	License: LGPL ver 2.1
*/

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

static char g_basic_program[65536];
static char g_line[512];

char* get_line(void){
	int pos=0;
	int c;
	while(1){
		c=getchar_timeout_us(1000);
		if (c<0) continue;
		switch(c){
			case 0x1b:
				// Escape sequence. Ignore two more characters
				getchar_timeout_us(1000);
				getchar_timeout_us(1000);
				break;
			case 0x08:
			case 0x7f:
				// Back space
				if (0<pos) {
					pos--;
					putchar(c);
				}
				break;
			case 0x0d:
				// Enter
				g_line[pos]=0;
				putchar(c);
				putchar(0x0a);
				return g_line;
			case 0x1a:
				// Ctrl-Z
				return 0;
			default:
				if (0x20<=c && c<0x7f || 0x09==c) {
					g_line[pos++]=c;
					if ((sizeof g_line)<=pos) pos=(sizeof g_line)-1;
					putchar(c);
				}
				break;
		}
	}
}

void command_type(void){
	char* line;
	int i;
	int pos=0;
	while(line=get_line()){
		// Check the size
		i=strlen(line);
		if ((sizeof g_basic_program)<= pos+i+3) {
			g_basic_program[0]=0;
			printf("\n  Program too large!\n");
			return;
		}
		// Copy the line to basic program
		memcpy(&g_basic_program[pos],line,i);
		pos+=i;
		g_basic_program[pos++]=0x0d;
		g_basic_program[pos++]=0x0a;
	}
	// Ctrl-Z pressed
	g_basic_program[pos]=0;
	printf("\n");
}

void command_list(void){
	puts(g_basic_program);
}

void command_run(void){
	printf("  COMMAND_RUN\n");
	// compile_program(g_basic_program);
	// run();
}

void command_error(void){
	printf("  Command Error\n");
	printf("  Valid commands: TYPE, LIST, and RUN\n");
}

void __attribute__ ((noinline)) command_assembler(void){
	asm ("nop");
}

int main() {
	char* command;
	stdio_init_all();
	// Wait for 3 seconds
	sleep_ms(3000);
	// Show logo
	printf("KM-BASIC tiny\n");
	// Main loop
	while(1){
		printf(">");
		command=get_line();
		if (!command) continue; // Ctrl-Z
		if (!strcmp(command,"")) printf("");
		else if (!strcmp(command,"TYPE") || !strcmp(command,"type")) command_type();
		else if (!strcmp(command,"LIST") || !strcmp(command,"list")) command_list();
		else if (!strcmp(command,"RUN") || !strcmp(command,"run")) command_run();
		else if (!strcmp(command,"\x08 dummy assembler")) command_assembler();
		else command_error();
	}
	return 0;
}
