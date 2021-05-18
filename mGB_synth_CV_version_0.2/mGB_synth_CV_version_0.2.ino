///////////MIDI GAMEBOY CONTROLLER - mGB - /////////////////////////////////////////////
////////////CODED BY SCRUBBER FOX using MIDI control surface library from PIETER P, //////
///////////download library from https://github.com/tttapa/Control-Surface ///////////////
/////// ///////CV section adapted from Tuesday Night Machines/////////////////
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

//pots doubled as CV Analog pins to allow them to change channels

//Pitchbend
  Bankable::PBPotentiometer Potentiometer8  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A0, //analog pin
  {CHANNEL_1},//address
};

//pulse width/shape select
Bankable::CCPotentiometer Potentiometer1 = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A1, //analog pin
  {1, CHANNEL_1},//address
};

//envelope/shape offset
Bankable::CCPotentiometer Potentiometer2  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A2, //analog pin
  {2, CHANNEL_1},//address
 
};

//Pitch sweep
Bankable::CCPotentiometer Potentiometer3  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A3, //analog pin
  {3, CHANNEL_1},//address
};

//Pitchbend range
Bankable::CCPotentiometer Potentiometer4  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A4, //analog pin
  {4, CHANNEL_1},//address
};

//Load preset
Bankable::CCPotentiometer Potentiometer5  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A5, //analog pin
  {5, CHANNEL_1},//address
};

//Pan
Bankable::CCPotentiometer Potentiometer7  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A6, //analog pin
  {10, CHANNEL_1},//address
};

//Sustain
Bankable::CCPotentiometer Potentiometer6  = {
  {bank,BankType::CHANGE_CHANNEL}, //bank configuration
  A7, //analog pin
  {64, CHANNEL_1},//address
};

int midiByte = 0 ;

void setup() {
  //  Set MIDI baud rate:
  Serial.begin(31250);\
  Control_Surface.begin(); // Initialize Control Surface
   Control_Surface >> pipes >> midi; // Connect Control Surface output to the MIDI interface
  Control_Surface << pipes << midi; // Connect Control Surface input to MIDI interface
  midi >> pipes >> midi;            // Route input from the MIDI interface back to its output

  // not sure if I need this for cv:
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(A6, INPUT_PULLUP);
  pinMode(A7, INPUT_PULLUP);
}

const int cvInputs = 8; // how many analog inputs are used for cv
int cvLevels[cvInputs] = {0, 0, 0, 0, 0, 0, 0, 0};//stores current CV level
int analogInputs[cvInputs] = {A0, A1, A2, A3, A4, A5, A6, A7}; // analog inputs
int midiCCnumbers[cvInputs] = {1, 2, 3, 4, 5, 10, 64, 0}; // define CC# for each CV input
int midiCCvalues[cvInputs] = {0, 0, 0, 0, 0, 0, 0, 0};///stores prev sent CC values

void loop() {
  
 
  for (int i = 0; i < cvInputs; i++)
  {
    cvLevels[i] = floor(analogRead(analogInputs[i])/8); // convert 10 bit analog input to 7 bit MIDI range
    
    // if cv has changed, update cv input value and send new midi cc value
    if (cvLevels[i] != midiCCvalues[i])
    {
      midiCCvalues[i] = cvLevels[i];
      sendCC(midiCCnumbers[i], midiCCvalues[i]);
      delay(10); // need to experiment with this delay some more, 50 is probably too long
      ////10ms shouldnt be too noisy for midi input
    }   
   } 
}

void sendCC(uint8_t control, uint8_t value) {
  Serial.write(0xB0 | (0xf));
  Serial.write(control & 0x7f);
  Serial.write(value & 0x7f);
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
