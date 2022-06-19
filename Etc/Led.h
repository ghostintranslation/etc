#ifndef Led_h
#define Led_h

#include "Registrar.h"
#include "IO.h"

class Led: public IO, public Registrar<Led>
{
  public:
    enum Status
    {
      Off,
      On,
      Blink,
      BlinkFast,
      BlinkOnce
    };
    
    Led(byte index);
    void update(void);
    void set(Led::Status status, uint16_t brightness);
    void setStatus(Led::Status status);
    void setBrightness(uint16_t brightness);

  private:
    audio_block_t *inputQueueArray[1];
    uint16_t target = 0;
    uint16_t value = 0;
    uint16_t brightness = ABSOLUTE_ANALOG_MAX;

};

inline Led::Led(byte index): IO(index, 1, inputQueueArray) {
}


inline void Led::update(void) {
  IO::update();

  // Receive input data
  audio_block_t *block;
  block = receiveReadOnly(0);
  if (!block) return;

  for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
    int16_t x = block->data[i];
    
    // Cutting off negative values
    if(x < 0){
      x = 0;
    }

    // Aproximated moving average
    this->brightness -= this->brightness / AUDIO_BLOCK_SAMPLES;
    this->brightness += x / AUDIO_BLOCK_SAMPLES;
  }

  release(block);


  // TODO: NOW SET TARGET ACCORDING TO STATUS AND BRIGHTNESS
}

inline void Led::set(Led::Status status, uint16_t brightness) {
  // Limiting the brightness value
  if(brightness > ABSOLUTE_ANALOG_MAX){
    brightness = ABSOLUTE_ANALOG_MAX;
  }

  this->status = status;
  this->brightness = brightness;
}

inline void Led::setStatus(Led::Status status) {
  this->status = status;
}

inline void Led::setBrightness(uint16_t brightness) {
  // Limiting the brightness value
  if(brightness > ABSOLUTE_ANALOG_MAX){
    brightness = ABSOLUTE_ANALOG_MAX;
  }

  this->brightness = brightness;
}

#endif
