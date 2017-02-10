/*
 * ECC.h
 *
 *  Created on: 10 feb. 2017
 *      Author: Casper
 */
#include"wisp-base.h"
#ifndef ECC_H_
#define ECC_H_

uint8_t* ECC_multiplication(uint8_t P[], uint8_t n, uint8_t param[]);
uint8_t* ECC_addition(uint8_t P1[], uint8_t P2[], uint8_t param[]);
uint8_t ModInv(uint8_t a, uint8_t m);
uint8_t gcdExtended(uint8_t a, uint8_t b, uint8_t *x, uint8_t *y);
uint8_t ipow(uint8_t base, uint8_t exp, uint8_t m);
void main_ecc(uint8_t param[], uint8_t start[], uint8_t privateKey);

#endif /* ECC_H_ */

