/**
 * @file       Bidirectional epc
 * @brief      Read epc data from reader and send back
 *
 *
 * @author     Martijn van der Marel (Delft University of Technology), edited from Aaron Parks (UW Sensor Systems Lab)
 *
 */

#include "wisp-base.h"

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
	  //(*(uint8_t*)(0x1950))=*wispData.readBufPtr;
}

/**
 * This function is called by WISP FW after a successful write command
 *  reception
 *
 */
void my_writeCallback (void) {
	  //(*(uint8_t*)(0x1980))=*wispData.writeBufPtr;
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

  // Get access to EPC, READ, and WRITE data buffers
  WISP_getDataBuffers(&wispData);

  // Set up operating parameters for WISP comm routines
  WISP_setMode( MODE_READ | MODE_WRITE | MODE_USES_SEL);
  //WISP_setAbortConditions(CMD_ID_READ | CMD_ID_WRITE | CMD_ID_ACK);
  //WISP_setAbortConditions(CMD_ID_ACK);
  WISP_setAbortConditions(CMD_ID_READ);
  //WISP_setAbortConditions(CMD_ID_WRITE);
  //WISP_setAbortConditions(CMD_ID_READ | CMD_ID_WRITE);


  // Set up EPC
  wispData.epcBuf[0] = 0; 		// Tag type (0x0B)
  wispData.epcBuf[1] = 0;		// Unused data field
  wispData.epcBuf[2] = 0;			// Unused data field
  wispData.epcBuf[3] = 0;			// Unused data field
  wispData.epcBuf[4] = 0;			// Unused data field
  wispData.epcBuf[5] = 0;			// Unused data field
  wispData.epcBuf[6] = 0;			// Unused data field
  wispData.epcBuf[7] = 0x00;		// Unused data field
  wispData.epcBuf[8] = 0x00;		// Unused data field
  wispData.epcBuf[9] = 0x00;		// Tag hardware revision (5.1)
 // wispData.epcBuf[10] = *((uint8_t*)INFO_WISP_TAGID+1); // WISP ID MSB: Pull from INFO seg
 // wispData.epcBuf[11] = *((uint8_t*)INFO_WISP_TAGID); // WISP ID LSB: Pull from INFO seg
  wispData.epcBuf[10] = 0;
  wispData.epcBuf[11] = 0;

  uint8_t i = (*(uint8_t*)(0x1960));
  //*wispData.readBufPtr = 0x40;									//'@@' as first word
  //*(wispData.readBufPtr+1) = 0x40;

  while(FOREVER){

	  uint8_t f = (*(uint8_t*)(0x1980));
	  if(f == 0xFF){
		  wispData.epcBuf[0] = 0xAA; 							//TEST FLAG
		  (*(uint8_t*)(0x1960)) = 0x00;
		  //WISP_setMode(MODE_WRITE);
		  //WISP_setAbortConditions(CMD_ID_WRITE);
		  WISP_doRFID();
		  //(*(uint8_t*)(0x1980)) = wispData.writeBufPtr[0] & 0xFF;
		  (*(uint8_t*)(0x1980)) = 0x21;  						//TEST FLAG
	  }
	  else{
		  wispData.epcBuf[0] = 0xBB; 							//TEST FLAG
		  //WISP_setMode(MODE_READ);
		  WISP_setAbortConditions(CMD_ID_READ);
		  *wispData.readBufPtr = (*(uint8_t*)(0x1980));			//'!!' as first word
		  *(wispData.readBufPtr+1) = (*(uint8_t*)(0x1980));
		  WISP_doRFID();
	  }
	  i = (*(uint8_t*)(0x1960));
	  i = i + 1;
	  (*(uint8_t*)(0x1960)) = i;
	  *(wispData.readBufPtr + 2) = i;							//iteration variable i as second word
  }
}
