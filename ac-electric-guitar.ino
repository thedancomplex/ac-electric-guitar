/* Stereo peak meter example, assumes Audio adapter but just uses terminal so no more parts required.

  This example code is in the public domain
*/
#include "Adafruit_MPR121.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Keypad.h>

const int myInput = AUDIO_INPUT_LINEIN;
// const int myInput = AUDIO_INPUT_MIC;

/* Cap sensor*/
// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();// 0-11 fret
Adafruit_MPR121 cap_2 = Adafruit_MPR121(); // 12-23 fret



/* detune amount */
float DETUNE_K = 1.1;
int DETUNE_FLAG = 0;
float DETUNE_STEP = 0.5;

/* tone mode switch */
int TONE_MODE_I = 0;
int TONE_MODE_MAX = 400;

/* -----------------------------------
    Note definitions start
    ----------------------------------
*/

float MIXER_THROUGH_GAIN = 0.5;
float MIXER_THROUGH_GAIN_S = 1.0;

int note_max = 108;
//int note_current = 58; //440
//int note_center = 58; // 440
int note_current = 46; //440
int note_center = 46; // 440

const int note_fret_0[12] =
{0,   5,   11,   6,    9,   10,   2,   8,   7,   4,   3,   1};
int note_fret_0_i[12] =
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
// half step mapping {0,5,11, 6,9,10,2,8,7,4,3,1};



const int note_fret_1[12] =
{7, 6, 11, 8, 10, 1, 2, 0, 3, 5, 4, 9};

int note_fret_1_i[12] =
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

/* List of all frequencies */
const float notes[108] = {16.35  ,
                          17.32 ,
                          18.35 ,
                          19.45 ,
                          20.6  ,
                          21.83 ,
                          23.12 ,
                          24.5  ,
                          25.96 ,
                          27.5  ,
                          29.14 ,
                          30.87 ,
                          32.7  ,
                          34.65 ,
                          36.71 ,
                          38.89 ,
                          41.2  ,
                          43.65 ,
                          46.25 ,
                          49  ,
                          51.91 ,
                          55  ,
                          58.27 ,
                          61.74 ,
                          65.41 ,
                          69.3  ,
                          73.42 ,
                          77.78 ,
                          82.41 ,
                          87.31 ,
                          92.5  ,
                          98  ,
                          103.83  ,
                          110 ,
                          116.54  ,
                          123.47  ,
                          130.81  ,
                          138.59  ,
                          146.83  ,
                          155.56  ,
                          164.81  ,
                          174.61  ,
                          185 ,
                          196 ,
                          207.65  ,
                          220 ,
                          233.08  ,
                          246.94  ,
                          261.63  ,
                          277.18  ,
                          293.66  ,
                          311.13  ,
                          329.63  ,
                          349.23  ,
                          369.99  ,
                          392 ,
                          415.3 ,
                          440 ,
                          466.16  ,
                          493.88  ,
                          523.25  ,
                          554.37  ,
                          587.33  ,
                          622.25  ,
                          659.25  ,
                          698.46  ,
                          739.99  ,
                          783.99  ,
                          830.61  ,
                          880 ,
                          932.33  ,
                          987.77  ,
                          1046.5  ,
                          1108.73 ,
                          1174.66 ,
                          1244.51 ,
                          1318.51 ,
                          1396.91 ,
                          1479.98 ,
                          1567.98 ,
                          1661.22 ,
                          1760  ,
                          1864.66 ,
                          1975.53 ,
                          2093  ,
                          2217.46 ,
                          2349.32 ,
                          2489.02 ,
                          2637.02 ,
                          2793.83 ,
                          2959.96 ,
                          3135.96 ,
                          3322.44 ,
                          3520  ,
                          3729.31 ,
                          3951.07 ,
                          4186.01 ,
                          4434.92 ,
                          4698.63 ,
                          4978.03 ,
                          5274.04 ,
                          5587.65 ,
                          5919.91 ,
                          6271.93 ,
                          6644.88 ,
                          7040  ,
                          7458.62 ,
                          7902.13
                         };

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
const int ledPin =  13;      // the number of the LED pin

/* -----------------------------------
    Note definitions end
    ----------------------------------
*/

