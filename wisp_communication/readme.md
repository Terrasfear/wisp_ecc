Author: Martijn van der Marel

Attempt to write data to the wisp using sllurp.

Current problem: When setting the abort conditions to write only when trying to write sth to the wisp, the wisp gets stuck in the do_RFID state (due to no successfull write command, most likely).
Problem is most likely due to sllurp library not being set up properly.

In this folder (only core files, rest of sllurp library and CCS is standard):

sllurp:

  -sllurp/access file (edited to send test words: see access(proto) -> writeSpecParam -> WriteData)
  
  -bin/access file
  
WISP:

  -CCS file:
    This code first waits for a write command and stores the data from wispData.writeBufPtr in non-volatile memory.
    Then, it sets up the wispData.readBufPtr and sends back to the reader. (This part works)
    
Attempted execution: 
- go to the bin folder in git bash
- run the following line: ./access -M WISP5 -T 7140 192.168.178.21 -l wisp_communication.log -w 2
- This should write 2 words to the wisp.
- Hold wisp near reader antenna for a few seconds.
- FLAG CHECK: if words are successfully written (and are not 0xFF), the first two entries of the epc buffer should change from AA to BB.
- Stop command in git and run the following line: ./access -M WISP5 -T 7140 192.168.178.21 -l ecctest.log -r 2
- This should start the reading process.
- Hold wisp near reader antenna for a few seconds.
- In the git window, the ReadData should contain the words that were sent.
