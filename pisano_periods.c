#include <stdio.h>
#include <stdlib.h>

long pisano_period(long);

/* Print the pisano period of the first 1000 numbers */
int main(int argc, char** argv) {
	int max_base = 1000;
	if (argc == 2) {
		max_base = atoi(argv[1]);
	}
	struct TreeNode *root = NULL;
	for (int base = 2; base < max_base; base++) {
		int period = pisano_period(base);
		printf("base: %d, %ld\n", base, pisano_period(base));
	}
}
