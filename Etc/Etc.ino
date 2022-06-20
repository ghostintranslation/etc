#include <Audio.h>
#include <SPI.h>


#define REGISTERS_LATCH_PIN 9
#define SPI_CLOCK_PIN 13
#define SPI_MOSI_PIN 11
// The audio library uses signed int16_t but inputs and Leds can only have positive values
#define ABSOLUTE_ANALOG_MIN 0
#define ABSOLUTE_ANALOG_MAX 32767

#include "Input.h"
#include "Led.h"


//Input* input1; 

Input input1(0);
Led led1(0);

AudioSynthWaveformSineModulated       waveform1;
AudioOutputI2S           i2s1;
AudioControlSGTL5000     sgtl5000_1;     //xy=930,518

void setup() {
  AudioMemory(20);
    
  Serial.begin(9600);
  while (!Serial && millis() < 5000) ;

//  InputsManager::getInstance()->init();

//  input1 = new Input(2);
  
  AudioNoInterrupts();

//  input1.setOnChange(onInputChange);
//  input1.setOnGateOpen(onInputGateOpen);
//  input1.setOnGateClose(onInputGateClose);

  led1.setStatus(Led::Status::BlinkFast);
  led1.setSmoothing(0);
  
  waveform1.frequency(1);
  waveform1.amplitude(1);
//  waveform1.begin(WAVEFORM_TRIANGLE);


  
//  new AudioConnection(input1, 0, i2s1, 0);
//  new AudioConnection(input2, 0, waveform1, 0);
//  new AudioConnection(waveform1, 0, i2s1, 0);
//  new AudioConnection(waveform1, 0, i2s1, 1);

  new AudioConnection(waveform1, 0, led1, 0);

  
  sgtl5000_1.enable();
  sgtl5000_1.volume(1);

  pinMode(REGISTERS_LATCH_PIN, OUTPUT);
  pinMode(SPI_CLOCK_PIN, OUTPUT);
  pinMode(SPI_MOSI_PIN, OUTPUT);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setSCK(SPI_CLOCK_PIN);
    SPI.setMOSI(SPI_MOSI_PIN);
  SPI.begin();
  
  AudioInterrupts();
}

elapsedMillis consoleClock;
byte data595 = 0;
void loop() {

  if(consoleClock >= 20){
//    Serial.println(led1.getTarget());
    consoleClock = 0;
  }
  
  
//  if(consoleClock >= 2){
//    Serial.printf("%d,%d,%d,%d,%d,%d,%d,%d", input1.getValue(), input2.getValue(), input3.getValue(), input4.getValue(), input5.getValue(), input6.getValue(), input7.getValue(), input8.getValue()); 
//    Serial.println("");
//    consoleClock = 0;
//  }
  
//  if(input2.getValue() >= 600){
//    // Set the latch to low (activate the shift registers)
//    digitalWrite(REGISTERS_LATCH_PIN, LOW);
//      
//    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
//    // Send the byte to set the MUX and select the DAC
//    SPI.transfer(1);
//    SPI.endTransaction();
//    
//    // Set the latch to high (shift registers actually set their pins and stop listening)
//    digitalWrite(REGISTERS_LATCH_PIN, HIGH);
//
////    if(data595 == 0){
////      data595 = 1;
////    }else{
////      data595 = 0;
////    }
////    clock595 = 0;
//  }else{
//       // Set the latch to low (activate the shift registers)
//    digitalWrite(REGISTERS_LATCH_PIN, LOW);
//      
//    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
//    // Send the byte to set the MUX and select the DAC
//    SPI.transfer(0);
//    SPI.endTransaction();
//    
//    // Set the latch to high (shift registers actually set their pins and stop listening)
//    digitalWrite(REGISTERS_LATCH_PIN, HIGH); 
//  }
}

void onInputChange(Input* input){
//  Serial.print(input->getTarget());
//  Serial.print(" ");
//  Serial.println(input->getValue());
}

void onInputGateOpen(Input* input){
  Serial.println("Gate open");
}

void onInputGateClose(Input* input){
  Serial.println("Gate close");
}
