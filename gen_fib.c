#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <flint/flint.h>
#include <flint/fmpz.h>
#include <time.h>

int main(int argc, char *argv[])
{
	// Get User Input
	if (argc != 2) {
		printf("usage: %s NUM\n", argv[0]);
		return EXIT_FAILURE;
	}

	clock_t start_time = clock();
	unsigned long long count = strtoll(argv[1], NULL, 10);

	long mcount = 0;

	flint_set_num_threads(24);

	fmpz_t a, b, c, d; // Current term, previous term, matrix terms
	fmpz_init_set_ui(a, 1);
	fmpz_init_set_ui(b, 0);
	fmpz_init_set_ui(c, 1);
	fmpz_init_set_ui(d, 0);

	fmpz_t c2;
	fmpz_init(c2);

	while (count) {
		printf("%lld\n", count);
		if (count % 2 == 0) { // 2x boost
			fmpz_mul(c2, c, c);
			fmpz_mul(c, c, d);
			fmpz_mul_2exp(c, c, 1);
			fmpz_mul(d, d, d);
			fmpz_add(c, c, c2);
			fmpz_add(d, d, c2);

			mcount += 3;
			count /= 2;
		} else { // Increment
			fmpz_mul(c2, a, c);
			fmpz_mul(a, a, d);
			fmpz_addmul(a, b, c);
			fmpz_add(a, a, c2);
			fmpz_mul(b, b, d);
			fmpz_add(b, b, c2);
			mcount += 4;
			count--;
		}
	}

	clock_t end_time = clock();
	double taken = ((double) end_time - start_time) / CLOCKS_PER_SEC;

	printf("\ncalculation time: %f seconds\n", taken);
	printf("%lu multiplications taken\n", mcount);

	FILE* fp = fopen("fib.fmpz", "w");
	fmpz_out_raw(fp, b);
	fclose(fp);

	printf("result saved to \'fib.fmpz\'\n");

	fmpz_clear(a);
	fmpz_clear(b);
	fmpz_clear(c);
	fmpz_clear(d);
	fmpz_clear(c2);

	return 0;
}
