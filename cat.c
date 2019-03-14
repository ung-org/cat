/*
 * UNG's Not GNU
 *
 * Copyright (c) 2011-2017, Jakob Kaivo <jkk@ung.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#define _XOPEN_SOURCE 700
#include <errno.h>
#include <nl_types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/** operands
INFILE [file...] The name of one or more files to print to stdout. DEF(stdin) DASH(stdin)
**/

static int cat(const char *path)
{
	FILE *f = stdin;

	if (path && strcmp("-", path)) {
		f = fopen(path, "r");
	}

	if (f == NULL) {
		static char *fmt = NULL;
		const char *def = "cat: Couldn't open %s: %s\n";
		if (fmt == NULL) {
			nl_catd catd = catopen("cat", NL_CAT_LOCALE);
			fmt = catgets(catd, NL_SETD, 1, def);
		}
		fprintf(stderr, fmt, path ? path : "stdin", strerror(errno));
		return 1;
	}

	int c;
	while ((c = fgetc(f)) != EOF) {
		fputc(c, stdout);
	}

	if (f != stdin) {
		fclose(f);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int ret = 0, c = 0;

	while ((c = getopt(argc, argv, "u")) != -1) {
		switch(c) {
		case 'u':	/** Do not buffer output **/
			setbuf(stdout, NULL);
			break;
		default:
			return 1;
		}
	}

	do {
		ret |= cat(argv[optind++]);
	} while (optind < argc);

	return ret;
}

/**cat en_US@color
1 \033[31mcat: Couldn't open \033[0;41;23m%s\033[0;31m: %s\033[0m\n
**/
