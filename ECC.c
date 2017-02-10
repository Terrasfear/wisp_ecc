#include <stdio.h>
#include <stdlib.h>
#define MOD(x, p) (((x)%(p)) < 0 ? ((x)%(p) +(p)) : ((x)%(p)))


int param[5], start[2];

int isPrime(int num)               /*http://stackoverflow.com/questions/5281779/c-how-to-test-easily-if-it-is-prime-number */
{
    int i;

    if (num <= 1) return 0;
    if (num % 2 == 0 && num > 2) return 0;
    for(i = 3; i < num / 2; i+= 2)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int ipow(int base, int exp, int m)
{
   int i, result = 1;

   for(i=0 ; i < exp ; i++)
   {
       result = MOD(result*base, m);
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

int onCurve(int p[2])
{

    if (MOD(ipow(p[1], 2, param[4]), param[4]) == MOD(param[0]*ipow(p[0], 3, param[4]) + param[1]*ipow(p[0], 2, param[4]) + param[2]*p[0] + param[3], param[4])) return 1;
    else return 0;
}

void PrintAllPoints(void)
{
    int i, j, count;
    count = 0;
    int p[2];
    printf("----------------------\nAll integer points on curve:\n\n");
    for(i = 0; i < param[4]; i++)
    {
        p[0] = i;
        for(j = 0; j < param[4]; j++)
        {
            p[1] = j;
            if (onCurve(p) == 1)
            {
                printf("(%d, %d), ", p[0], p[1]);
                count++;
            }
        }
        printf("\n");
    }
    printf("Amount of integer points on curve: %d\n", count);
    printf("----------------------\n");
}

int* ECC_addition(int P1[], int P2[])
{
    int m;
    static int P3[3];

    if (P1[2] == 1)                                                                                         /*case one*/
    {
        /*printf("case one\n");*/
        P3[0] = P2[0];
        P3[1] = P2[1];
        P3[2] = P2[2];

    }
    else if(P2[2] == 1)
    {
        /*printf("case one\n");*/
        P3[0] = P1[0];
        P3[1] = P1[1];
        P3[2] = P1[2];
    }
    else if (P1[0] == P2[0] && (P1[1] != P2[1] || (P1[1] == 0 && P2[1] == 0)))                              /*case two*/
    {

        /*printf("case two\n");*/
        P3[0]=0;
        P3[1]=0;
        P3[2]=1;

    }
    else                                                                                                    /* case three*/
    {
        /*printf("case three\n");*/
        if (*P1 == *P2)
        {
        m = (3 * param[0] * ipow(P1[0], 2, param[4]) + 2 * param[1] * P1[0] + param[2]) * ModInv(2, param[4]) * ModInv(P1[1], param[4]);
        }
        else
        {
        m = (P2[1] -P1[1]) * ModInv((P2[0]-P1[0]), param[4]);
        }
        m  = MOD(m, param[4]);
        /*printf("m= %d\n", m);*/


        P3[0] = - P1[0] - P2[0] - param[1] * ModInv(param[0], param[4]) + ipow(m,2, param[4]) * ModInv(param[0], param[4]);
        P3[0]  = MOD(P3[0], param[4]);

        P3[1] = - P1[1] + m * (P1[0] - P3[0]);
        P3[1]  = MOD(P3[1], param[4]);
        P3[2] = 0;

        }
    return P3;
}

int* ECC_multiplication(int P[], int n)
{
    int* q;
    int Q[3];
    Q[0] = P[0];
    Q[1] = P[1];
    Q[2] = 0;
    printf("Start: (%d, %d, %d)\n", Q[0], Q[1], Q[2]);
    while(n > 1)
    {
        printf("Adding P(%d, %d) and Q(%d, %d): ", P[0], P[1], Q[0], Q[1]);
        q = ECC_addition(P, Q);
        Q[0] = *q;
        Q[1] = *(q+1);
        Q[2] = *(q+2);
        printf("(%d, %d, %d)\n", Q[0], Q[1], Q[2]);
        if (onCurve(Q)!=1 && Q[2]!=1) printf("Calculated point not on curve!\n");
        --n;
    }
    return Q;
}


void getParam(void)
{
    /*static int param[5];*/
    param[0] = 1;
    param[1] = 0;
    printf("ECC calculator for Y^2=X^3+aX+b curves in Zp environment\n");
    printf("Input curve parameters\n");
    printf("a = ");
    scanf("%d", &param[2]);
    printf("b = ");
    scanf("%d", &param[3]);
    printf("prime = ");
    scanf("%d", &param[4]);
    if (isPrime(param[4]) == 0)
    {
        printf("\n%d is not a prime!", param[4]);
        exit(0);
    }
    param[2] =  MOD(param[2], param[4]);
    param[3] =  MOD(param[3], param[4]);
}

void getStart(void)
{
    /*static int start[2];*/
    printf("Input public starting point (x,y)\nx = ");
    scanf("%d", &start[0]);
    printf("y = ");
    scanf("%d", &start[1]);
    if (onCurve(start) == 0)
    {
        printf("\n(%d, %d) is not on the curve!", start[0], start[1]);
        exit(0);
    }
}

int getPrivateKey(void)
{
    static int private_key;
    printf("Input private key (alpha): ");
    scanf("%d", &private_key);
    return private_key;
}

int main(void)
{
    int privateKey;
    int *K;

    getParam();
    PrintAllPoints();
    getStart();
    privateKey = getPrivateKey();


    K = ECC_multiplication(start, privateKey);

    printf("Generated public key: (%d, %d)", *K, *(K+1));

    return 0;
}

