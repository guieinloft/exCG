#include <stdio.h>

int main()
{
	int a = 0x7f7f7f7f;
	float b = *((float*)&a);

	printf("%f\n", b);
}
