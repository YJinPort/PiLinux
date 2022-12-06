#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	srand((unsigned int)time(NULL));
	printf("rand_num : %d\n", rand());
	printf("rand_num : %d\n", rand());
	printf("rand_num : %d\n", rand());
	printf("rand_num : %d\n", rand());
	printf("rand_num : %d\n", rand());

	return 0;
}
