#ifndef _RADIO_H_
#define _RADIO_H_

#include <SPI.h>
#include <RFM69.h>

RFM69 rfm69;

class RadioStream : public Stream
{
  public:
    virtual size_t write(uint8_t value) { return Serial.write(value); }
    virtual void send() { ; }
    void begin() {; }

    virtual int available() { return Serial.available(); }
    virtual int read() { return Serial.read(); }
    virtual int peek() { return Serial.peek(); }
    virtual void flush() { Serial.flush(); }
};

  /*
  radio.initialize(FREQUENCY, NODEID, NETWORKID);
#ifdef IS_RFM69HW
  radio.setHighPower();  //uncomment only for RFM69HW!
#endif
  radio.encrypt(ENCRYPTKEY);
  radio.readAllRegs();

  if (radio.sendWithRetry(GATEWAYID, buff, buffLen))
  if (radio.receiveDone())
  {
    io.print('['); io.print(radio.SENDERID, DEC); io.print("] ");
    for (byte i = 0; i < radio.DATALEN; i++)
      io.print((char)radio.DATA[i]);
    io.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");

    if (radio.ACKRequested())
    {
      radio.sendACK();
    }
  }
  */

#endif

