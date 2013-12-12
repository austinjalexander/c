#include <stdio.h>

int main()
{

	int i,j;
	char product[7];

	for (i = 999; i > 900; i--)
	{
		for (j = 999; j > 900; j--)
		{
			sprintf(product, "%d", (i * j));
			
			if (product[0] == product[5])
			{
				if (product[1] == product[4])
				{
					if (product[2] == product[3])
					{
						printf("%s\n", product);
					}
				}
			}
			

		}
	}

	return 0;
}