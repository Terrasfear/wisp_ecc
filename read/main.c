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

  while(FOREVER){

	  //Saying 'Hello!'
	  *wispData.readBufPtr = 0x48;
	  *(wispData.readBufPtr + 1) = 0x65;
	  *(wispData.readBufPtr + 2) = 0x6C;
	  *(wispData.readBufPtr + 3) = 0x6C;
	  *(wispData.readBufPtr + 4) = 0x6F;
	  *(wispData.readBufPtr + 5) = 0x21;

	  WISP_doRFID();
  }
}
