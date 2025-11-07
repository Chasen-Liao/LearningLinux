#include <stdio.h>

#define MAX 1000

int main(int argc, char *argv[])
{
        int max = MAX;
        int sum = 0;
        int i = 1;

        while(i <= max)
                sum += i++;

        printf("sum=%d\n", sum);

        return 0;
}
