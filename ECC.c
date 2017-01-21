
#include <stdio.h>

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

int on_curve(int p[2], int param[5])
{
    if (mod(ipow(p[1], 2), param[4]) == mod(param[0]*ipow(p[0], 3) + param[1]*ipow(p[0], 2) + param[2]*p[0] + param[3], param[4])) return 1;
    else return 0;
}

int mod (int a, int p)
{
   int ret = a % p;
   if(ret < 0)
     ret+=p;
   return ret;
}

void print_all_points(int par[5])
{
    int i, j;
    int p[2];
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
}

int* ECC_addition(int P1[], int P2[], int param[])
{
    int m;
    static int q[2];

    if(P1 == P2)
    {

        m = (3 * param[0] * ipow(P1[0], 2) + 2 * param[1] * P1[0] + param[2]) * ipow(2, param[4]-2) * ipow(P1[1], (param[4]-2));
        if (m >= param[4])
            m %= param[4];
    }
    else
    {
        m = (P2[1] -P1[1])*ipow((P2[0]-P1[0]),(param[4]-2));
        if (m >= param[4])
            m %= param[4];
    }

    q[0] = - P1[0] - P2[0] - param[1] * ipow(param[0], (param[4]-2)) + ipow(m,2) * ipow(param[0], (param[4]-2));
    if (q[0] >= param[4])
        q[0] %= param[4];

    q[1] = - P1[1] + m * (P1[0] - q[0]);
    if (q[1] >= param[4])
        q[1] %= param[4];

    return q;
}

int* generate_key (int private_key, int start[2], int param[5])
{
    static int P1[3], P2[3], P3[3];
    int* q;

    if (start[0] >= param[4])                   /* inladen van beginpunt*/
    {
        P1[0] = start[0] % param[4];
        P2[0] = start[0] % param[4];
    }
    else
    {
        P1[0] = start[0];
        P2[0] = start[0];
    }

    if (start[1] >= param[4])
    {
        P1[1] = start[1] % param[4];
        P2[1] = start[1] % param[4];
    }
    else
    {
        P1[1] = start[1];
        P2[1] = start[1];
    }
    P1[2] = 0;
    P2[2] = 0;
    P3[2] = 0;
                                        /*parameters modulo prime*/
    if (param[0] >= param[4])
        param[0] %= param[4];
    if (param[1] >= param[4])
        param[1] %= param[4];
    if (param[2] >= param[4])
        param[2] %= param[4];
    if (param[3] >= param[4])
        param[3] %= param[4];

    while(private_key >= 0)
    {
        if (P1[2] == 1)                         /*case one*/
        {
            P3[0] = P2[0];
            P3[1] = P2[1];
            P3[2] = P2[2];
        }
        else if(P2[2] == 1)
        {
            P3[0] = P1[0];
            P3[1] = P1[1];
            P3[2] = P1[2];
        }
        else if (P1[0] == P2[0] && (P1[1] != P2[1] || (P1[1] == 0 && P2[1] == 0))) /*case two*/
        {
            printf("%d = %d, &( %d != %d || (%d = 0 & %d = 0))\n", P1[0], P2[0], P1[1], P2[1], P1[1], P2[1]);
            P3[0]=0;
            P3[1]=0;
            P3[2]=1;
        }
        else                                    /* case three*/
        {
            q = ECC_addition(P1, P2, param);
            P3[0] = *q;
            P3[1] = *(q+1);
        }
        printf("(%d, %d, %d)\n", P3[0], P3[1], P3[2]);
        P2[0] = P3[0];
        P2[1] = P3[1];
        P2[2] = P3[2];

        --private_key;
    }

    return P3;
}






int main(void)
{
    int start[2]= {0,3};
    int private_key=3;
    /*int a, b, c, d, p;      /* y^2 = a x^3 + b x^2 + c x + d*/
                            /* p staat voor priem getal*/
    int param[5] = {1, 0, -3, 9, 13};
    
    //int param[5] = {1, 0, 2, 9, 37};              /*parameters van https://arxiv.org/ftp/arxiv/papers/1202/1202.1895.pdf*/
    //print_all_points(param);
    
    int *Q;

    Q = generate_key(private_key, start, param);

    printf("(%d, %d)", *Q, *(Q+1));

    return 0;
}
