#include <stdio.h>
#include "pass1.h"
#include "pass2.h"
#include "opcode.h"

#define FILENAME_INTERMEDIATE	"intermediate.txt"
#define FILENAME_SYMBOL			"symbol.txt"
#define FILENAME_LISTING		"listing.txt"

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s [source filename] [output filename]\n", argv[0]);
		return 1;
	}
	init_optbl();
	pass1(argv[1], FILENAME_INTERMEDIATE, FILENAME_SYMBOL);
	pass2(FILENAME_INTERMEDIATE, FILENAME_SYMBOL, FILENAME_LISTING, argv[2]);
	return 0;
}
