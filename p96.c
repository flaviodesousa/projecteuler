#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int duplicado_row(char *matriz, int posicao, char c)
{
	int p = (posicao / 9)*9;
	char *cp = matriz + p;
	int cnt = 0;
	for (int i = 0; i < 9; i++)
		if (cp[i] == c)
			if (cnt++)
				return 1;
	return 0;
}

int duplicado_col(char *matriz, int posicao, char c)
{
	int p = posicao % 9;
	char *cp = matriz + p;
	int cnt = 0;
	for (int i = 0; i < 81; i += 9)
		if (cp[i] == c)
			if (cnt++)
				return 1;
	return 0;
}

int duplicado_box(char *matriz, int posicao, char c)
{
	int p = (posicao / 27) * 3 + (posicao % 9) / 3;
	p = (p / 3) * 27 + (p % 3) * 3;
	char *cp = matriz + p;
	int cnt = 0;
	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++)
			if (cp[row*9+col] == c)
				if (cnt++)
					return 1;
	return 0;
}

int duplicado(char *matriz, int posicao, char c)
{
	return 
		duplicado_row(matriz, posicao, c) ||
		duplicado_col(matriz, posicao, c) ||
		duplicado_box(matriz, posicao, c);
}

int livre(char *matriz, int inicial)
{
	while (inicial < 81)
		if (matriz[inicial] == '0')
			return inicial;
		else
			inicial++;
	return -1;
}

int resolver(char *matriz, int inicial)
{
	int posicao = livre(matriz, inicial);
	if (posicao == -1) /* nao há mais buracos, resolvido! */
		return 1;
	for (char c='1'; c <= '9'; c++)
	{
		matriz[posicao] = c;
		if ( ! duplicado(matriz, posicao, c) )
			if (resolver(matriz, posicao + 1))
				return 1;
	}
	matriz[posicao] = '0';
	return 0;
}

int main()
{
	FILE *fp;
	fp = fopen("sudoku.txt","rt");
	char *linha; linha = malloc(128);
	char *matriz; matriz = malloc(128);
	int sum = 0;
	int solves = 0;
	int contador_linha = -1;
	while ( ! feof(fp) )
	{
		fgets(linha, 128, fp); contador_linha++;
		if (contador_linha % 10 == 0) /* omitir linhas 'Grid' */
		{
			*matriz = '\0';
		}
		else
		{
			strncat(matriz, linha, 9);
			if (contador_linha % 10 == 9)
			{
				if (resolver(matriz, 0))
				{
					char inicio[4] = "";
					strncat(inicio, matriz, 3);
					sum += atoi(inicio);
					printf("resolvido: %d %d %s %s\n", ++solves, sum, inicio, matriz);
				}
				else
				{
					printf("sem solucao! %s\n", matriz);
				}
			}
		}
	}
	fclose(fp);
	return 0;
}