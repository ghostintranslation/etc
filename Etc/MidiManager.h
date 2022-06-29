#ifndef MidiManager_h
#define MidiManager_h

#include <vector>
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI); // MIDI library init

using MidiControlChangeCallbackFunction = void (*)(uint16_t);

struct MidiControlChangeCallback{
  byte control;
  MidiControlChangeCallbackFunction callback = nullptr;
};

class MidiManager
{
  public:
    static MidiManager *getInstance();
    void addControlChangeCallback(ControlChangeCallback fptr, )
    void update();

  private:
    // Singleton
    static MidiManager *instance;
    MidiManager();

    std::vector<MidiControlChangeCallback> midiControlChangeCallbacks;

    void handleMidiControlChange(byte channel, byte control, byte value);
};

// Singleton pre init
MidiManager * MidiManager::instance = nullptr;

inline MidiManager::MidiManager(){

  // MIDI init
  MIDI.setHandleControlChange(getInstance()->handleMidiControlChange);
  MIDI.begin();
  usbMIDI.setHandleControlChange(getInstance()->handleMidiControlChange);

  Serial1.begin(31250, SERIAL_8N1_RXINV);
}

/**
 * Singleton instance
 */
inline MidiManager *MidiManager::getInstance() {
  if (!instance)
     instance = new MidiManager;
  return instance;
}

inline void MidiManager::addControlChangeCallback(MidiControlChangeCallbackFunction fptr, byte control){
  this->midiControlChangeCallbacks.push_back({
    control: control,
    callback: fptr
  });
}

inline void MidiManager::removeControlChangeCallback(MidiControlChangeCallbackFunction fptr, byte control){
  this->midiControlChangeCallbacks.erase(std::remove(this->midiControlChangeCallbacks.begin(), this->midiControlChangeCallbacks.end(), {control: control, callback: fptr}), this->midiControlChangeCallbacks.end());
}

inline bool MidiManager::isExistControlChangeCallback(MidiControlChangeCallbackFunction fptr, byte control){
  if ( std::find(this->midiControlChangeCallbacks.begin(), this->midiControlChangeCallbacks.end(), {control: control, callback: fptr}) != this->midiControlChangeCallbacks.end() ){
    return true;
  }

  return false;
}

inline void MidiManager::update(){
  MIDI.read();
  usbMIDI.read();
}

inline void handleMidiControlChange(byte channel, byte control, byte value){
  // This value is converted from the MIDI range of unsigned 8 bits to a signed 16 bits range to match
  // the audio library's blocks samples range. So a 0 value is now -32768.
  int16_t newValue = value * 256 - 32768;

  for(MidiControlChangeCallback i : this->midiControlChangeCallbacks){
    if(i.control == control && i.callback != nullptr){
      i.callback(newValue);
    }
  }
}

#endif