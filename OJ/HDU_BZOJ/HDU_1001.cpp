#include <cstdio>

int main()
{
	long long n;
	while(scanf("%lld", &n) != EOF)
		printf("%lld\n\n", (n*(n+1)>>1));
	return 0;
}