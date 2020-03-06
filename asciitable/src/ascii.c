#include <stdio.h>
#include <stdlib.h>

const char helpmsg[] =\
	  "Usage: \n"\
	  "\n"\
	  "dascii <char>\n";

int
main(int argc, char **argv)
{
	if (argc < 2) {
		puts(helpmsg);
		exit(1);
	}

	printf("Char: '%c' Decimal: '%u' Hex: '0x%x'\n", argv[1][0], argv[1][0], argv[1][0]);
	exit(0);
}
