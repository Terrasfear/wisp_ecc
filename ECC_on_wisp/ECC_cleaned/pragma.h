/*
 * pragma.h
 *
 *  Created on: 23 mei 2017
 *      Author: Casper
 */

#ifndef PRAGMA_H_
#define PRAGMA_H_

#pragma PERSISTENT(QA)
extern uint16_t QA[3] = {0, 0, 0};

#pragma PERSISTENT(NA)
extern uint16_t NA = 0;

#pragma PERSISTENT(QB)
extern uint16_t QB[3] = {0, 0, 0};

#pragma PERSISTENT(NB)
extern uint16_t NB = 0;

#pragma PERSISTENT(SW)
extern uint8_t SW = 0;

#pragma PERSISTENT(R)
extern uint8_t R[2] = {0, 0};

#pragma PERSISTENT(F)
extern uint8_t F = 1;

#pragma PERSISTENT(M)
extern uint32_t M = 0;

#pragma PERSISTENT(RESULT)
extern uint16_t RESULT[3] = {0, 0, 0};

#endif /* PRAGMA_H_ */