// GUItool: begin automatically generated code
/*
AudioInputI2S            audioInput;     //xy=137,151
AudioSynthWaveform       waveform1;      //xy=321,209
AudioAnalyzePeak         peak_R;         //xy=324,276
AudioAnalyzePeak         peak_L;         //xy=327,36
AudioEffectMultiply      multiply1;      //xy=485,142
AudioOutputI2S           audioOutput;    //xy=638,139
AudioConnection          patchCord1(audioInput, 0, peak_L, 0);
AudioConnection          patchCord2(audioInput, 1, peak_R, 0);
AudioConnection          patchCord3(audioInput, 1, multiply1, 0);
AudioConnection          patchCord4(waveform1, 0, multiply1, 1);
AudioConnection          patchCord5(multiply1, 0, audioOutput, 0);
AudioConnection          patchCord6(multiply1, 0, audioOutput, 1);
*/

// GUItool: begin automatically generated code
/*
AudioInputI2S            audioInput;     //xy=1043,436
AudioAnalyzePeak         peak_R;         //xy=1142,585
AudioAnalyzePeak         peak_L;         //xy=1233,321
AudioMixer4              mixer1;         //xy=1267,445
AudioSynthWaveform       waveform1;      //xy=1327,589
AudioEffectMultiply      multiply1;      //xy=1495,430
AudioOutputI2S           audioOutput;    //xy=1684,427
AudioConnection          patchCord1(audioInput, 0, peak_L, 0);
AudioConnection          patchCord2(audioInput, 0, mixer1, 0);
AudioConnection          patchCord3(audioInput, 1, peak_R, 0);
AudioConnection          patchCord4(audioInput, 1, mixer1, 1);
AudioConnection          patchCord5(mixer1, 0, multiply1, 0);
AudioConnection          patchCord6(waveform1, 0, multiply1, 1);
AudioConnection          patchCord7(multiply1, 0, audioOutput, 0);
AudioConnection          patchCord8(multiply1, 0, audioOutput, 1);
*/
// GUItool: end automatically generated code



// GUItool: begin automatically generated code

// GUItool: begin automatically generated code

// GUItool: begin automatically generated code
AudioInputI2S            audioInput;     //xy=878,376
AudioAnalyzePeak         peak_R;         //xy=1002,651
AudioMixer4              mixer3;         //xy=1042,488
AudioSynthWaveform       waveform5;      //xy=1095,719
AudioSynthWaveform       waveform6;      //xy=1117,765
AudioSynthWaveform       waveform7;      //xy=1129,816
AudioSynthWaveform       waveform8;      //xy=1142,875
AudioEffectMultiply      multiply2;      //xy=1178,411
AudioEffectMultiply      multiply3;      //xy=1183,466
AudioAnalyzePeak         peak_L;         //xy=1233,321
AudioSynthWaveform       waveform1;      //xy=1309,534
AudioMixer4              mixer1;         //xy=1319,394
AudioSynthWaveform       waveform2;      //xy=1326,581
AudioSynthWaveform       waveform3;      //xy=1339,625
AudioSynthWaveform       waveform4;      //xy=1358,691
AudioMixer4              mixer5;         //xy=1362,802
AudioEffectMultiply      multiply1;      //xy=1495,430
AudioMixer4              mixer2;         //xy=1495,595
AudioMixer4              mixer4;         //xy=1649,618
AudioOutputI2S           audioOutput;    //xy=1684,427
AudioConnection          patchCord1(audioInput, 0, peak_L, 0);
AudioConnection          patchCord2(audioInput, 0, mixer1, 0);
AudioConnection          patchCord3(audioInput, 0, multiply2, 0);
AudioConnection          patchCord4(audioInput, 0, multiply3, 0);
AudioConnection          patchCord5(audioInput, 1, peak_R, 0);
AudioConnection          patchCord6(audioInput, 1, mixer1, 1);
AudioConnection          patchCord7(audioInput, 1, multiply2, 1);
AudioConnection          patchCord8(audioInput, 1, mixer3, 0);
AudioConnection          patchCord9(mixer3, 0, multiply3, 1);
AudioConnection          patchCord10(waveform5, 0, mixer5, 0);
AudioConnection          patchCord11(waveform6, 0, mixer5, 1);
AudioConnection          patchCord12(waveform7, 0, mixer5, 2);
AudioConnection          patchCord13(waveform8, 0, mixer5, 3);
AudioConnection          patchCord14(multiply2, 0, mixer1, 2);
AudioConnection          patchCord15(multiply3, 0, mixer1, 3);
AudioConnection          patchCord16(waveform1, 0, mixer2, 0);
AudioConnection          patchCord17(mixer1, 0, multiply1, 0);
AudioConnection          patchCord18(waveform2, 0, mixer2, 1);
AudioConnection          patchCord19(waveform3, 0, mixer2, 2);
AudioConnection          patchCord20(waveform4, 0, mixer2, 3);
AudioConnection          patchCord21(mixer5, 0, mixer4, 1);
AudioConnection          patchCord22(multiply1, 0, audioOutput, 0);
AudioConnection          patchCord23(multiply1, 0, audioOutput, 1);
AudioConnection          patchCord24(mixer2, 0, mixer4, 0);
AudioConnection          patchCord25(mixer4, 0, multiply1, 1);
// GUItool: end automatically generated code

