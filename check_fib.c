#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <flint/flint.h>
#include <flint/fmpz.h>
#include <omp.h>

#define MAX_BASE 1000

long fib_remainder_base(long, long);

/* Get a number and index from the user,
 * and check if the remainders match up */
int main(int argc, char** argv) {
	omp_set_num_threads(24);
	if (argc != 3) {
		printf("Usage: ./check [gmp file] [index]\n");
		return -1;
	}

	unsigned long long index = strtoll(argv[2], NULL, 10), lremainder;
	if (index == 0 && *argv[2] != '0') {
		printf("Failed to parse \"%s\" as integer", argv[2]);
		return -1;
	}

	FILE *fp = fopen(argv[1], "r");
	if (!fp) {
		printf("Failed to open file \"%s\".", argv[1]);
		return -1;
	}

	fmpz_t num, remainder;
	fmpz_inp_raw(num, fp);
	fclose(fp);
	
#pragma omp parallel for
	for (int base = 2; base <= MAX_BASE; ++base) {
		fmpz_t remainder;
		fmpz_init(remainder);  

		unsigned long long lremainder = fib_remainder_base(index, base);

		fmpz_mod_ui(remainder, num, base);

#pragma omp critical
		{
			gmp_printf("base %d: %llu, %Zd\n", base, lremainder, remainder);
			if (fmpz_cmp_ui(remainder, lremainder)) {
				printf("Failed check\n");
			}
		}

		fmpz_clear(remainder);
	}

	printf("Passed all bases up to %d.\n", MAX_BASE);
}
