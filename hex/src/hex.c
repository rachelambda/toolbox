#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

const char helpmsg[] =\
	  "Usage: \n"\
	  "\n"\
	  "hex <mode and flags> [string] <hex byte>\n"\
	  "\n"\
	  "Modes: \n"\
	  "\n"\
	  "S - read from standard input\n"\
	  "F - read from file, filename is given as second argument\n"\
	  "A - read from second arg\n"\
	  "\n"\
	  "Additional flags:\n"\
	  "\n"\
	  "O - output result to hex.res";

typedef enum InputMode {
	STDIN = 0,
	INFILE = 1,
	ARG = 2
} InputMode;

const char inflags[3] = { 'S', 'F', 'A' };

unsigned char fout = 0;

int
main(int argc, char **argv)
{
	if (argc < 2) {
		puts(helpmsg);
		exit(1);
	}

	InputMode inmode;
	unsigned char valid = 0;

	for (int i = strlen(argv[1]); i > 0;) {
		i--;

		if (argv[1][i] == 'O') {
			puts("Outputting results to hex.res");
			fout = 1;
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

	case INFILE:;

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
		inbuffer = argv[2];
		inbuffersize = strlen(inbuffer);
		break;
	default:
		puts(helpmsg);
		exit(1);
	}

	if (!fout)
		for (int n= 0; n < inbuffersize; n++) {
			printf("%02hhx", inbuffer[n]);
		}
	else {
		FILE* fp = fopen("hex.res", "wa");
		for (int n= 0; n < inbuffersize; n++) {
			fprintf(fp, "%02hhx", inbuffer[n]);
		}
		fclose(fp);
	}

	exit(0);
}
