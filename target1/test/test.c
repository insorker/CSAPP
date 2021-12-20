#include <stdio.h>
#include <string.h>

char buf[100];
int cookie = 0x59b997fa;

int main() {
	sprintf(buf, "%.4x", cookie);
	printf("%ld, %s\n", strlen(buf), buf);
	return 0;
}
