#include <stdio.h>
#include <string.h>
#include <math.h>

void displayPathtoPrincess(int n, char grid[n][n])
{
	int m[2], p[2];
	int i,j;

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (grid[i][j] == 'm')
			{
				m[0] = i;
				m[1] = j;
			}
			if (grid[i][j] == 'p')
			{
				p[0] = i;
				p[1] = j;
			}
		}
	}

	if (p[0] > m[0])
	{
		for (i = 1; i <= (p[0] - m[0]); i++)
			printf("%s\n", "DOWN");
	}
	else if (m[1] > p[0])
	{
		for (i = 1; i <= (m[0] - p[0]); i++)
			printf("%s\n", "UP");		
	}

	if (p[1] > m[1])
	{
		for (i = 1; i <= (p[1] - m[1]); i++)
			printf("%s\n", "RIGHT");
	}
	else if (m[1] > p[1])
	{
		for (i = 1; i <= (m[1] - p[1]); i++)
			printf("%s\n", "LEFT");		
	}
}

int main(void)
{
	int i;
	int m;
	scanf("%d", &m);
	char grid[m][m];
	char line[m];

	for(i = 0; i < m; i++)
	{
		scanf("%s", line);
		strncpy(grid[i], line, m);
	}

	displayPathtoPrincess(m, grid);

	return 0;
}