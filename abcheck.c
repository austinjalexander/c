#include <stdio.h>

void ABCheck(char * str[])
{
	int i;



	for (i = 0; i < sizeof(*str); i++)
	{
		if (str[i] == 'a')
		{
			if (str[i + 4] == 'b')
			{
				i = sizeof(*str);
				rintf("%s", str);
			}
				
		}
	}

	p
}

int main(void)
{

	char input[] = "after badly";

	ABCheck(input);

	return 0;
}