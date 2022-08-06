/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"

#define PC_CONNECT_BUFFER_SIZE 64
static int g_connect_file_size;
static char g_connect_buffer[PC_CONNECT_BUFFER_SIZE];

/*
	Begin emulation of file functions
*/
#define FR_OK 0
#define FIL int
#define FA_WRITE 0
#define FA_CREATE_ALWAYS 0

char g_file[65536];
int g_file_point;
int f_open(void* fh, char* fname, int mode){
	sleep_ms(10);
	g_file_point=0;
	sleep_ms(10);
	return FR_OK;
}
int f_write(void* fh, char* buff, int size, void* dummy){
	int i;
	for(i=0;i<size;i++) {
		g_file[g_file_point++]=buff[i];
	}
	sleep_ms(10);
	return FR_OK;
}
int f_close(void* fh){
	g_file[g_file_point++]=0;
	sleep_ms(10);
	return FR_OK;
}
int f_chdir(char* dirname) { return FR_OK; }
int f_mkdir(char* dirname) { return FR_OK; }
/*
	End emulation of file functions
*/

void communication_error(void){
	printf("\nAn error occured in MachiKania during the commnucation\n");
	printf("Restart the application and reset MachiKania\n");
	while(1) sleep_ms(1000);
}

char* receive_command(void){
	// Note that USB-serial connection doesn't cause error (1 M bytes communication tested)
	static char command[17]="command command ";
	int i;
	unsigned int c;
	for(i=0;i<16;i++) {
		//while((c=getchar_timeout_us(1000))==PICO_ERROR_TIMEOUT);
		c=getchar_timeout_us(1000000);
		if (PICO_ERROR_TIMEOUT==c) {
			printf("\nCommand request timeout");
			communication_error();
		}
		command[i]=c;
	}
	return command;
}

void pc_connected(void){
	char* command;
	unsigned int c;
	int i;
	FIL fh;
	printf("SENDCMDS\x08\x08\x08\x08\x08\x08\x08\x08");
	while(1){
		command=receive_command();
		if (!strncmp(command,"DONEDONE\x08\x08\x08\x08\x08\x08\x08\x08",16)) {
			// All done
			f_chdir("/");
			printf("ALL DONE\x08\x08\x08\x08\x08\x08\x08\x08");
			return;
		} else if (!strncmp(command,"CD:",3)) {
			// Change directory
			printf("chdir: %s\n",command+3);
			while(1){
				if (FR_OK==f_chdir(command+3)) break;
				if (FR_OK==f_mkdir(command+3)) break;
				if (FR_OK==f_chdir(command+3)) break;
				printf("\nchdir failed");
				communication_error();
			}
			printf("OK\x08\x08OK\x08\x08OK\x08\x08OK\x08\x08");
		} else if (!strncmp(command,"SIZE:",5)) {
			g_connect_file_size=strtol(command+5,NULL,10);
			printf("size: %d, ",g_connect_file_size);
			printf("OK\x08\x08OK\x08\x08OK\x08\x08OK\x08\x08");
		} else if (!strncmp(command,"CP:",3)) {
			printf("copy: %s\n",command+3);
			if (FR_OK!=f_open(&fh,command+3,FA_WRITE|FA_CREATE_ALWAYS)) {
				printf("\nFile cannot be open");
				communication_error();
			}
			printf("SENDFILE\x08\x08\x08\x08\x08\x08\x08\x08");
			for(i=0;i<g_connect_file_size;i++) {
				//while((c=getchar_timeout_us(100000))==PICO_ERROR_TIMEOUT);
				c=getchar_timeout_us(1000000);
				if (PICO_ERROR_TIMEOUT==c) {
					printf("\nCommand request timeout");
					communication_error();
				}
				g_connect_buffer[(PC_CONNECT_BUFFER_SIZE-1)&i]=c;
				if ((PC_CONNECT_BUFFER_SIZE-1)==((PC_CONNECT_BUFFER_SIZE-1)&i)) {
					printf("OK\x08\x08OK\x08\x08OK\x08\x08OK\x08\x08");
					if (FR_OK!=f_write(&fh,g_connect_buffer,PC_CONNECT_BUFFER_SIZE,NULL)) {
						printf("\fCannot write to file");
						communication_error();
					}
				} else if (g_connect_file_size-1==i) {
					if (FR_OK!=f_write(&fh,g_connect_buffer,(PC_CONNECT_BUFFER_SIZE-1)&i,NULL)) {
						printf("\fCannot write to file");
						communication_error();
					}				
				}
			}
			f_close(&fh);
			printf("DONEDONE\x08\x08\x08\x08\x08\x08\x08\x08");
		} else {
			printf("\n%s ???\n",command);
			communication_error();
		}
	}
}

void connect2pc(void){
	int i;
	sleep_ms(900);
	printf("MACHIKAP\x08\x08\x08\x08\x08\x08\x08\x08");
	sleep_ms(100);
	for(i=0;i<16;i++){
		if ("OK\x08\x08OK\x08\x08OK\x08\x08OK\x08\x08"[i]!=getchar_timeout_us(1000)) break;
		if (15==i) pc_connected();
	}
	return;	
}

int main() {
	int i;
	stdio_init_all();
	connect2pc();
	printf("\n");
	while (true) {
		//printf("Hello, world!\n");
		for(i=0;i<4;i++){
			printf("%c\n","/-\\|"[i]);
			putchar(0x1b); putchar(0x5b); putchar(0x41);
			sleep_ms(1000);
		}
	}
	return 0;
}
/*
	Connection sequence (P: PC; M: MachiKania):
	
	M: "MACHIKAP\x08\x08\x08\x08\x08\x08\x08\x08"
	P: "OK\x08\x08OK\x08\x08OK\x08\x08OK\x08\x08"
	M: "SENDCMDS\x08\x08\x08\x08\x08\x08\x08\x08"
	
	P: "CD:DIRNAME\x00\x08\x08\x08\x08\x08"
	M: "OK\x08\x08OK\x08\x08OK\x08\x08OK\x08\x08" or
	   "NG\x08\x08NG\x08\x08NG\x08\x08NG\x08\x08"
	
	P: "CP:FILENAME.BAS\x00"
	M: "OK\x08\x08OK\x08\x08OK\x08\x08OK\x08\x08"
	P: "SIZE:1048576\x08\x08\x08"
	M: "SENDFILE\x08\x08\x08\x08\x08\x08\x08\x08"
	P: Send 512 bytes
	M: "OK\x08\x08OK\x08\x08OK\x08\x08OK\x08\x08"
	P: Send 512 bytes
	M: "OK\x08\x08OK\x08\x08OK\x08\x08OK\x08\x08"
	...
	P: Send 512 bytes
	M: "OK\x08\x08OK\x08\x08OK\x08\x08OK\x08\x08"
	P: Send last bytes
	M: "DONEDONE\x08\x08\x08\x08\x08\x08\x08\x08"
	
	P: "DONEDONE\x08\x08\x08\x08\x08\x08\x08\x08"
	M: "OK\x08\x08OK\x08\x08OK\x08\x08OK\x08\x08"
*/
/*
	File buffers
		The receive buffer contains 512 bytes.
		UART byte data are stored in the buffer first.
		When 512 bytes received, reply "OK".
		Then write the buffer to file.
*/