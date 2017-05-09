/*
 * ECC.h
 *
 *  Created on: 10 feb. 2017
 *      Author: Casper
 */
#include"wisp-base.h"
#ifndef ECC_H_
#define ECC_H_

uint16_t* ECC_multiplication(uint16_t P[], uint16_t n, uint16_t param[]);
uint16_t* ECC_addition(uint16_t P1[], uint16_t P2[], uint16_t param[]);
uint16_t ModInv(uint16_t a, uint16_t m);
uint16_t gcdExtended(uint16_t a, uint16_t b, uint16_t *x, uint16_t *y);
uint16_t ipow(uint16_t base, uint16_t exp, uint16_t m);
void main_ecc(uint16_t param[], uint16_t start[], uint16_t privateKey);

#endif /* ECC_H_ */
