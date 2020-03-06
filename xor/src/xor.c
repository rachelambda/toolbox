#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

const char helpmsg[] =\
	  "Usage: \n"\
	  "\n"\
	  "xor <mode and flags> [string] <hex byte>\n"\
	  "\n"\
	  "Modes: \n"\
	  "\n"\
	  "S - read from standard input\n"\
	  "F - read from file, filename is given as second argument\n"\
	  "A - read from second arg\n"\
	  "\n"\
	  "Additional flags:\n"\
	  "\n"\
	  "O - output result to xor.res file\n"\
	  "B - brute force all bytes, starting from given byte";

typedef enum InputMode {
	STDIN = 0,
	INFILE = 1,
	ARG = 2
} InputMode;

const char inflags[3] = { 'S', 'F', 'A' };

unsigned char fout = 0;
unsigned char brute = 0;

int
main(int argc, char **argv)
{
	if (argc < 3) {
		puts(helpmsg);
		exit(1);
	}

	InputMode inmode;
	unsigned char valid = 0;

	for (int i = strlen(argv[1]); i > 0;) {
		i--;

		if (argv[1][i] == 'O') {
			puts("Outputting results to xor.res");
			fout = 1;
		}

		if (argv[1][i] == 'B') {
			puts("=== Brute Force ===");
			brute = 1;
		}

		for ( int n = 0; n < 3; n++ ) {
			if (argv[1][i] == inflags[n]) {
				inmode = n;
				valid += 1;
			}
		}
	}

	if (valid != 1) {
		puts(helpmsg);
		exit(1);
	}

	char* inbuffer = NULL;
	int inbuffersize = 0;
	char* xorbyteptr = argv[2];

	switch (inmode) {
	case STDIN:

		inbuffer = malloc(512);
		int inbufferfill = 0;
		inbuffersize = 512;
		char c = 0x0;
		for (;;) {
			c = getchar();
			if (c == EOF)
				break;
			if (inbufferfill == inbuffersize) {
				inbuffersize += 512;
				inbuffer = realloc(inbuffer, inbuffersize);
			}
			
			inbuffer[inbufferfill] = c;

			inbufferfill++;
		}

		inbuffersize = inbufferfill;
		inbuffer = realloc(inbuffer, inbuffersize);
		
		break;
	case INFILE:

		if (argc < 4) {
			puts(helpmsg);
			exit(1);
		}

		xorbyteptr = argv[3];

		FILE* fptr = fopen((argv[2]), "rb");

		if (!fptr) {
			puts("Input file not found.");
			exit(2);
		}

		fseek(fptr, 0, SEEK_END);
		inbuffersize = ftell(fptr);
		fseek(fptr, 0, SEEK_SET);
		inbuffer = malloc(inbuffersize);
		fread(inbuffer, 1, inbuffersize, fptr);
		fclose(fptr);

		break;
	case ARG:

		if (argc < 4) {
			puts(helpmsg);
			exit(1);
		}

		inbuffer = argv[2];
		xorbyteptr = argv[3];
		inbuffersize = strlen(inbuffer);
		break;
	default:
		puts(helpmsg);
		exit(1);
	}


	char xorbyte;
	sscanf(xorbyteptr, "%x", &xorbyte);

	if (!brute) {
		for (int n= 0; n < inbuffersize; n++) {
			inbuffer[n] ^= xorbyte;
		}
		if (fout) {
			int fptr = open("xor.res", O_CREAT | O_RDWR, 0666);
			if (fptr > 0) {
				write(fptr, inbuffer, inbuffersize);
			} else {
				puts("Error writing to log file.");
				exit(3);
			}
		} else {
			write(1, inbuffer, inbuffersize);
		}
	} else {
		for (int n= 0; n < inbuffersize; n++) {
			for (char b = xorbyte; b != (xorbyte -1); b++) {
				inbuffer[n] ^= b;
			}

			if (fout) {
				int fptr = open("xor.res", O_CREAT | O_RDWR | O_APPEND, 0666);
				if (fptr > 0) {
					write(fptr, inbuffer, inbuffersize);
				} else {
					puts("Error writing to log file.");
					exit(3);
				}
			} else {
				write(1, inbuffer, inbuffersize);
			}
		}
	}


	exit(0);
}
