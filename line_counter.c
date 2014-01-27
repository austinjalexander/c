// from K&R

#include <stdio.h>

main()
{
	int c, nl;

	nl = 0;
	while ((c = getchar()) != EOF)
		if (c == '\n')
			++nl;
	printf("%d\n", nl);
}

// 'A' and '\n' are simply "character constants" with machine and ASCII numerical values;
// thus, they're simply another way to write integers

// "\n" is a string constant that happens to contain only one character