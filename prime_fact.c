#include <stdio.h>

main()
{
//long long num = 600851475143;
//long long num = 8462696833;
//long long num = 10086647;
//long num = 6857;
//long num = 1471;
//int num = 839;
int num = 71;

//int first_100_primes[100] = { 0 };
int i;

for (i = 1; i < num; i++)
{
	if ((num % i) == 0)
		printf("%d\n", i);
}



/*
//long half_of_num = (num / 2);
//long i;

//int factor;
//long j;
//int prime_truth;

// count down backward from num
for (i = (half_of_num); i > 1; i--)
{
// if i is a factor
if ((num % i) == 0)
{
i = 1;
}
}
*/
/*
// loop as many times as num
for (i = 1; i <= num; i++)
{
// if the current i is a factor
if (num % i == 0)
{
// check if i is a prime factor
prime_truth = 1; 
for (j = 2; j < i; j++)
{
// if any number j divides i, quit loop
if ((i % j) == 0)
{
j = i;
prime_truth = 0;
printf("%d\n", i);
}
}
// if the number is prime
if (prime_truth == 1)
{
//printf("%d\n", i);
}
}
} 
*/

}