// GUItool: end automatically generated code

// GUItool: end automatically generated code



// GUItool: end automatically generated code

/* Keypad */

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
//byte rowPins[ROWS] = {3, 16, 20, 21}; //connect to the row pinouts of the keypad
//byte colPins[COLS] = {17, 4, 5};//, 2}; //connect to the column pinouts of the keypad
//byte rowPins[ROWS] = {3, 16, 20, 21}; //connect to the row pinouts of the keypad
//byte colPins[COLS] = {17, 4, 5,2};//, 21}; //connect to the column pinouts of the keypad
byte rowPins[ROWS] = {17, 4, 5, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {3, 20, 16}; //, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );







AudioControlSGTL5000 audioShield;





/* mixer choice setting */
int MIXER_NUM = 6;
int MIXER_CURRENT = 2;
int MIXER_L   = 0;
int MIXER_R   = 1;
int MIXER_LR  = 2;
int MIXER_ALL = 3;
int MIXER_INV = 4;
int MIXER_MULT = 5;

/* tone type */
int D_TONE_TYPE_NUM = 5;
int D_TONE_TYPE_CURRENT = 3;
int D_TONE_TYPE_SINE = 0;
int D_TONE_TYPE_SQUARE = 1;
int D_TONE_TYPE_TRIANGLE = 2;
int D_TONE_TYPE_SAWTOOTH = 3;
int D_TONE_TYPE_SINE_TRIANGLE = 4;


  
void setup() {
  AudioMemory(56);
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.5);
  Serial.begin(9600);

  /* Waveform setup */
  waveform1.begin(WAVEFORM_TRIANGLE);
  waveform1.frequency(notes[note_current]);
  waveform1.phase(0.0);
  waveform1.amplitude(1.0);

  /* Waveform setup */
  waveform2.begin(WAVEFORM_SAWTOOTH);
  waveform2.frequency(notes[note_current]);
  waveform2.phase(0.0);
  waveform2.amplitude(1.0);

    /* Waveform setup */
  waveform3.begin(WAVEFORM_SQUARE);
  waveform3.frequency(notes[note_current]);
  waveform3.phase(0.0);
  waveform3.amplitude(1.0);

    /* Waveform setup */
  waveform4.begin(WAVEFORM_SINE);
  waveform4.frequency(notes[note_current]);
  waveform4.phase(0.0);
  waveform4.amplitude(1.0);



  /* Waveform setup */
  waveform5.begin(WAVEFORM_TRIANGLE);
  waveform5.frequency(notes[note_current]);
  waveform5.phase(0.0);
  waveform5.amplitude(1.0);

  /* Waveform setup */
  waveform6.begin(WAVEFORM_SAWTOOTH);
  waveform6.frequency(notes[note_current]);
  waveform6.phase(0.0);
  waveform6.amplitude(1.0);

    /* Waveform setup */
  waveform7.begin(WAVEFORM_SQUARE);
  waveform7.frequency(notes[note_current]);
  waveform7.phase(0.0);
  waveform7.amplitude(1.0);

    /* Waveform setup */
  waveform8.begin(WAVEFORM_SINE);
  waveform8.frequency(notes[note_current]);
  waveform8.phase(0.0);
  waveform8.amplitude(1.0);
  





  

  mixer1.gain(0,MIXER_THROUGH_GAIN);
  mixer1.gain(1,MIXER_THROUGH_GAIN);
  mixer1.gain(2,0.0);
  mixer1.gain(3,0.0);


  /* choose over tone */
  mixer2.gain(0,MIXER_THROUGH_GAIN_S);
  mixer2.gain(1,0.0);
  mixer2.gain(2,0.0);
  mixer2.gain(3,0.0);

  /* choose detuned overtone */
  mixer5.gain(0,MIXER_THROUGH_GAIN_S);
  mixer5.gain(1,0.0);
  mixer5.gain(2,0.0);
  mixer5.gain(3,0.0);

  /* Pick to use detune */
  mixer4.gain(0,MIXER_THROUGH_GAIN);
  mixer4.gain(1,MIXER_THROUGH_GAIN);
  mixer4.gain(2,0.0);
  mixer4.gain(3,0.0);
  setNote(note_current);
  

  /* invert */
  mixer3.gain(0,-MIXER_THROUGH_GAIN);
  


  /* Setup Cap Touch 0 */
  cap.begin(0x5A);

  /* Setup Cap Touch 2 */
  cap_2.begin(0x5C);

  /* Audio Offset */
  /* Offset Setup */
  for (int i = 0; i < 12; i++) {
    note_fret_0_i[note_fret_0[i]] = i;
    note_fret_1_i[note_fret_1[i]] = i + 12;
  }
}

