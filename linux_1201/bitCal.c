#include <stdio.h>

int main() {
	unsigned int num;
	printf("input num: ");
	scanf("%d", &num);

	printf("---change 2num---\n");

	for(int i = 7; i >= 0; i--) {
		int result = num >> i & 1;
		printf("%d", result);
	}
	printf("\n");
}
