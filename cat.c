/*
 * UNG's Not GNU
 *
 * Copyright (c) 2011-2019, Jakob Kaivo <jkk@ung.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define _XOPEN_SOURCE 700
#include <errno.h>
#include <nl_types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int cat(const char *path)
{
	FILE *f = stdin;

	if (path && strcmp("-", path)) {
		f = fopen(path, "r");
	}

	if (f == NULL) {
		static char *fmt = NULL;
		const char *def = "cat: %s: %s\n";
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
