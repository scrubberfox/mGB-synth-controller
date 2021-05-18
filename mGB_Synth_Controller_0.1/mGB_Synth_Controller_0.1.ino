////////////MIDI GAMEBOY CONTROLLER - mGB - /////////////////////////////////////////////
////////////CODED BY SCRUBBER FOX using MIDI control surface library from PIETER P //////
///////////download library from https://github.com/tttapa/Control-Surface ///////////////
#include <Control_Surface.h>
#include <Selectors/IncrementDecrementSelector.hpp>
HardwareSerialMIDI_Interface midi = Serial;
MIDI_PipeFactory<3> pipes;

//FIVE BANKS WITH ONE TRACK PER BANK
Bank<5> bank(1);
//

IncrementDecrementSelectorLEDs<5> selector = {
  bank, //Bank to manage
  {2, 3}, //push button pins (increment,decrement)
  {4, 5, 6, 7, 8}, // LED pins, one for each channel/bank
  Wrap::Wrap, //wrap around
};
//Pots 

Bankable::CCPotentiometer Potentiometer1 = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A1, //analog pin
  {1, CHANNEL_1},//address
};
Bankable::CCPotentiometer Potentiometer2  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A2, //analog pin
  {2, CHANNEL_1},//address
};
Bankable::CCPotentiometer Potentiometer3  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A3, //analog pin
  {3, CHANNEL_1},//address
};

Bankable::CCPotentiometer Potentiometer4  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A4, //analog pin
  {4, CHANNEL_1},//address
};
Bankable::CCPotentiometer Potentiometer5  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A5, //analog pin
  {5, CHANNEL_1},//address
};

Bankable::CCPotentiometer Potentiometer6  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A6, //analog pin
  {10, CHANNEL_1},//address
};
Bankable::CCPotentiometer Potentiometer7  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A7, //analog pin
  {64, CHANNEL_1},//address
};  
  
  Bankable::PBPotentiometer Potentiometer8  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A0, //analog pin
  {CHANNEL_1},//address
};

int midiByte = 0 ;
void setup() {
  Control_Surface.begin(); // Initialize Control Surface
   Control_Surface >> pipes >> midi; // Connect Control Surface output to the MIDI interface
  Control_Surface << pipes << midi; // Connect Control Surface input to MIDI interface
  midi >> pipes >> midi;            // Route input from the MIDI interface back to its output
  Serial.begin(31250) ;

  }
 
void loop() {
  Control_Surface.loop(); // Update the Control Surface   
    //  if (MIDI.read())
//    MIDI.send(MIDI.getType(),
///              MIDI.getData1(),
//              MIDI.getData2(),
//              MIDI.getChannel());
if (Serial.available() > 0) {
    midiByte = Serial.read() ;
    Serial.write(midiByte) ;
}
}
