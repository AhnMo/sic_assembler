#include <stdio.h>
#include "pass1.h"
#include "pass2.h"

#define FILENAME_INTERMEDIATE	"intermediate.txt"
#define FILENAME_LISTING		"listing.txt"

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s [source filename] [output filename]", argv[0]);
		exit(0);
	}
	pass1(argv[1], FILENAME_INTERMEDIATE);
	pass2(FILENAME_INTERMEDIATE, FILENAME_LISTING, argv[2]);
	return 0;
}
