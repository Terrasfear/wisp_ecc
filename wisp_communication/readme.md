Author: Martijn van der Marel

Attempt to write data to the wisp using sllurp.

Current problem: When setting the abort conditions to write only when trying to write sth to the wisp, the wisp gets stuck in the do_RFID state (due to no successfull write command, most likely).
Problem is most likely due to sllurp library not being set up properly.

In this folder:
sllurp:
  -sllurp/access file (edited to send test words: see access(proto) -> writeSpecParam -> WriteData)
  -bin/access file
WISP:
  -CCS file:
    This code first waits for a write command and stores the data from wispData.writeBufPtr in non-volatile memory.
    Then, it sets up the wispData.readBufPtr and sends back to the reader. (This part works)
