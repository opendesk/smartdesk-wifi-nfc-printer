#include <Mailbox.h>
#include "SPI.h"
#include "PN532_SPI.h"
#include "snep.h"
#include "NdefMessage.h"
#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"
#include "NfcAdapter.h"
#include "med_logo.h"

int printer_RX_Pin = 6;  // This is the green wire
int printer_TX_Pin = 7;  // This is the yellow wire

int buzz_pin = 11; // BUZZZ

PN532_SPI pn532spi(SPI, 10);
SNEP nfc(pn532spi);
uint8_t ndefBuf[128];
uint8_t recordBuf[128];

NfcAdapter nfc_adapter = NfcAdapter(pn532spi); // create an NFC adapter object

Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);

void makeNoise()
{
  // we are on
  digitalWrite(buzz_pin, HIGH);
  delay(500);
  digitalWrite(buzz_pin, LOW);
}


void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  // Initialize Bridge and Mailbox
  Bridge.begin();
  digitalWrite(13, HIGH);
  
  // Initialize Printer
  printer.begin();
  printer.setSize('L');
  printer.println("I am Configured");
  printer.feed(2);
  printer.printBitmap(med_logo_width, med_logo_height, med_logo_data);
  
  printer.feed(5);
  // Init serial
  Console.begin(); 
  while (!Console);
  Serial.begin(9600);
  makeNoise();
}

void printerPrint(char* print_buffer) {
  printer.printBitmap(med_logo_width, med_logo_height, med_logo_data);
  printer.feed(2);
  printer.setSize('M');
  printer.println(print_buffer);
  printer.feed(5);
  delay(2000);
}

String hex2char(const byte * data, const uint32_t numBytes)
{
  uint32_t szPos;
  String result;
  result = result + String("  ");
  for (szPos=0; szPos < numBytes; szPos++) 
  {
    if (data[szPos] <= 0x1F)
      result = result + String("");
    else
      result = result + String((char)data[szPos]);
  }
  result = result + String("");
  return result;
}


void checkforNFC(){
    // nfc.read is a blocking function.. adding timeout
    int msgSize = nfc.read(ndefBuf, sizeof(ndefBuf), 20000);
    if (msgSize > 0) {
        makeNoise();
        NdefMessage msg  = NdefMessage(ndefBuf, msgSize);
        msg.print();
        NdefRecord record = msg.getRecord(0);
        record.print();
        int recordLength = record.getPayloadLength();
        if (recordLength <= sizeof(recordBuf)) {
            record.getPayload(recordBuf);
            printerPrint((char*)recordBuf);
            
        }
    }
    return;
}

void checkRegularCard() {
  if (nfc_adapter.tagPresent()) // Do an NFC scan to see if an NFC tag is present
    {
        int i = 0;
        makeNoise();
        NfcTag tag = nfc_adapter.read(); // read the NFC tag into an object, nfc.read() returns an NfcTag object.
        NdefMessage msg = tag.getNdefMessage();
        for (i=0; i<msg.getRecordCount(); i++)
        {
          NdefRecord tmp = msg.getRecord(i);
          int record_length;
          record_length = tmp.getPayloadLength();
          byte storage[record_length];
          tmp.getPayload(storage);
          String respBuffer;
          respBuffer = hex2char(storage, record_length);
          char charBuf[100];
          respBuffer.toCharArray(charBuf, 100);
          printerPrint(charBuf);
        }
        
    }
    delay(500); // wait half a second (500ms) before scanning again (you may increment or decrement the wait time)
    return;
}


String read_proxy_response()
{
  String response = "";

  while(Console.available()>0) {
    char c = Console.read();
    response += c;
  }
  return response;
}


void checkWifi()
{
  String response = read_proxy_response();
  char charBuf[50];
  if (response.length() > 0)
  {
    response.toCharArray(charBuf, 50);
    printerPrint(charBuf);
  }
}

void loop() {
  checkforNFC();
  checkWifi();
  checkRegularCard();
}
