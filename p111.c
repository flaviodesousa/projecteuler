#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

int M[10];
int N[10] = {0};
mpz_t S[10];
mpz_t TEN;
int k = 4;

void initialize() {
	for (int i = 0; i < 10; i++) {
		mpz_init(S[i]);
		M[i] = k / 2;
	}
	mpz_init_set_ui(TEN, 10);
}

void count() {
	mpz_t initial, final, p;
	char *num, *cp;
	int sample = 0;
	int digit_count[10];

	num = malloc(k+1);
	mpz_inits(p, initial, final, NULL);

	// initial = 10^(k-1) - 1
	mpz_init(initial); mpz_ui_pow_ui(initial, 10, k - 1); mpz_sub_ui(initial, initial, 1);
	// final = 10^k - 1
	mpz_init(final); mpz_ui_pow_ui(final, 10, k); mpz_sub_ui(final, final, 1);

	mpz_nextprime(p, initial);
	do {
		//gmp_printf("p=%Zd\n", p);
		memset(digit_count, 0, sizeof digit_count);
		cp = mpz_get_str(num, 10, p);
		int best_digit = -1;
		do {
			register int digit = *cp - '0';
			if (++digit_count[digit] >= M[digit])
				best_digit = digit;
			//gmp_printf("char %c digit %d count %d M[%d]=%d\n", *cp, digit, digit_count[digit], digit, M[digit]);
		} while (*++cp);
		if (best_digit >= 0) {
			register int dc = digit_count[best_digit];
			if (dc == M[best_digit]) {
				N[best_digit]++;
				mpz_add(S[best_digit], S[best_digit], p);
			} else if (dc > M[best_digit]) {
				M[best_digit] = dc;
				N[best_digit] = 1;
				mpz_set(S[best_digit], p);
			}
		}
		mpz_nextprime(p, p);
	} while (mpz_cmp(final, p) > 0);
}

void print() {
	mpz_t sum; mpz_init(sum);
	for (int i = 0; i < 10; i++)
	{
		mpz_add(sum, sum, S[i]);
		gmp_printf("%d M=%d N=%d S=%Zd SUM=%Zd\n", i, M[i], N[i], S[i], sum);
	}
	gmp_printf("Finished with %Zd\n", sum);
}

int main(int argc, char **argv) {
	if (argc > 1) k = atoi(argv[1]);
	initialize();
	count();
	print();
}
