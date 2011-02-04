#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define MIN_VALUE 1000
#define MAX_VALUE 9999

#define TRIANGLE_TYPE   0
#define SQUARE_TYPE     1
#define PENTAGONAL_TYPE 2
#define HEXAGONAL_TYPE  3
#define HEPTAGONAL_TYPE 4
#define OCTAGONAL_TYPE  5

#define TRIANGLE(n) (n*(n+1)/2)
#define SQUARE(n) (n*n)
#define PENTAGONAL(n) (n*(3*n-1)/2)
#define HEXAGONAL(n) (n*(2*n-1))
#define HEPTAGONAL(n) (n*(5*n-3)/2)
#define OCTAGONAL(n) (n*(3*n-2))

struct solution_s {
	int type;
	int n;
	int value;
	int visited;
};

struct set_s {
	struct solution_s solutions[100];
} sets[100];

int eval(int type, int n) {
	switch (type) {
		case TRIANGLE_TYPE: return TRIANGLE(n);
		case SQUARE_TYPE: return SQUARE(n);
		case PENTAGONAL_TYPE: return PENTAGONAL(n);
		case HEXAGONAL_TYPE: return HEXAGONAL(n);
		case HEPTAGONAL_TYPE: return HEPTAGONAL(n);
		case OCTAGONAL_TYPE: return OCTAGONAL(n);
	}
	return -1;
}

void fill() {
	memset(sets, 0, sizeof sets);
	for (int type = TRIANGLE_TYPE; type <= OCTAGONAL_TYPE; type++) {
		int n = 1;
		int value;
		while ((value = eval(type, n)) <= MAX_VALUE) {
			if (value >= MIN_VALUE) {
				int prefix = value / 100;
				int suffix = value % 100;
				sets[prefix].solutions[suffix].type = type;
				sets[prefix].solutions[suffix].n = n;
				sets[prefix].solutions[suffix].value = value;
			}
			n++;
		}
	}
}

int walk(char *path, int starting_value, int prefix, int suffix, int types, int depth) {
		struct solution_s *s = &sets[prefix].solutions[suffix];
		int new_prefix = suffix;
		//printf("%*stypes=%x depth=%d value=%d type=%d\n", depth, "", types, depth, s->value, s->type);
		char new_path[100];
		sprintf(new_path, "%s -> %d(%d) ", path, s->value, s->type);
		if (starting_value == s->value && depth) {
			printf("%s\n", new_path);
			return 0;
		}
		if (depth > 10) {
			printf("\n%*sToo deep! %d", depth, "", depth);
			return -99999999;
		}
		for (int new_suffix = 10; new_suffix <= 99; new_suffix++) {
			struct solution_s *ns = &sets[new_prefix].solutions[new_suffix];
			if (ns->n > 0 && types & (1 << ns->type)) {
				int sum = s->value + walk(new_path, starting_value, new_prefix, new_suffix, types ^ (1 << ns->type), depth + 1);
				if (sum > 0) return sum;
			}
		}
		return -99999999;
}

void search() {
	for (int prefix = 10; prefix <= 99; prefix++)
	for (int suffix = 10; suffix <= 99; suffix++) {
		struct solution_s *s = &sets[prefix].solutions[suffix];
		if (s->n > 0) {
			printf("Exploring %d\n", s->value);
			int sum = walk("", s->value, prefix, suffix, 0x3f/* ^ (1 << s->type)*/, 0);
			if (sum > 0) {
				printf("--------------- Found sum = %d\n", sum);
			}
		}
	}
}

int main() {
	fill();
	search();
	return 0;
}