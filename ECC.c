#include <stdio.h>
#define MOD(x, p) (((x)%(p)) < 0 ? ((x)%(p) +(p)) : ((x)%(p)))


int ipow(int base, int exp)                     /*source: http://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int */
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

int gcdExtended(int a, int b, int *x, int *y)   /*source: http://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/*/
{
    if (a == 0)
    {
        *x = 0, *y = 1;
        return b;
    }
    int x1, y1;
    int gcd = gcdExtended(b%a, a, &x1, &y1);
    *x = y1 - (b/a) * x1;
    *y = x1;
    return gcd;
}

int ModInv(int a, int m)                            /*source (edited): http://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/*/
{
    int x, y;
    int g = gcdExtended(MOD(a, m), m, &x, &y);
    if (g != 1)
    {
        printf("Inverse of %d doesn't exist\n", a);
        return 0;
    }
    else
    {
        return MOD(x, m);
    }
}

int on_curve(int p[2], int param[5])
{
    if (MOD(ipow(p[1], 2), param[4]) == MOD(param[0]*ipow(p[0], 3) + param[1]*ipow(p[0], 2) + param[2]*p[0] + param[3], param[4])) return 1;
    else return 0;
}

void print_all_points(int par[5])
{
    int i, j;
    int p[2];
    printf("----------------------\nAll integer points on curve:\n\n");
    for(i = 0; i < par[4]; i++)
    {
        p[0] = i;
        for(j = 0; j < par[4]; j++)
        {
            p[1] = j;
            if (on_curve(p, par) == 1)
            {
                printf("(%d, %d), ", p[0], p[1]);
            }
        }
        printf("\n");
    }
    printf("----------------------\n");
}

int* ECC_addition(int P1[], int P2[], int param[])
{
    int m;
    static int q[2];

    if(*P1 == *P2)
    {
        m = (3 * param[0] * ipow(P1[0], 2) + 2 * param[1] * P1[0] + param[2]) * ModInv(2, param[4]) * ModInv(P1[1], param[4]);
    }
    else
    {
        m = (P2[1] -P1[1]) *ModInv((P2[0]-P1[0]), param[4]);
    }
    m  = MOD(m, param[4]);

    q[0] = - P1[0] - P2[0] - param[1] * ModInv(param[0], param[4]) + ipow(m,2) * ModInv(param[0], param[4]);
    q[0]  = MOD(q[0], param[4]);

    q[1] = - P1[1] + m * (P1[0] - q[0]);
    q[1]  = MOD(q[1], param[4]);

    return q;
}

int* generate_key (int private_key, int start[2], int param[5])
{
    static int P1[3], P2[3], P3[3];
    int* q;

    P1[0] = P2[0] = P3[0] = MOD(start[0], param[4]);    /*beginpunt inladen*/
    P1[1] = P2[1] = P3[1] = MOD(start[1], param[4]);
    P1[2] = P2[2] = P3[2] = 0;

                                        /*parameters modulo prime*/
    param[0] = MOD(param[0], param[4]);
    param[1] = MOD(param[1], param[4]);
    param[2] = MOD(param[2], param[4]);
    param[3] = MOD(param[3], param[4]);

    //printf("(%d, %d, %d)\n", P1[0], P1[1], P1[2]);
    while(private_key > 1)
    {
        printf("Adding P1(%d, %d) and P2(%d, %d): ", P1[0], P1[1], P2[0], P2[1]);

        if (P1[2] == 1)                         /*case one*/
        {
            //printf("case one\n");
            P3[0] = P2[0];
            P3[1] = P2[1];
            P3[2] = P2[2];
        }
        else if(P2[2] == 1)
        {
            //printf("case one\n");
            P3[0] = P1[0];
            P3[1] = P1[1];
            P3[2] = P1[2];
        }
        else if (P1[0] == P2[0] && (P1[1] != P2[1] || (P1[1] == 0 && P2[1] == 0))) /*case two*/
        {
            //printf("case two\n");
            //printf("%d = %d, &( %d != %d || (%d = 0 & %d = 0))\n", P1[0], P2[0], P1[1], P2[1], P1[1], P2[1]);
            P3[0]=0;
            P3[1]=0;
            P3[2]=1;
        }
        else                                    /* case three*/
        {
            //printf("case three\n");
            q = ECC_addition(P1, P2, param);
            P3[0] = *q;
            P3[1] = *(q+1);
        }
        if(on_curve(P3, param)!=1) printf("Computed point not on curve!\n");
        else printf("(%d, %d, %d)\n", P3[0], P3[1], P3[2]);

        P2[0] = P3[0];
        P2[1] = P3[1];
        P2[2] = P3[2];

        --private_key;
    }

    return P3;
}

int main(void)
{
    int start[2]={9,3};
    int private_key=5;

    /*int a, b, c, d, p;      /* y^2 = a x^3 + b x^2 + c x + d*/
    int param[5] = {1, 0, 2, 2, 37};
    int *Q;

    print_all_points(param);

    if(on_curve(start, param)!=1) printf("Start point not on curve!\n"); else printf("Start: (%d, %d)\n", start[0], start[1]);

    Q = generate_key(private_key, start, param);

    printf("Public Key: (%d, %d)", *Q, *(Q+1));

    return 0;
}
