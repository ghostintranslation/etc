#ifndef ETC_h
#define ETC_h

#define ABSOLUTE_ANALOG_MIN -32768
#define ABSOLUTE_ANALOG_MAX 32767
#define ANALOG_RANGE 65536

#include "Input.h"
#include "MidiManager.h"
#include "Led.h"
#include "LedManager.h"
#include "InputsManager.h"

class ETC
{
  public:
    static ETC *getInstance();
    static void update();

  private:
    // Singleton
    static ETC *instance;
    ETC();

    elapsedMicros updateClock;
    // IntervalTimer updateTimer;
};

// Singleton pre init
ETC * ETC::instance = nullptr;

inline ETC::ETC(){
  // updateTimer.begin(ETC::update, 4000);
  // updateTimer.priority(255);
}

/**
 * Singleton instance
 */
inline ETC *ETC::getInstance() {
  if (!instance)
     instance = new ETC;
  return instance;
}

inline void ETC::update(){
//  InputManager::getInstance()->update();
  if(getInstance()->updateClock >= 22){
    for(Input* input : Input::getAll()){
      input->realTimeUpdate();
    }
    getInstance()->updateClock = 0;
  }
  
  LedManager::getInstance()->update();
}


ETC &ETC = *ETC::getInstance();

#endif
