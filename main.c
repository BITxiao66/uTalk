#include <stdio.h>

int main()
{
    char in[] = "aaa:bbb";
    char son1[10], son2[10];
    sscanf(in, "a%s:%s", son1, son2);
    puts(son1);
    puts(son2);
    return 0;
}