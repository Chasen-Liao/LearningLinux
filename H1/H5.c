#include <stdio.h>

double hermite(int n, int x) 
{
    if (n == 0) {
        return 1;
    }
    else if (n == 1){
        return 2 * x;
    }
    else {
        return 2 *x *hermite(n - 1, x) - 2 * x * hermite(n - 2, x);
    }
}

int main()
{
    int n, x;
    scanf("%d %d", &n, & x);
    
    double res = hermite(n ,x);
    printf("%.2lf\n", res);

    return 0;
}
