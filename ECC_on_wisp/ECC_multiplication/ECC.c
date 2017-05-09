/*
 * ECC.c
 *
 *  Created on: 10 feb. 2017
 *      Author: Casper
 */


#include "ECC.h"
#define MOD(x, p) (((x)%(p)) < 0 ? (((x)+(p))%(p)) : (((x)+(p))%(p)))


uint16_t ipow(uint16_t base, uint16_t exp, uint16_t m)
{
   uint16_t i, result = 1;
   uint32_t iresult;
   for(i=0 ; i < exp ; i++)
   {
       iresult = result*base;
	   result = MOD(iresult, m);
   }

    return result;
}

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

uint16_t* ECC_addition(uint16_t P1[], uint16_t P2[], uint16_t param[])
{
    uint32_t m;
    static uint16_t P3[3];

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
        (*(uint16_t*)(0x19E2)) = m;

        P3[0] = MOD(MOD(-P1[0],param[4]) + MOD(-P2[0], param[4]) + MOD(-param[1] * ipow(param[0], param[4]-2, param[4]), param[4]) + MOD(ipow(m,2, param[4]) * ipow(param[0], param[4]-2, param[4]), param[4]),param[4]);
        //P3[0] = MOD(P3[0], param[4]);
        //(*(uint16_t*)(0x19E8)) = MOD(-MOD(P1[0],param[4]) - MOD(P2[0], param[4]),param[4]);
        //(*(uint16_t*)(0x19EA)) = param[4];
        //(*(uint16_t*)(0x19EC)) = MOD(-5,param[4]);
        //(*(uint16_t*)(0x19E4)) = P3[0];
        P3[1] = MOD(-P1[1], param[4]) + MOD(m * MOD((P1[0] - P3[0]), param[4]), param[4]);
        P3[1] = MOD(P3[1], param[4]);
        (*(uint16_t*)(0x19E6)) = P3[1];
        P3[2] = 0;
        }
    return P3;
}

uint16_t* ECC_multiplication(uint16_t P[], uint16_t n, uint16_t param[])
{
    uint16_t* q;
    uint16_t Q[3];

	if ((*(uint16_t*)(0x1996)) == 0xFFFF)		/* Initialiseren bij eerste keer opstarten*/
	{
		(*(uint16_t*)(0x199E)) = P[0];
		(*(uint16_t*)(0x19A6)) = P[1];
		(*(uint16_t*)(0x19AE)) = 0;

		(*(uint16_t*)(0x19CE)) = n-1;
		(*(uint16_t*)(0x19D6)) = n;

	    (*(uint16_t*)(0x19DE)) = 0x0000;
	    (*(uint16_t*)(0x1996)) = 0x0000;
	}

	if ((*(uint16_t*)(0x19DE)) == 0x0000)		/* Inladen van de Q waarden wanneer de WISP is uitgevallen*/
	{
		Q[0] = (*(uint16_t*)(0x199E));
		Q[1] = (*(uint16_t*)(0x19A6));
		Q[2] = (*(uint16_t*)(0x19AE));
	}
	else
	{
		Q[0] = (*(uint16_t*)(0x19B6));
		Q[1] = (*(uint16_t*)(0x19BE));
		Q[2] = (*(uint16_t*)(0x19C6));
	}

    while((*(uint16_t*)(0x19CE)) >= 1 && (*(uint16_t*)(0x19D6)) >= 1)
    {
   		q = ECC_addition(P, Q, param);
   		Q[0] = *q;
   		Q[1] = *(q+1);
   		Q[2] = *(q+2);
   																/* Opslaan in non-volitile*/
   		if ((*(uint16_t*)(0x19DE)) == 0xFFFF)						/* Save slot A*/
		{
   			(*(uint16_t*)(0x199E)) = Q[0];
   			(*(uint16_t*)(0x19A6)) = Q[1];
			(*(uint16_t*)(0x19AE)) = Q[2];

			(*(uint16_t*)(0x19CE)) = (*(uint16_t*)(0x19D6)) - 1;

			(*(uint16_t*)(0x19DE)) = 0x0000;
		}
    	else													/* Save slot B*/
    	{
    		(*(uint16_t*)(0x19B6)) = Q[0];
    		(*(uint16_t*)(0x19BE)) = Q[1];
    		(*(uint16_t*)(0x19C6)) = Q[2];

    		(*(uint16_t*)(0x19D6)) = (*(uint16_t*)(0x19CE)) - 1;

    		(*(uint16_t*)(0x19DE)) = 0xFFFF;
    	}
    }
	(*(uint16_t*)(0x1996)) = 0xFFFF;
    return Q;
}

void main_ecc(uint16_t param[], uint16_t P[], uint16_t privateKey)
{
    uint16_t *K;
    uint8_t i;
    for (i=2; i<4; i++) param[i] = MOD(param[i], param[4]);

    uint16_t start[3] = {MOD(P[0],param[4]), MOD(P[1],param[4]), 0};

    K = ECC_multiplication(start, privateKey, param);

    //if((*(uint8_t*)(0x197E))==0x00){
    (*(uint16_t*)(0x1980))=*K;
    (*(uint16_t*)(0x1984))=*(K+1); //}
    //else{
    //(*(uint16_t*)(0x1988))=*K;
    //(*(uint16_t*)(0x198C))=*(K+1);}
}
