#ifndef MidiManager_h
#define MidiManager_h

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
  for(MidiControlChangeCallback i : this->midiControlChangeCallbacks){
    if(i.control == control && i.callback != nullptr){
      i.callback(value);
    }
  }
}

#endif