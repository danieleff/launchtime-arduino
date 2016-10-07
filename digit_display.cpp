#include <Arduino.h>
#include "digit_display.h"
#include "settings.h"

// https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf

const byte OP_DECODEMODE = 9;
const byte OP_INTENSITY = 10;
const byte OP_SCANLIMIT = 11;
const byte OP_SHUTDOWN = 12;
const byte OP_DISPLAYTEST = 15;

const static byte charTable [] PROGMEM  = {
    B00000000,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,//   0
    B01111111,B01111011,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,//   8
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,//  16
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,//  24
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,//  32  !"#$%&'
    B00000000,B00000000,B00000000,B00000000,B10000000,B00000001,B10000000,B00000000,//  40 ()*+,-./
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,//  48 01234567
    B01111111,B01111011,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,//  56 89:;<=>?
    B00000000,B01110111,B01111111,B01001110,B00111101,B01001111,B01000111,B01011110,//  64 @ABCDEFG
    B00110111,B00000110,B00111100,B01010111,B00001110,B01010100,B01110110,B01111110,//  72 HIJKLMNO
    B01100111,B01101011,B01100110,B01011011,B00001111,B00111110,B00111110,B00101010,//  80 PQRSTUVW
    B00110111,B00111011,B01101101,B00000000,B00000000,B00000000,B00000000,B00001000,//  88 XYZ[\]^_
    B00000000,B01111101,B00011111,B00001101,B00111101,B01101111,B01000111,B01111011,//  96 'abcdefg
    B00010111,B00000100,B00011000,B01010111,B00000110,B00010100,B00010101,B00011101,// 104 hijklmno
    B01100111,B01110011,B00000101,B01011011,B00001111,B00011100,B00011100,B00010100,// 112 pqrstuvw
    B00110111,B00111011,B01101101,B00000000,B00000000,B00000000,B00000000,B00000000 // 120 xyz{|}~
};

void DigitDisplay::setup(byte din, byte cs, byte clk) {
  this->din = din;
  this->cs = cs;
  this->clk = clk;

  pinMode(din, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  reset();
}

void DigitDisplay::reset() {
  
  send(OP_DISPLAYTEST, 0); //displaytest
  send(OP_SCANLIMIT, 7); //scanlimit
  
  send(OP_INTENSITY, settings.intensity); //intensity
  send(OP_DECODEMODE, 0);
  
  send(OP_SHUTDOWN, 1); //shutdown false
}

void DigitDisplay::setIntensity(byte intensity) {
  send(OP_INTENSITY, intensity & 0x0F);
}

void DigitDisplay::showDigit(int position, byte digit, boolean dot) {
  showChar(position, digit + '0', dot);
}

void DigitDisplay::showChar(int position, char digit, boolean dot) {
  send(position + 1, pgm_read_byte_near(charTable + digit) | (dot ? 0x80 : 0));
}

void DigitDisplay::write(char* string) {
  for(int i = 0; i < 8; i++) {
    uint8_t b = 0;
    
    char chr = string[i];
    if (chr >> 7) {
      chr &= 0x7F;
      b = 0x80;
    }
    b |= pgm_read_byte_near(charTable + chr);
    send(8-i, b);
  }
  
}

void DigitDisplay::write(const __FlashStringHelper *string) {
  PGM_P p = reinterpret_cast<PGM_P>(string);
  for(int i=0;i<8;i++) {
    send(8-i, pgm_read_byte_near(charTable + pgm_read_byte(p++)));
  }
}


void DigitDisplay::loop() {
  
}


void DigitDisplay::send(volatile byte opcode, volatile byte data) {
  digitalWrite(cs,LOW);
  
  shiftOut(din, clk, MSBFIRST, opcode);
  shiftOut(din, clk, MSBFIRST, data);
      
  digitalWrite(cs,HIGH);
}
