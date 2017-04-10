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
   uint16_t iresult;
   for(i=0 ; i < exp ; i++)
   {
       iresult = result*base;
	   result = MOD(iresult, m);
   }

    return result;
}
/*
// source: http://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
uint16_t gcdExtended(uint16_t a, uint16_t b, uint16_t *x, uint16_t *y)
{
    if (a == 0)
    {
        *x = 0, *y = 1;
        return b;
    }
    uint16_t x1, y1;
    uint16_t gcd = gcdExtended(b%a, a, &x1, &y1);
    *x = y1 - (b/a) * x1;
    *y = x1;
    return gcd;
}

// source (edited): http://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
uint16_t ModInv(uint16_t a, uint16_t m)
{

    uint16_t x, y;
    uint16_t g = gcdExtended(MOD(a, m), m, &x, &y);

    return MOD(x, m);
}
*/
uint8_t* ECC_addition(uint8_t P1[], uint8_t P2[], uint8_t param[])
{
    uint16_t m;
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
        	m = (MOD(3 * param[0] * ipow(P1[0], 2, param[4]), param[4]) + MOD(2 * param[1] * P1[0] + param[2], param[4])) * MOD(ipow(2, param[4]-2, param[4]) * ipow(P1[1], param[4]-2, param[4]), param[4]);
        }
        else
        {
        	m = MOD((P2[1] - P1[1]), param[4]) * ipow((MOD(P2[0]-P1[0], param[4])), param[4]-2, param[4]);
        }

        m  = MOD(m, param[4]);
        (*(uint8_t*)(0x19E2)) = m;

        P3[0] = MOD(- P1[0] - P2[0], param[4]) - MOD(param[1] * ipow(param[0], param[4]-2, param[4]), param[4]) + MOD(ipow(m,2, param[4]) * ipow(param[0], param[4]-2, param[4]), param[4]);
        P3[0] = MOD(P3[0], param[4]);

        P3[1] = MOD(-P1[1], param[4]) + MOD(m * MOD((P1[0] - P3[0]), param[4]), param[4]);
        P3[1] = MOD(P3[1], param[4]);

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
		(*(uint8_t*)(0x199E)) = P[0];
		(*(uint8_t*)(0x19A6)) = P[1];
		(*(uint8_t*)(0x19AE)) = 0;

		(*(uint8_t*)(0x19CE)) = n-1;
		(*(uint8_t*)(0x19D6)) = n;

	    (*(uint8_t*)(0x19DE)) = 0x00;
	    (*(uint8_t*)(0x1996)) = 0x00;
	}

	if ((*(uint8_t*)(0x19DE)) == 0x00)		/* Inladen van de Q waarden wanneer de WISP is uitgevallen*/
	{
		Q[0] = (*(uint8_t*)(0x199E));
		Q[1] = (*(uint8_t*)(0x19A6));
		Q[2] = (*(uint8_t*)(0x19AE));
	}
	else
	{
		Q[0] = (*(uint8_t*)(0x19B6));
		Q[1] = (*(uint8_t*)(0x19BE));
		Q[2] = (*(uint8_t*)(0x19C6));
	}

    while((*(uint8_t*)(0x19CE)) >= 1 && (*(uint8_t*)(0x19D6)) >= 1)
    {
   		q = ECC_addition(P, Q, param);
   		Q[0] = *q;
   		Q[1] = *(q+1);
   		Q[2] = *(q+2);
   																/* Opslaan in non-volitile*/
   		if ((*(uint8_t*)(0x19DE)) == 0xFF)						/* Save slot A*/
		{
   			(*(uint8_t*)(0x199E)) = Q[0];
   			(*(uint8_t*)(0x19A6)) = Q[1];
			(*(uint8_t*)(0x19AE)) = Q[2];

			(*(uint8_t*)(0x19CE)) = (*(uint8_t*)(0x19D6)) - 1;

			(*(uint8_t*)(0x19DE)) = 0x00;
		}
    	else													/* Save slot B*/
    	{
    		(*(uint8_t*)(0x19B6)) = Q[0];
    		(*(uint8_t*)(0x19BE)) = Q[1];
    		(*(uint8_t*)(0x19C6)) = Q[2];

    		(*(uint8_t*)(0x19D6)) = (*(uint8_t*)(0x19CE)) - 1;

    		(*(uint8_t*)(0x19DE)) = 0xFF;
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
