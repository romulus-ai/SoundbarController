#include <Arduino.h>
#include <RCSwitch.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>


#define RC_PIN D5
#define IR_PIN D6

//RC Protocol for Soundbar
#define RC_SOUNDBAR_VOLMINUS 7984233
#define RC_SOUNDBAR_VOLPLUS  7984234
#define RC_SOUNDBAR_PAIR     7984235
#define RC_SOUNDBAR_MUTE     7984236
#define RC_SOUNDBAR_POWER    7984237


//IR-Signale koennen nach dem Einbinden der Bibliotheken ueber passende Beispielprogramme 
//(Datei -> Beispiele -> IRremoteESP8266 -> IRrecvDumpV2) eingelesen/decodiert werden.
//IR-Codes (in Hex) des Ventilators:
#define IR_SOUNDBAR_VOLMINUS 0x807F10EF // Protocol NEC
#define IR_SOUNDBAR_VOLPLUS  0x807FC03F // Protocol NEC
#define IR_SOUNDBAR_PAIR     0x807F906F // Protocol NEC
#define IR_SOUNDBAR_MUTE     0x807FCC33 // Protocol NEC
#define IR_SOUNDBAR_POWER    0x807F807F // Protocol NEC

#define IR_BITS 32 //Anzahl Bits
#define IR_WDH 3  //Anzahl Wiederholungen
//Delay in ms zwischen dem Senden von mehrfachen IR-Befehlen (bei ...plus X / ...minus X).
#define IR_DELAY_MS 100

RCSwitch mySwitch = RCSwitch();
IRsend irsend(IR_PIN);

void setup() {
  Serial.begin(115200);
  mySwitch.enableReceive(RC_PIN);
  Serial.println("[RC] Initialized");


  irsend.begin();
  Serial.println("[IR] Initialized");

}

String uint64ToString(uint64_t input) {
  String result = "";
  uint8_t base = 10;

  do {
    char c = input % base;
    input /= base;

    if (c < 10)
      c +='0';
    else
      c += 'A' - 10;
    result = c + result;
  } while (input);
  return result;
}

void loop() {
  if (mySwitch.available()) {
    int rc_received = mySwitch.getReceivedValue();
    Serial.println("[RC] Received: " + String(rc_received));

    if(rc_received == RC_SOUNDBAR_VOLPLUS) {
      Serial.println("[IR HANDLER] sending signal > vol+ < HEX: " + uint64ToString(IR_SOUNDBAR_VOLPLUS));
      irsend.sendNEC(IR_SOUNDBAR_VOLPLUS, IR_BITS, IR_WDH);
    }
    else if (rc_received == RC_SOUNDBAR_VOLMINUS) {
      Serial.println("[IR HANDLER] sending signal > vol- < HEX: " + uint64ToString(IR_SOUNDBAR_VOLMINUS));
      irsend.sendNEC(IR_SOUNDBAR_VOLMINUS, IR_BITS, IR_WDH);
    }
    else if (rc_received == RC_SOUNDBAR_PAIR) {
      Serial.println("[IR HANDLER] sending signal > pair < HEX: " + uint64ToString(IR_SOUNDBAR_PAIR));
      irsend.sendNEC(IR_SOUNDBAR_PAIR, IR_BITS, IR_WDH);
    }
    else if (rc_received == RC_SOUNDBAR_MUTE) {
      Serial.println("[IR HANDLER] sending signal > mute < HEX: " + uint64ToString(IR_SOUNDBAR_MUTE));
      irsend.sendNEC(IR_SOUNDBAR_MUTE, IR_BITS, IR_WDH);
    }
    else if (rc_received == RC_SOUNDBAR_POWER) {
      Serial.println("[IR HANDLER] sending signal > power < HEX: " + uint64ToString(IR_SOUNDBAR_POWER));
      irsend.sendNEC(IR_SOUNDBAR_POWER, IR_BITS, IR_WDH);
    }

    mySwitch.resetAvailable();
  }
}