// for best effect make your terminal/monitor a minimum of 62 chars wide and as high as you can.

elapsedMillis fps;
uint8_t cnt = 0;

void switchToneTypeTouch(int des){
  if(1 == des){
    TONE_MODE_I = TONE_MODE_I + 1;
    if(TONE_MODE_I > TONE_MODE_MAX)
    {
      switchToneType();
      TONE_MODE_I = 0;
    }
  }
  else{
    TONE_MODE_I = TONE_MODE_I - 2;
    if(TONE_MODE_I < 0) TONE_MODE_I = 0;
  }

}

void doNote()
{
  bool read_high = false;
  bool read_low = false;
  int is_touched = 0;
  currtouched = cap.touched();
  if (0 == currtouched) read_high = true;
  int the_note = note_center;


  float vol = 0.5;
  int toneModeFlag = 0;
  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ){
      // Switch tone type if top fret is hit
      if(i == 0 )
      {
        switchToneTypeTouch(1);
        toneModeFlag = 1;
      }
    }
    
    if ((currtouched & _BV(i))){ // && !(lasttouched & _BV(i)) ) {
      if (the_note < (note_center + note_fret_0_i[i])) the_note = note_center + note_fret_0_i[i];
      digitalWrite(ledPin, HIGH);
      note_current = the_note;
      is_touched++;
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      digitalWrite(ledPin, LOW);

    }
  }
  if(toneModeFlag == 0) switchToneTypeTouch(0);


  currtouched = cap_2.touched();
  if (0 == currtouched) read_low = true;
  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i))){// && !(lasttouched & _BV(i)) ) {
      if (the_note < (note_center + note_fret_1_i[i])) the_note = note_center + note_fret_1_i[i];
      digitalWrite(ledPin, HIGH);
      note_current = the_note;
      is_touched++;
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      digitalWrite(ledPin, LOW);
    }
  }

  //if((0 == is_touched) & read_high & read_low) note_current = note_center;
  if (read_high & read_low) note_current = note_center;
  audioShield.volume(vol);
  //waveform1.frequency(notes[note_current]);
  setNote(note_current);
  // reset our state
  lasttouched = currtouched;
  return;
}

