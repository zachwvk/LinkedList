#include <stdio.h>

void list_test(void);
void run_demo(void);

int main(int argc, char **argv)
{
	int c;
	
	printf("Hello World\r\n");

	run_demo();
	//list_test();

	printf("Press any Key\r\n");
	c = getchar();
}