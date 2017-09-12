/*
 * ECC.c
 *
 *  Created on: 10 feb. 2017
 *      Authors: Casper & Martijn
 */


#include "ECC.h"
#include "pragma.h"
#define MOD(x, p) (((x)+(p))%p)


uint16_t ipow(uint8_t base, uint8_t exp, uint8_t m)
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

uint16_t* ECC_addition(uint16_t P1[], uint16_t P2[], uint8_t a, uint8_t b, uint8_t p)
{
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
        	M = MOD(3 * ipow(P1[0], 2, p) + a, p) * MOD(ipow(2, p-2, p) * ipow(P1[1], p-2, p), p);
        }
        else
        {
        	M = MOD((P2[1] - P1[1]), p) * ipow((MOD(P2[0]-P1[0], p)), p-2, p);
        }

        M  = MOD(M, p);

        P3[0] = MOD(- P1[0] - P2[0] + ipow(M,2, p),p);

        P3[1] = MOD(- P1[1], p) + MOD(M * MOD((P1[0] - P3[0]), p), p);
        P3[1] = MOD(P3[1], p);

        P3[2] = 0;
        }
    return P3;
}

uint16_t* ECC_multiplication(uint16_t P[], uint8_t n, uint8_t a, uint8_t b, uint8_t p)
{
    uint16_t* q;
    uint16_t Q[3];


	if (F == 1)		/* Initialiseren bij eerste keer opstarten*/
	{
		QA[0] = P[0];
		QA[1] = P[1];
		QA[2] = 0;

		NA = n-1;
		NB = n;

	    SW = 0;
	    F = 0;
	}

	if (SW == 0)		/* Inladen van de Q waarden wanneer de WISP is uitgevallen*/
	{
		Q[0] = QA[0];
		Q[1] = QA[1];
		Q[2] = QA[2];
	}
	else
	{
		Q[0] = QB[0];
		Q[1] = QB[1];
		Q[2] = QB[2];
	}


    while(NA >= 1 && NB >= 1)
    {
   		q = ECC_addition(P, Q, a, b, p);
   		Q[0] = *q;
   		Q[1] = *(q+1);
   		Q[2] = *(q+2);
		
   		if (SW == 1)						/* Save slot A*/
		{
   			QA[0] = Q[0];
   			QA[1] = Q[1];
   			QA[2] = Q[2];

			NA = NB - 1;

			SW = 0;
		}
    	else								/* Save slot B*/
    	{
    		QB[0] = Q[0];
    		QB[1] = Q[1];
    		QB[2] = Q[2];

    		NB= NA - 1;

    		SW = 1;
    	}
    }
	F = 1;
    return Q;
}

uint16_t* main_ecc(uint8_t a, uint8_t b, uint8_t p, uint16_t P[], uint8_t privateKey)
{
    uint16_t *K;
    a = MOD(a,p);
    b = MOD(b,p);

    uint16_t start[3] = {MOD(P[0],p), MOD(P[1],p), 0};

    K = ECC_multiplication(start, privateKey, a, b, p);

    R[0]=*K;
    R[1]=*(K+1);

    return R;
}
