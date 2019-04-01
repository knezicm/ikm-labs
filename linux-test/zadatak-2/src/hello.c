#include "helloLib.h"

int main()
{
	int a = 25;
	int b = 78;
	int c;

	printFirstNumber(a);
	printSecondNumber(b);
	c = addNumbers(a, b);
	printResult(c);

	return 0;
}
