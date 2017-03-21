/*
 * ECC.c
 *
 *  Created on: 10 feb. 2017
 *      Author: Casper
 */


#include "ECC.h"
#define MOD(x, p) (((x)%(p)) < 0 ? ((x)%(p) +(p)) : ((x)%(p)))


uint8_t ipow(uint8_t base, uint8_t exp, uint8_t m)
{
   uint8_t i, result = 1;

   for(i=0 ; i < exp ; i++)
   {
       result = MOD(result*base, m);
   }

    return result;
}

// source: http://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
uint8_t gcdExtended(uint8_t a, uint8_t b, uint8_t *x, uint8_t *y)
{
    if (a == 0)
    {
        *x = 0, *y = 1;
        return b;
    }
    uint8_t x1, y1;
    uint8_t gcd = gcdExtended(b%a, a, &x1, &y1);
    *x = y1 - (b/a) * x1;
    *y = x1;
    return gcd;
}

// source (edited): http://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
uint8_t ModInv(uint8_t a, uint8_t m)
{

    uint8_t x, y;
    uint8_t g = gcdExtended(MOD(a, m), m, &x, &y);

    return MOD(x, m);
}
uint8_t* ECC_addition(uint8_t P1[], uint8_t P2[], uint8_t param[])
{
    uint8_t m;
    static uint8_t P3[3];

    if (P1[2] == 1)                                                                                         /*case one*/
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
    else if (P1[0] == P2[0] && (P1[1] != P2[1] || (P1[1] == 0 && P2[1] == 0)))                              /*case two*/
    {
        P3[0]=0;
        P3[1]=0;
        P3[2]=1;
    }
    else                                                                                                    /* case three*/
    {
        if (*P1 == *P2)
        {
        m = (3 * param[0] * ipow(P1[0], 2, param[4]) + 2 * param[1] * P1[0] + param[2]) * ModInv(2, param[4]) * ModInv(P1[1], param[4]);
        }
        else
        {
        m = (P2[1] -P1[1]) * ModInv((P2[0]-P1[0]), param[4]);
        }
        m  = MOD(m, param[4]);

        P3[0] = - P1[0] - P2[0] - param[1] * ModInv(param[0], param[4]) + ipow(m,2, param[4]) * ModInv(param[0], param[4]);
        P3[0]  = MOD(P3[0], param[4]);

        P3[1] = - P1[1] + m * (P1[0] - P3[0]);
        P3[1]  = MOD(P3[1], param[4]);
        P3[2] = 0;

        }
    return P3;
}

uint8_t* ECC_multiplication(uint8_t P[], uint8_t n, uint8_t param[])
{
    uint8_t* q;
    uint8_t Q[3];

	if ((*(uint8_t*)(0x1996)) == 0xFF)		/* Initialiseren bij eerste keer opstarten*/
	{
		(*(uint8_t*)(0x2004)) = P[0];
		(*(uint8_t*)(0x2012)) = P[1];
		(*(uint8_t*)(0x2020)) = 0;

		(*(uint8_t*)(0x2052)) = n-1;
		(*(uint8_t*)(0x2060)) = n;

	    (*(uint8_t*)(0x2068)) = 0x00;
	    (*(uint8_t*)(0x1996)) = 0x00;
	}

	if ((*(uint8_t*)(0x2068)) == 0x00)		/* Inladen van de Q waarden wanneer de WISP is uitgevallen*/
	{
		Q[0] = (*(uint8_t*)(0x2004));
		Q[1] = (*(uint8_t*)(0x2012));
		Q[2] = (*(uint8_t*)(0x2020));
	}
	else
	{
		Q[0] = (*(uint8_t*)(0x2028));
		Q[1] = (*(uint8_t*)(0x2036));
		Q[2] = (*(uint8_t*)(0x2044));
	}

    while((*(uint8_t*)(0x2052)) >= 1 && (*(uint8_t*)(0x2060)) >= 1)
    {
   		q = ECC_addition(P, Q, param);
   		Q[0] = *q;
   		Q[1] = *(q+1);
   		Q[2] = *(q+2);
   																/* Opslaan in non-volitile*/
   		if ((*(uint8_t*)(0x2068)) == 0xFF)						/* Save slot A*/
		{
   			(*(uint8_t*)(0x2004)) = Q[0];
   			(*(uint8_t*)(0x2012)) = Q[1];
			(*(uint8_t*)(0x2020)) = Q[2];

			(*(uint8_t*)(0x2052)) = (*(uint8_t*)(0x2060)) - 1;

			(*(uint8_t*)(0x2068)) = 0x00;
		}
    	else													/* Save slot B*/
    	{
    		(*(uint8_t*)(0x2028)) = Q[0];
    		(*(uint8_t*)(0x2036)) = Q[1];
    		(*(uint8_t*)(0x2044)) = Q[2];

    		(*(uint8_t*)(0x2060)) = (*(uint8_t*)(0x2052)) - 1;

    		(*(uint8_t*)(0x2068)) = 0xFF;
    	}
    }
	(*(uint8_t*)(0x1996)) = 0xFF;
    return Q;
}

void main_ecc(uint8_t param[], uint8_t P[], uint8_t privateKey)
{
    uint8_t *K;
    uint8_t start[3] = {P[0], P[1], 0};

    K = ECC_multiplication(start, privateKey, param);

    (*(uint8_t*)(0x1980))=*K;
    (*(uint8_t*)(0x1988))=*(K+1);
}