void toggleDetune()
{
  if(DETUNE_FLAG == 0)
  {
      /* Pick to use detune */
    mixer4.gain(0,MIXER_THROUGH_GAIN_S);
    mixer4.gain(1,0.0);
    mixer4.gain(2,0.0);
    mixer4.gain(3,0.0);
    DETUNE_FLAG = 1;
  }
  else
  {
      /* Pick to use detune */
    mixer4.gain(0,MIXER_THROUGH_GAIN);
    mixer4.gain(1,MIXER_THROUGH_GAIN);
    mixer4.gain(2,0.0);
    mixer4.gain(3,0.0);
    DETUNE_FLAG = 0;
  }
}

void setNote(int note){
  note_current = note;
  waveform1.frequency(notes[note_current]);
  waveform2.frequency(notes[note_current]);
  waveform3.frequency(notes[note_current]);
  waveform4.frequency(notes[note_current]);

  waveform5.frequency(notes[note_current]+DETUNE_K);
  waveform6.frequency(notes[note_current]+DETUNE_K);
  waveform7.frequency(notes[note_current]+DETUNE_K);
  waveform8.frequency(notes[note_current]+DETUNE_K);
}


















void doPrintPeak() {
  if (fps > 24) {
    if (peak_L.available() && peak_R.available()) {
      fps = 0;
      uint8_t leftPeak = peak_L.read() * 30.0;
      uint8_t rightPeak = peak_R.read() * 30.0;

      for (cnt = 0; cnt < 30 - leftPeak; cnt++) {
        Serial.print(" ");
      }
      while (cnt++ < 30) {
        Serial.print("<");
      }
      Serial.print("||");
      for (cnt = 0; cnt < rightPeak; cnt++) {
        Serial.print(">");
      }
      while (cnt++ < 30) {
        Serial.print(" ");
      }
      Serial.println();
    }
  }
}


void switchToneType(){
    D_TONE_TYPE_CURRENT = D_TONE_TYPE_CURRENT+1;
    if(D_TONE_TYPE_CURRENT > D_TONE_TYPE_NUM) D_TONE_TYPE_CURRENT = 0;
    if(D_TONE_TYPE_CURRENT == D_TONE_TYPE_SAWTOOTH){
      mixer2.gain(0,0.0);
      mixer2.gain(1,MIXER_THROUGH_GAIN_S);
      mixer2.gain(2,0.0);
      mixer2.gain(3,0.0);
      
      mixer5.gain(0,0.0);
      mixer5.gain(1,MIXER_THROUGH_GAIN_S);
      mixer5.gain(2,0.0);
      mixer5.gain(3,0.0);
    }
    if(D_TONE_TYPE_CURRENT == D_TONE_TYPE_TRIANGLE){
      mixer2.gain(0,MIXER_THROUGH_GAIN_S);
      mixer2.gain(1,0.0);
      mixer2.gain(2,0.0);
      mixer2.gain(3,0.0);
      
      mixer5.gain(0,MIXER_THROUGH_GAIN_S);
      mixer5.gain(1,0.0);
      mixer5.gain(2,0.0);
      mixer5.gain(3,0.0);
    }
    if(D_TONE_TYPE_CURRENT == D_TONE_TYPE_SQUARE){
      mixer2.gain(0,0.0);
      mixer2.gain(1,0.0);
      mixer2.gain(2,MIXER_THROUGH_GAIN_S);
      mixer2.gain(3,0.0);
      
      mixer5.gain(0,0.0);
      mixer5.gain(1,0.0);
      mixer5.gain(2,MIXER_THROUGH_GAIN_S);
      mixer5.gain(3,0.0);
    }

    if(D_TONE_TYPE_CURRENT == D_TONE_TYPE_SINE){
      mixer2.gain(0,0.0);
      mixer2.gain(1,0.0);
      mixer2.gain(2,0.0);
      mixer2.gain(3,MIXER_THROUGH_GAIN_S);
      
      mixer5.gain(0,0.0);
      mixer5.gain(1,0.0);
      mixer5.gain(2,0.0);
      mixer5.gain(3,MIXER_THROUGH_GAIN_S);
    }

    if(D_TONE_TYPE_CURRENT == D_TONE_TYPE_SINE_TRIANGLE){
      mixer2.gain(0,MIXER_THROUGH_GAIN_S);
      mixer2.gain(1,0.0);
      mixer2.gain(2,0.0);
      mixer2.gain(3,0.0);
      
      mixer5.gain(0,0.0);
      mixer5.gain(1,0.0);
      mixer5.gain(2,0.0);
      mixer5.gain(3,MIXER_THROUGH_GAIN_S);
    }

}

