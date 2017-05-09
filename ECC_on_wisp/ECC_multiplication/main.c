/**
 * @file       usr.c
 * @brief      WISP application-specific code set
 * @details    The WISP application developer's implementation goes here.
 *
 * @author     Aaron Parks, UW Sensor Systems Lab
 *
 */

#include "wisp-base.h"
#include "ECC.h"

WISP_dataStructInterface_t wispData;

/** 
 * This function is called by WISP FW after a successful ACK reply
 *
 */
void my_ackCallback (void) {
  asm(" NOP");
}

/**
 * This function is called by WISP FW after a successful read command
 *  reception
 *
 */
void my_readCallback (void) {
  asm(" NOP");
}

/**
 * This function is called by WISP FW after a successful write command
 *  reception
 *
 */
void my_writeCallback (void) {
  asm(" NOP");
}

/** 
 * This function is called by WISP FW after a successful BlockWrite
 *  command decode

 */
void my_blockWriteCallback  (void) {
  asm(" NOP");
}


/**
 * This implements the user application and should never return
 *
 * Must call WISP_init() in the first line of main()
 * Must call WISP_doRFID() at some point to start interacting with a reader
 */
void main(void) {

  WISP_init();

  // Register callback functions with WISP comm routines
  WISP_registerCallback_ACK(&my_ackCallback);
  WISP_registerCallback_READ(&my_readCallback);
  WISP_registerCallback_WRITE(&my_writeCallback);
  WISP_registerCallback_BLOCKWRITE(&my_blockWriteCallback);
  
  // Initialize BlockWrite data buffer.
  uint16_t bwr_array[6] = {0};
  RWData.bwrBufPtr = bwr_array;
  
  // Get access to EPC, READ, and WRITE data buffers
  WISP_getDataBuffers(&wispData);
  
  // Set up operating parameters for WISP comm routines
  WISP_setMode( MODE_READ | MODE_WRITE | MODE_USES_SEL); 
  WISP_setAbortConditions(CMD_ID_READ /*| CMD_ID_WRITE | CMD_ID_ACK*/);
  
  // Set up EPC
  wispData.epcBuf[0] = 0x0B; 		// Tag type
  wispData.epcBuf[1] = 0;			// Unused data field
  wispData.epcBuf[2] = 0;			// Unused data field
  wispData.epcBuf[3] = 0;			// Unused data field
  wispData.epcBuf[4] = 0;			// Unused data field
  wispData.epcBuf[5] = 0;			// Unused data field
  wispData.epcBuf[6] = 0;			// Unused data field
  wispData.epcBuf[7] = 0x00;		// Unused data field
  wispData.epcBuf[8] = 0x00;		// Unused data field
  wispData.epcBuf[9] = 0x51;		// Tag hardware revision (5.1)
  wispData.epcBuf[10] = *((uint8_t*)INFO_WISP_TAGID+1); // WISP ID MSB: Pull from INFO seg
  wispData.epcBuf[11] = *((uint8_t*)INFO_WISP_TAGID); // WISP ID LSB: Pull from INFO seg
  
 //Loading parameters:
 //read public key from pc
 //possibly read curve parameters
 //private key should stay in-code!
 //pcpublickey = ;
 //WISP_doRFID();

  wispData.epcBuf[1] = 0x0A;

  uint16_t param[5] = {1, 0, 7, 8, 313};
  uint16_t start[2] = {4, 10};
  uint16_t key = 5;

  //if((*(uint8_t*)(0x197E))==0x00)
  main_ecc(param, start, key);
  //wispData.epcBuf[1] = 0x0B;
  //(*(uint8_t*)(0x197E)) = 0x01;
  //main_ecc(param, pcpublickey, key)
  //(*(uint8_t*)(0x197E)) = 0x00;
  //wispData.epcBuf[1] = 0x0C;
  // Talk to the RFID reader.
  while (FOREVER) {

	  *wispData.readBufPtr = (*(uint16_t*)(0x1980));
	  *(wispData.readBufPtr + 1) = (*(uint16_t*)(0x1984));
	  //*(wispData.readBufPtr + 2) = (*(uint16_t*)(0x1988)); //usually kept in code as result of key exchange
	  //*(wispData.readBufPtr + 3) = (*(uint16_t*)(0x198C)); //as above
	  //*(wispData.readBufPtr + 2) = (*(uint16_t*)(0x1996));

	  WISP_doRFID();
  }
}
