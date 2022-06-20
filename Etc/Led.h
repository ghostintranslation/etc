#ifndef Led_h
#define Led_h

#include "IO.h"
#include "Registrar.h"

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
    void update(void) override;
    void set(Led::Status status, uint16_t brightness);
    void setStatus(Led::Status status);
    void setBrightness(uint16_t brightness);

  private:
    audio_block_t *inputQueueArray[1];
    uint16_t target = 0;
    uint16_t value = 0;
    uint16_t brightness = ABSOLUTE_ANALOG_MAX;

    Status status = Off;

    // Time counter for the blinking
    elapsedMillis blickClock;

};

inline Led::Led(byte index): IO(index, 1, inputQueueArray) {
}

inline void Led::update(void) {
  IO::update();

  // Receive input data
  audio_block_t *block;
  block = receiveReadOnly(0);
  
  if (block){
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
  }

  // Setting target according to status and brightness
  switch (this->status)
  {
  case Blink:
    if (this->blickClock % 400 < 200)
    {
      this->setTarget(0);
    }
    else
    {
      this->setTarget(this->brightness);
    }
    break;
  case BlinkFast:
    if (this->blickClock % 200 < 100)
    {
      this->setTarget(0);
    }
    else
    {
      this->setTarget(this->brightness);
    }
    break;
  case BlinkOnce:
    if (this->blickClock > 100)
    {
      this->setTarget(0);
    }else{
      this->setTarget(this->brightness);
    }
    break;

  default:
      this->setTarget(this->brightness);
    break;
  }
}

inline void Led::set(Led::Status status, uint16_t brightness) {
  this->setStatus(status);
  this->setBrightness(brightness);
}

inline void Led::setStatus(Led::Status status) {
  this->status = status;

  switch (this->status)
  {
  case Off:
    this->brightness = 0;
    break;
  case BlinkOnce:
    this->blickClock = 0;
    break;
  default:
    break;
  }
}

inline void Led::setBrightness(uint16_t brightness) {
  // Limiting the brightness value
  if(brightness > ABSOLUTE_ANALOG_MAX){
    brightness = ABSOLUTE_ANALOG_MAX;
  }

  this->brightness = brightness;
}

#endif
