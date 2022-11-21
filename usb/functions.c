#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "machikania.h"
#include "regexp.h"
#include "pico/stdlib.h"

char* case_insensitive(char* str){
	int len=strlen(str);
	int max=len*4+5;
	char* ret=calloc(max,1);
	char* res=ret;
	char c,c2;
	if (!ret) regerror("malloc failed");
	int i=0;
	while(res<ret+max && (c=(str++)[0])){
		switch(c){
			case '\\':
				(res++)[0]=c;
				(res++)[0]=(str++)[0];
				break;
			case '[':
				// For example:
				//	[abc] => [aAbBcC]
				//	[a-c] => [a-cA-C]
				//	[A-C] => [A-Ca-c]
				(res++)[0]=c;
				while(res<ret+max && (c=(str++)[0])){
					if ('A'<=c && c<='Z') {
						(res++)[0]=c;
						if ('-'==str[0]) {
							str++;
							(res++)[0]='-';
							c2=(str++)[0];
							(res++)[0]=c2;
							(res++)[0]=c+0x20;
							(res++)[0]='-';
							(res++)[0]=c2+0x20;
						} else {
							(res++)[0]=c+0x20;
						}
					} else if ('a'<=c && c<='z') {
						(res++)[0]=c;
						if ('-'==str[0]) {
							str++;
							(res++)[0]='-';
							c2=(str++)[0];
							(res++)[0]=c2;
							(res++)[0]=c-0x20;
							(res++)[0]='-';
							(res++)[0]=c2-0x20;
						} else {
							(res++)[0]=c-0x20;
						}
					} else if ('-'==c || '\\'==c) {
						(res++)[0]=c;
						(res++)[0]=(str++)[0];
					} else if (']'==c) {
						(res++)[0]=c;
						break;
					} else {
						(res++)[0]=c;
					}
				
				}
				break;
			default:
				if ('A'<=c && c<='Z') {
					(res++)[0]='[';
					(res++)[0]=c+0x20;
					(res++)[0]=c;
					(res++)[0]=']';
				} else if ('a'<=c && c<='z') {
					(res++)[0]='[';
					(res++)[0]=c;
					(res++)[0]=c-0x20;
					(res++)[0]=']';
				} else {
					(res++)[0]=c;
				}
				break;
		}
	}
	return ret;
}

char* support_curly_sub(char* str,char endc, char** res);
char* support_curly(char* str){
	int len=strlen(str);
	volatile int max=1; // Don't know why "volatile" is needed in "if (max<a) max=a;"
	int i,a;
	char c=0;
	// Get the max number
	for(i=0;i<len;i++){
		if ('\\'==str[i]) {
			i++;
			continue;
		}
		if ('{'!=str[i]) continue;
		a=atoi(str+i+1);
		if (max<a) max=a;
	}
	// Allocate memory
	char* ret=calloc(len*max+1,1);
	char* res=ret;
	if (!ret) regerror("malloc error");
	// Compile
	support_curly_sub(str,0,&res);
	return ret;
}

char* support_curly_sub(char* str,char endc, char** pres){
	char* res=(char*)(*pres);
	char* begin=str;
	char* end=str;
	char c;
	int min,max,i;
	while(c=(res++)[0]=(str++)[0]){
		if (c==endc) break;
		if ('{'!=c) begin=str-1;
		switch(c){
			case '\\':
				(res++)[0]=(str++)[0];
				continue;
			case '(':
				str=support_curly_sub(str,')',&res);
				continue;
			case '[':
				str=support_curly_sub(str,']',&res);
				continue;
			case '{':
				res--;
				end=str-1;
				min=atoi(str);
				while('}'!=(str++)[0]);
				while(0<(--min)){
					for(i=0;begin+i<end;i++) (res++)[0]=begin[i];
				}
				continue;
			default:
				continue;
		}
	}
	*pres=res;
	return str;
}
