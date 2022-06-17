#ifndef Input_h
#define Input_h

#include "AudioStream.h"
#include "InputsManager.h"

class Input: public AudioStream
{
  public:
    Input(byte index);
    void init();
    void update(void);
    unsigned int getValue();
    
  private:
    byte index = 0;
    unsigned int value = 0;
};

inline Input::Input(byte index): AudioStream(0, NULL){
  this->index = index;
  if(this->index > 8){
    this->index = 8;
  }

  InputsManager::getInstance()->init();

//  this->active = true;
}

inline void Input::update(void){
  audio_block_t *block;

  // allocate the audio blocks to transmit
  block = allocate(); 
  if (block == NULL){
    return;
  }

//  volatile audio_block* blockData = InputsManager::getInstance()->getData(this->index);
//  
//  for(int i=blockData->readIndex; i < blockData->count; i++){ 
//    block->data[i] = blockData->data[i];
//    blockData->readIndex++;
////    Serial.println(blockData->data[i]);
//  }

//  int16_t* inputBuffer = InputsManager::getInstance()->getBuffers(this->index)->read();

  int16_t* inputBuffer = InputsManager::getInstance()->readInput(this->index);
  if(inputBuffer != NULL){
    unsigned int avg = 0;

    for(unsigned int i=0; i<AUDIO_BLOCK_SAMPLES; i++){
//      Serial.println(inputBuffer[i]);
      block->data[i] = inputBuffer[i];
      avg+=block->data[i];
    }
    this->value = avg/AUDIO_BLOCK_SAMPLES;
    transmit(block);
  }
  
  release(block);
}

inline unsigned int Input::getValue(){
  return this->value;
}
#endif