void keyAction(char key)
{
  if ('1' == key) {
    //note_max = 108;
    if (note_center > 0)
    {
      note_center = note_center - 1;
      note_current = note_current - 1;
      //waveform1.frequency(notes[note_current]);
      setNote(note_current);
    }
  }
  if ('2' == key) {
    // return to normal
    int tmp = 46 - note_center;
    note_current = note_current + tmp;
    note_center = 46;
  }
  if ('3' == key) {
    //note_max = 108;
    if (note_center < (108 - 25))
    {
      note_center = note_center + 1;
      note_current = note_current + 1;
      //waveform1.frequency(notes[note_current]);
      setNote(note_current);
    }
  }

  if ('*' == key) {
    DETUNE_K = DETUNE_K - DETUNE_STEP;
    if(DETUNE_K < 0.0) DETUNE_K = 0.0;
    setNote(note_current);
  }

  
  if ('#' == key) {
    DETUNE_K = DETUNE_K + DETUNE_STEP;
    if(DETUNE_K < 0.0) DETUNE_K = 0.0;
    setNote(note_current);
  }


  if('E' == key) {
      MIXER_CURRENT = MIXER_LR;
      mixer1.gain(0,MIXER_THROUGH_GAIN);
      mixer1.gain(1,MIXER_THROUGH_GAIN);
      mixer1.gain(2,0.0);
      mixer1.gain(3,0.0);
    }

  if ('7' == key) {
    // Toggle mixer
    if(MIXER_CURRENT == MIXER_LR){
      MIXER_CURRENT = MIXER_L;
      mixer1.gain(0,0.0);
      mixer1.gain(1,MIXER_THROUGH_GAIN_S);
      mixer1.gain(2,0.0);
      mixer1.gain(3,0.0);
    }
    else{
      MIXER_CURRENT = MIXER_LR;
      mixer1.gain(0,MIXER_THROUGH_GAIN);
      mixer1.gain(1,MIXER_THROUGH_GAIN);
      mixer1.gain(2,0.0);
      mixer1.gain(3,0.0);
    }
  }


  if('C' == key) {
      MIXER_CURRENT = MIXER_L;
      mixer1.gain(0,0.0);
      mixer1.gain(1,MIXER_THROUGH_GAIN_S);
      mixer1.gain(2,0.0);
      mixer1.gain(3,0.0);
    }

    if('A' == key) {
      switchToneType();
    }

   if('0' == key) {
      toggleDetune();
    }

  if('D' == key) {
    MIXER_CURRENT = MIXER_CURRENT+1;
    if(MIXER_CURRENT > MIXER_NUM) MIXER_CURRENT = 0;
    if(MIXER_CURRENT == MIXER_L){
      mixer1.gain(0,MIXER_THROUGH_GAIN_S);
      mixer1.gain(1,0.0);
      mixer1.gain(2,0.0);
      mixer1.gain(3,0.0);
    }
    if(MIXER_CURRENT == MIXER_R){
      mixer1.gain(0,0.0);
      mixer1.gain(1,MIXER_THROUGH_GAIN_S);
      mixer1.gain(2,0.0);
      mixer1.gain(3,0.0);
    }
    if(MIXER_CURRENT == MIXER_LR){
      mixer1.gain(0,MIXER_THROUGH_GAIN);
      mixer1.gain(1,MIXER_THROUGH_GAIN);
      mixer1.gain(2,0.0);
      mixer1.gain(3,0.0);
    }
    if(MIXER_CURRENT == MIXER_ALL){
      mixer1.gain(0,MIXER_THROUGH_GAIN);
      mixer1.gain(1,MIXER_THROUGH_GAIN);
      mixer1.gain(2,MIXER_THROUGH_GAIN);
      mixer1.gain(3,MIXER_THROUGH_GAIN);
    }
    
  }

}

void doPrintKey() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    keyAction(key);
    Serial.println(key);
  }
}
void loop() {
  doNote();
  doPrintPeak();
  doPrintKey();
}
