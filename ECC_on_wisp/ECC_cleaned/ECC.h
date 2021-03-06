/*
 * ECC.h
 *
 *  Created on: 10 feb. 2017
 *      Author: Casper
 */
#include"wisp-base.h"
#ifndef ECC_H_
#define ECC_H_

uint16_t* ECC_multiplication(uint16_t P[], uint8_t n, uint8_t a, uint8_t b, uint8_t p);
uint16_t* ECC_addition(uint16_t P1[], uint16_t P2[], uint8_t a, uint8_t b, uint8_t p);
uint16_t ModInv(uint16_t a, uint16_t m);
uint16_t gcdExtended(uint16_t a, uint16_t b, uint16_t *x, uint16_t *y);
uint16_t ipow(uint8_t base, uint8_t exp, uint8_t m);
uint16_t* main_ecc(uint8_t a, uint8_t b, uint8_t p, uint16_t start[], uint8_t privateKey);

#endif /* ECC_H_ */

