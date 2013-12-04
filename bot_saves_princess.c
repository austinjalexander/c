#include <stdio.h>
#include <string.h>
#include <math.h>

void displayPathtoPrincess(int n, char grid[n][n])
{
	
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