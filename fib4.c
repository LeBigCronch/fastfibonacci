#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <gmp.h>
#include <time.h>

int main(int argc, char *argv[])
{
	// Get User Input
	if (argc != 2) {
		printf("usage: %s NUM\n", argv[0]);
		return EXIT_FAILURE;
	}

	clock_t start_time = clock();
	register unsigned long count = strtol(argv[1], NULL, 10);

	mpz_t a, b, c, d; // Current term, previous term, matrix terms
	mpz_init_set_ui(a, 1);
	mpz_init_set_ui(b, 0);
	mpz_init_set_ui(c, 1);
	mpz_init_set_ui(d, 0);

	mpz_t c2, c3, c4, c5, d2, d3, d4, d5; // Intermediate terms for 5x boosts
	mpz_init(c2);
	mpz_init(c3);
	mpz_init(c4);
	mpz_init(c5);
	mpz_init(d2);
	mpz_init(d3);
	mpz_init(d4);
	mpz_init(d5);

	while (count) {
		if (count % 5 == 0) { // 5x boost
			mpz_mul(c2, c, c);         // c^2
			mpz_mul(c3, c2, c);        // c^3
			mpz_mul(c4, c2, c2);       // c^4
			mpz_mul(c5, c4, c);        // c^5
			mpz_mul(d2, d, d);         // d^2
			mpz_mul(d3, d2, d);        // d^3
			mpz_mul(d4, d3, d);        // d^4
			mpz_mul(d5, d4, d);        // d^5
			mpz_mul(c4, c4, d);		   // cd^4
			mpz_mul(c3, c3, d2);	   // c^2d^3
			mpz_mul(c2, c2, d3);       // c^3d^2
			mpz_mul(d4, c, d4);		   // c^4d

			mpz_mul_ui(c, c5, 5);
			mpz_addmul_ui(c, c4, 15);
			mpz_addmul_ui(c, c3, 20);
			mpz_addmul_ui(c, c2, 10);
			mpz_addmul_ui(c, d4, 5);

			mpz_mul_ui(d, c5, 3);
			mpz_addmul_ui(d, c4, 10);
			mpz_addmul_ui(d, c3, 10);
			mpz_addmul_ui(d, c2, 10);
			mpz_add(d, d, d5);
			count /= 5;
		} else if (count % 2 == 0) { // 2x boost
			mpz_mul(c2, c, c);
			mpz_mul(c, c, d);
			mpz_mul_2exp(c, c, 1);
			mpz_mul(d, d, d);
			mpz_add(c, c, c2);
			mpz_add(d, d, c2);
			count /= 2;
		} else { // Increment
			mpz_mul(c2, a, c);
			mpz_mul(a, a, d);
			mpz_addmul(a, b, c);
			mpz_add(a, a, c2);
			mpz_mul(b, b, d);
			mpz_add(b, b, c2);
			count--;
		}

	}

	clock_t end_time = clock();
	double taken = ((double) end_time - start_time) / CLOCKS_PER_SEC;
	/* gmp_printf("%Zd\n", b); */
	printf("\ncalculation time: %f seconds\n", taken);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(c);
	mpz_clear(d);
	mpz_clear(c2);
	mpz_clear(c3);
	mpz_clear(c4);
	mpz_clear(c5);
	mpz_clear(d2);
	mpz_clear(d3);
	mpz_clear(d4);
	mpz_clear(d5);
	return 0;
}
