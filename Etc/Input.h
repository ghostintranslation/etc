#ifndef Input_h
#define Input_h

#include "IO.h"
#include "InputsManager.h"

// Forward declaration
class Input;

using InputCallback = void (*)(Input* input);

/**
   Physical input object
   0 inputs
   3 outputs: raw, trigger, gate
*/
class Input: public IO
{
  public:
    Input(byte index);
    void init();
    virtual void update(void) override;
    void setOnChange(InputCallback changeCallback);
    void setOnGateOpen(InputCallback gateOpenCallback);
    void setOnGateClose(InputCallback gateCloseCallback);

  private:
    // The number of samples the trigger has been on
    byte triggerOnSamples = 0;

    // Callbacks
    InputCallback changeCallback = nullptr;
    InputCallback gateOpenCallback = nullptr;
    InputCallback gateCloseCallback = nullptr;
};

inline Input::Input(byte index): IO(index, 0, NULL) {
  // TODO: Move code of the init in the constructor so it executes only once?
  InputsManager::getInstance()->init();

}

inline void Input::update(void) {
  IO::update();
  
  audio_block_t *block, *triggerBlock, *gateBlock;

  // allocate the audio blocks to transmit
  block = allocate();
  triggerBlock = allocate();
  gateBlock = allocate();

  if (block == NULL || triggerBlock == NULL || gateBlock == NULL) {
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

  if (inputBuffer != NULL) {
    unsigned int avg = 0;

    for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      //      Serial.println(inputBuffer[i]);

      // Raw output
      block->data[i] = inputBuffer[i];

      // Trigger output
      if (inputBuffer[i] > INT16_MAX / 2 && triggerOnSamples == 0) {
        triggerOnSamples++;
      }

      if (triggerOnSamples > 0 && triggerOnSamples < 128) {
        triggerBlock->data[i] = INT16_MAX;
      } else {
        triggerBlock->data[i] = 0;
      }

      if (triggerOnSamples >= 128) {
        triggerOnSamples = 0;
      }

      // Gate output
      gateBlock->data[i] = inputBuffer[i] > INT16_MAX / 2 ? INT16_MAX : 0; // 32767 / 2

      // Aproximated moving average
      this->target -= this->target / AUDIO_BLOCK_SAMPLES;
      this->target += block->data[i] / AUDIO_BLOCK_SAMPLES;
    }

    // Triggering Gate
    if (this->gateOpenCallback != nullptr) {
      if (this->target > INT16_MAX / 2 && this->prevValue < INT16_MAX / 2) {
        this->gateOpenCallback(this);
      } else if (this->target < INT16_MAX / 2 && this->prevValue > INT16_MAX / 2) {
        this->gateCloseCallback(this);
      }
    }

    // Triggering Change
    if (this->changeCallback != nullptr) {
      if (this->value != this->prevValue) {
        this->changeCallback(this);
      }
    }

    this->prevValue = this->value;

    transmit(block, 0);
    transmit(triggerBlock, 1);
    transmit(gateBlock, 2);
  }

  release(block);
  release(triggerBlock);
  release(gateBlock);
}

/**
   Set the callback function to call when the value changes
*/
inline void Input::setOnChange(InputCallback changeCallback)
{
  this->changeCallback = changeCallback;
}

/**

*/
inline void Input::setOnGateOpen(InputCallback gateOpenCallback)
{
  this->gateOpenCallback = gateOpenCallback;
}

/**

*/
inline void Input::setOnGateClose(InputCallback gateCloseCallback)
{
  this->gateCloseCallback = gateCloseCallback;
}
#endif
