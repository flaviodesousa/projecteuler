#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define MAXLINE 1024

#define NORTH 0
#define SOUTH 1
#define WEST 2
#define EAST 3

#define ORDER 80
#define LASTINDEX (ORDER - 1)

/* empirically gotten */
#define BIGVALUE 0x7fffffff

const int DX[] = {  0,  0, -1,  1 };
const int DY[] = { -1,  1,  0,  0 };

struct matrix_node_s {
	unsigned int value;
	long best_sum;
	struct matrix_node_s *seeds[4];
	char crumb;
};

struct matrix_node_s matrix[ORDER][ORDER];

void parse_line(char *line, int row) {
	char *cp;
	int value = 0;
	int got_some_digits = 0;
	int column = 0;
	strcat(line,",");
	for(cp = line;*cp;cp++) {
		if (*cp >= '0' && *cp <= '9') {
			value = 10 * value + *cp - '0';
			got_some_digits = 1;
		} else {
			if (got_some_digits) {
				matrix[row][column].value = value;
				matrix[row][column].best_sum = BIGVALUE;
				matrix[row][column].seeds[NORTH] = row                ? &matrix[row - 1][column    ] : NULL;
				matrix[row][column].seeds[SOUTH] = row    < LASTINDEX ? &matrix[row + 1][column    ] : NULL;
				matrix[row][column].seeds[WEST ] = column             ? &matrix[row    ][column - 1] : NULL;
				matrix[row][column].seeds[EAST ] = column < LASTINDEX ? &matrix[row    ][column + 1] : NULL;
				matrix[row][column].crumb = '.';
				got_some_digits = value = 0;
				column++;
			}
		}
	}
}

void slurp() {
	FILE *fp;
	char line_read[MAXLINE + 2];
	int row = -1;
	fp = fopen("matrix.txt","r");
	if (!fp) {
		perror(NULL);
		exit(1);
	}
	while (!feof(fp)) {
		if (fgets(line_read, MAXLINE, fp) == NULL) {
			perror(NULL);
			exit(1);
		}
		parse_line(line_read, ++row);
	}
	fclose(fp);
}

int update(int r, int c) {
	long min_sum = BIGVALUE;
	for (int direction = NORTH; direction <= EAST; direction++) {
		if (
			matrix[r][c].seeds[direction] && 
			min_sum > matrix[r][c].seeds[direction]->best_sum
			) 
				min_sum = matrix[r][c].seeds[direction]->best_sum;
	}
	assert(min_sum != BIGVALUE);
	min_sum += matrix[r][c].value;
	if (min_sum != matrix[r][c].best_sum) {
		matrix[r][c].best_sum = min_sum;
		return 1;
	}
	return 0;
}

int totalize(int contour) {
	int updates = 0;
	for (int i = LASTINDEX; i > contour; i--) {
		updates |= update(i, contour);
		updates |= update(contour, i);
	}
	return updates | update(contour, contour);
}

void blow_path() {
	matrix[LASTINDEX][LASTINDEX].best_sum = matrix[LASTINDEX][LASTINDEX].value;
	matrix[LASTINDEX][LASTINDEX].crumb = '#';
	int had_update = 0;
	int iteration = 0;
	do {
		++iteration;
		printf("%d iteration%s...\n", iteration, iteration > 1 ? "s" : "");
		had_update = 0;
		for (int i = LASTINDEX - 1; i >= 0; i--) {
			had_update |= totalize(i);
		}
	} while ( had_update );
}

void drop_crumbs(struct matrix_node_s *node) {
	node->crumb = '#';
	long seed = node->best_sum - node->value;
	for (int i = 0; i < 4; i++) {
		if (node->seeds[i] && node->seeds[i]->best_sum == seed)
			drop_crumbs(node->seeds[i]);
	}
}

void draw_path() {
	for (int r = 0; r < LASTINDEX; r++)
	{
		for (int c = 0; c < LASTINDEX; c++)
			putchar(matrix[r][c].crumb);
		putchar('\n');
	}
}

int main() {
	slurp();
	blow_path();
	drop_crumbs(&matrix[0][0]);
	draw_path();
	printf("Best sum = %ld\n", matrix[0][0].best_sum);
	return 0;
}