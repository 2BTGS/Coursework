#include <stdio.h>
#include <stdlib.h>

#include "algorithm.h"

int main(int argc, char **argv)
{
	char *res;

	if(argc != 2) {
		fprintf(stderr, "usage: %s \"STRING\"\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	res = algorithm(argv[1]);

	printf("%s\n", res);

 	free(res);

	exit(EXIT_SUCCESS);
}
