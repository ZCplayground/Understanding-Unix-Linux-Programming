/*
A easily used API
man pwd --> SEE ALSO --> man 3 getcwd
*/
#include<stdio.h>
#include<unistd.h>

int main()
{
	char wd[4096];
	getcwd(wd, 4096);
	printf("%s\n", wd);
	return 0;
}
