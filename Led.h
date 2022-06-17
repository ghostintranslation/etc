#ifndef Led_h
#define Led_h

// TODO: THE MANAGER update WILL set value to reach target and transmit value over SPI


class Led: public AudioStream
{
  public:
    Led(byte index);
    void update(void);
    uint16_t getValue();
  
  private:
    audio_block_t *inputQueueArray[1];
    uint16_t target;  
    uint16_t value;
    uint16_t intensity;

};

inline Led::Led(byte index): AudioStream(1, inputQueueArray){
  this->index = index;
  if(this->index > 8){
    this->index = 8;
  }

  LedsManager::getInstance()->register(this);

//  this->active = true;
}


inline void Led::update(void){
    // TODO: USE THE STATUS TO BE ABLE TO BLINK WITH RECEIVED VALUE

    // Receive input data
    audio_block_t *block;
    block = receiveReadOnly(0);
    if (!block) return;

    uint16_t i, x;
    for (i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        // bring int16_t data into uint16_t range
        x += block->data[i] + 32768;
        
        // Moving average
        this->target -= this->value / AUDIO_BLOCK_SAMPLES;
        this->target += x / AUDIO_BLOCK_SAMPLES;
    }

    release(block);
}

inline uint16_t Led::getValue(){
    return this->value;
}

inline uint16_t Led::getTarget(){
    return this->target;
}

inline void Led::setTarget(uint16_t target){
    this->target = target;
}

inline uint16_t Led::getIntensity(){
    return this->intensity;
}

inline void Led::setIntensity(uint16_t intensity){
    this->intensity = intensity;
}

inline void Led::setStatus(){
    // TODO: SET THE STATUS
    // IN THE MANAGER, IF this->active == false THEN this->target will be either this->intensity or 0
}