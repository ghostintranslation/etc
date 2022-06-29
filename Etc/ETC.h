#ifndef ETC_h
#define ETC_h

#include "Input.h"
#include "Led.h"
#include "LedManager.h"

#define ABSOLUTE_ANALOG_MIN -32768
#define ABSOLUTE_ANALOG_MAX 32767

class ETC
{
  public:
    static ETC *getInstance();
    static void update();

  private:
    // Singleton
    static ETC *instance;
    ETC();

    IntervalTimer updateTimer;
};

// Singleton pre init
ETC * ETC::instance = nullptr;

inline ETC::ETC(){
  updateTimer.begin(ETC::update, 4000);
  updateTimer.priority(255);
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
  noInterrupts();
  LedManager::getInstance()->update();
  interrupts();
}


ETC &ETC = *ETC::getInstance();

#endif
