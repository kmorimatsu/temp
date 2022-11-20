/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "machikania.h"
#include "functions.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include "regexp.h"

char buf[BUFSIZ];

int errreport = 0;		/* Report errors via errseen? */
char *errseen = NULL;		/* Error message. */
int status = 0;			/* Exit status. */

#ifdef MACHIKANIA
void error(char *s1, char *s2){
	printstr("regexp: ");
	printstr(s1);
	printstr(s2);
	printstr("\n");
	exit(1);
}

void dummy_printf(char* str, ...){}
#define printf dummy_printf

#else
void error(char *s1, char *s2){
	printf("regexp: ");
	printf(s1);
	printf(s2);
	printf("\n");
	exit(1);
}
#endif

void regerror(char *s){
	if (errreport)
		errseen = s;
	else
		error(s, "");
}

char* test(int pos, char c);
int main() {
	regexp *r;
	int i;
	int argc=4;
	char* argv[]={
		"programname",
		"(sim)([a-z]le)",
		"a very simple simple simple string",
		"tESt",
	};

	machikania_init();
	free(case_insensitive("tes\\[t[a-f\\]G-H][abc0-9]"));

	stdio_init_all();
	sleep_ms(3000);

	r = regcomp(argv[1]);
	if (r == NULL)
		error("regcomp failure", "");
	if (argc > 2) {
		i = regexec(r, argv[2]);
		printf("%d", i);
		for (i = 1; i < NSUBEXP; i++)
			if (r->startp[i] != NULL && r->endp[i] != NULL)
				printf(" \\%d", i);
		printf("\n");
	}
	if (argc > 3) {
		regsub(r, argv[3], buf);
		printf("%s\n", buf);
	}

	for(i=0;true;i++) {
		printf("%c\x08","/|\\-"[i&3]);
		sleep_ms(1000);
	}
	return 0;
}

