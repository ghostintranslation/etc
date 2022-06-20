#ifndef LedManager_h
#define LedManager_h

#include <SPI.h>

#include "Led.h"

class LedManager
{
  public:
    static LedManager *getInstance();
    void update();

  private:
    // Singleton
    static LedManager *instance;
    LedManager();

    // Update clock
    elapsedMillis clockUpdate;
    const unsigned int intervalClockUpdate = 8; // 8ms period = 120Hz

    // PWM clock
    elapsedMillis clockPWM;
    const unsigned int intervalPWM = 16; // 16ms period = 60Hz

    uint8_t ledsData = 0;
    
    void writeLeds();

};

// Singleton pre init
LedManager * LedManager::instance = nullptr;

inline LedManager::LedManager(){
}

/**
 * Singleton instance
 */
inline LedManager *LedManager::getInstance() {
  if (!instance)
     instance = new LedManager;
  return instance;
}

inline void LedManager::update(){
    if(this->clockUpdate > this->intervalClockUpdate){
        this->writeLeds();
        this->clockUpdate = 0;
    }

    if (this->clockPWM > this->intervalPWM)
    {
        this->clockPWM = 0;
    }
}

inline void LedManager::writeLeds()
{
    // Preparing the shift register data
    uint8_t ledsData = 0;

    // If no LED we don't need to send anything
    if(Led::getCount() == 0){
        return;
    }

    // Preparing the LEDs data
    for(uint8_t i = 0; i < Led::getCount(); i++){
        if (Led::get(i)->getValue() == ABSOLUTE_ANALOG_MIN)
        {
            continue;
        }
        else if (Led::get(i)->getValue() == ABSOLUTE_ANALOG_MAX)
        {
            bitSet(ledsData, Led::get(i)->getIndex());
        }
        else if ((float)this->clockPWM / this->intervalPWM < (float)Led::get(i)->getValue() / ABSOLUTE_ANALOG_MAX)
        {
            bitSet(ledsData, Led::get(i)->getIndex());
        }
    }

    // Reduce SPI use by not pushing data if not necessary
    if(this->ledsData == ledsData){
        return;
    }
  
    // Set the latch to low (activate the shift registers)
    digitalWrite(REGISTERS_LATCH_PIN, LOW);
    
    // Send the leds byte
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(ledsData);
    SPI.endTransaction();
    
    // Set the latch to high (shift registers actually set their pins and stop listening)
    digitalWrite(REGISTERS_LATCH_PIN, HIGH);

    this->ledsData = ledsData;
}

#endif
