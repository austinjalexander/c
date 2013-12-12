#include <stdio.h>
#include <string.h>

void ABCheck(char str[])
{
	int i, str_length, found = 0;
	str_length = strlen(str);

	for (i = 0; i < str_length; i++)
	{
		if (str[i] == 'a')
		{
			if (str[i + 4] == 'b')
			{
				found = 1;
				i = str_length;
			}
		}
	}

	if (found == 0)
		printf("false\n");
	else
		printf("true\n");
}

int main(void)
{

	char str[] = "Laura sobs";
	//char str[] = "after badly";

	ABCheck(str);

	return 0;
}