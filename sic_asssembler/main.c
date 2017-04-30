#include <stdio.h>
#include "pass1.h"
#include "pass2.h"
#include "opcode.h"
#include "symbol.h"

#define FILENAME_INTERMEDIATE	"intermediate.txt"
#define FILENAME_SYMBOL			"symbol.txt"
#define FILENAME_LISTING		"listing.txt"

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s [source filename] [output filename]\n", argv[0]);
		return 1;
	}
	// Initialize operator table for pass1 and pass2
	init_optbl();

	puts("======================================");
	puts("                PASS 1                ");
	puts("======================================");
	pass1(argv[1], FILENAME_INTERMEDIATE, FILENAME_SYMBOL);

	puts("======================================");
	puts("                PASS 1                ");
	puts("======================================");
	pass2(FILENAME_INTERMEDIATE, FILENAME_SYMBOL, FILENAME_LISTING, argv[2]);

	puts("======================================");
	puts("                 DONE                 ");
	puts("======================================");
	return 0;
}
