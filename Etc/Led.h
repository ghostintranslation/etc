#ifndef Led_h
#define Led_h

// TODO: THE MANAGER update WILL TRANSMIT VALUE OVER SPI

#include "IO.h"

class Led: public IO
{
  public:
    Led(byte index);
    void update(void);
    void setStatus();
    void setIntensity(uint16_t intensity);
    uint16_t getIntensity();

  private:
    audio_block_t *inputQueueArray[1];
    uint16_t target;
    uint16_t value;
    uint16_t intensity;

};

inline Led::Led(byte index): IO(index, 1, inputQueueArray) {
}


inline void Led::update(void) {
  IO::update();

  // TODO: USE THE STATUS TO BE ABLE TO BLINK WITH RECEIVED VALUE

  // Receive input data
  audio_block_t *block;
  block = receiveReadOnly(0);
  if (!block) return;

  uint16_t i, x;
  for (i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
    // bring int16_t data into uint16_t range
    x += block->data[i] + 32768;

    // Aproximated moving average
    this->target -= this->value / AUDIO_BLOCK_SAMPLES;
    this->target += x / AUDIO_BLOCK_SAMPLES;
  }

  release(block);
}

inline uint16_t Led::getIntensity() {
  return this->intensity;
}

inline void Led::setIntensity(uint16_t intensity) {
  this->intensity = intensity;
}

inline void Led::setStatus() {
  // TODO: SET THE STATUS
  // IN THE MANAGER, IF this->active == false THEN this->target will be either this->intensity or 0
}
#endif
