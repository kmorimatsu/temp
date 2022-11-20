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