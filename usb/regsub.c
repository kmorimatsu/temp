/*
 * regsub
 */
// Inserted 1 line for MachiKania
#include "machikania.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "regexp.h"
#include "regmagic.h"

/*
 - regsub - perform substitutions after a regexp match
 */
void
regsub(rp, source, dest)
const regexp *rp;
const char *source;
char *dest;
{
	register regexp * const prog = (regexp *)rp;
	register char *src = (char *)source;
	register char *dst = dest;
	register char c;
	register int no;
	register size_t len;

	if (prog == NULL || source == NULL || dest == NULL) {
		regerror("NULL parameter to regsub");
		return;
	}
	if ((unsigned char)*(prog->program) != MAGIC) {
		regerror("damaged regexp");
		return;
	}

	while ((c = *src++) != '\0') {
		if (c == '&')
			no = 0;
		// Modified a line for MachiKania
		// else if (c == '\\' && isdigit(*src))
		else if (c == '$' && isdigit(*src))
			no = *src++ - '0';
		else
			no = -1;

		if (no < 0) {	/* Ordinary character. */
			// Modified a line for MachiKania
			// if (c == '\\' && (*src == '\\' || *src == '&'))
			if (c == '\\' && (*src == '\\' || *src == '&' || *src == '$'))
				c = *src++;
			*dst++ = c;
		} else if (prog->startp[no] != NULL && prog->endp[no] != NULL &&
					prog->endp[no] > prog->startp[no]) {
			len = prog->endp[no] - prog->startp[no];
			(void) strncpy(dst, prog->startp[no], len);
			dst += len;
			if (*(dst-1) == '\0') {	/* strncpy hit NUL. */
				regerror("damaged match string");
				return;
			}
		}
	}
	*dst++ = '\0';
}
