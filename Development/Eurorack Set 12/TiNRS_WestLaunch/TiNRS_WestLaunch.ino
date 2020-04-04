#include <Adafruit_NeoPixel.h>
#include "MIDIUSB.h"

int neoPixelPin = 6;
int numPixels = 100;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

int rows[] =  {2, 3, 4, 5, 6,7,8,9,10,11};
int cols[] = {14, 8, 9, 10, 16,7,15,A0, A1};
const int rowCount = sizeof(rows) / sizeof(rows[0]);
const int colCount = sizeof(cols) / sizeof(cols[0]);

byte keys[colCount][rowCount];

uint32_t timepressed[colCount][rowCount] =
{
 {0,0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0,0},
};

uint32_t repeats[colCount][rowCount] =
{
 {0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
};


int dorepeat[colCount][rowCount] =
{
 {1,1,1,1,1},
{1,1,1,1,0},
{1,1,0,0,1},
{1,1,0,1,1},
{1,1,1,0,0},
{1,1,0,1,1},
{1,1,1,1,1},
{0,0,0,1,1},
{1,1,0,1,0},
};


int midinote[colCount][rowCount] =
{
 {0,1,2,3,4},
{5,6,7,8,-1},
{9,10,-1,-1,11},
{12,13,-1,14,15},
{16,17,18,-1,-1},
{19,20,-2,21,22},
{23,24,25,-1,-1},
{26,27,28,-1,-1},
{-1,-1,-1,-1,-1},
};


byte lastkeys[colCount][rowCount];





void SendNoteOn(byte channel, byte pitch, byte velocity)
{
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void SendNoteOff(byte channel, byte pitch, byte velocity)
{
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void SendControlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}


int LedState[100] = {0};
int LedColor[100] = {0};

void noteOn(int channel, int note, int velocity)
{
  int col = 0;
  int row = 0;
  if (note >= 104)
  {
    col = note - 104;
    row = 1;
  }
  else
  {
    int upper = note / 16;
    int lower = note % 16;
    if (upper < 8 && lower < 8)
    {
      col = lower;
      row = upper;
    }
    else
    {
      if (lower == 8)
      {
        col = 8;
        row = upper + 2;
      }
    }
  }

  uint16_t idx = row * 9 + col;
  if (idx < 100)
  {
    LedState[idx] = velocity;
  }
}

void noteOff(int channel, int note, int velocity)
{

}

void controlChange(int channel, int cc, int value)
{

}

void setup()
{
  strip.begin();
  strip.show();
}


void readMatrix() 
{
  
  for (int colIndex = 0; colIndex < colCount; colIndex++) 
  {
    byte curCol = cols[colIndex];
    pinMode(curCol, OUTPUT);
    digitalWrite(curCol, LOW);
    uint32_t current = millis();
    for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) 
    {
      byte rowCol = rows[rowIndex];
      pinMode(rowCol, INPUT_PULLUP);
      byte b  = digitalRead(rowCol);;
      if (b != keys[colIndex][rowIndex] )
      {
        
        if (0)
        {Serial.print(colIndex);
        
        Serial.print(" ");
        Serial.println(rowIndex);
        }
        keys[colIndex][rowIndex] = b;
        if (b == LOW)
        {
          timepressed[colIndex][rowIndex] = current;
          repeats[colIndex][rowIndex] = 0;
          if (midinote[colIndex][rowIndex] > -1)
          {
            SendNoteOn(0,midinote[colIndex][rowIndex]+36,127);
          }
        }
       else
       {
          if (midinote[colIndex][rowIndex] > -1)
          {
            SendNoteOff(0,midinote[colIndex][rowIndex]+36,127);
          }
         
       }
      }

      else
      {
        if (b == LOW)
        {
          if (dorepeat[colIndex][rowIndex] > 0 &&  current - timepressed[colIndex][rowIndex]> 50 )
          {
            timepressed[colIndex][rowIndex] = current;
            repeats[colIndex][rowIndex]++;
            if (repeats[colIndex][rowIndex] > 4)
            {
            }
          }
        }
      }

      pinMode(rowCol, INPUT);
    }
    pinMode(curCol, INPUT_PULLUP);
    digitalWrite(curCol, HIGH);
  }
}


void loop()
{
  readMatrix();
  for (int i = 0 ; i < 100; i++)
  {
    int ledstate = LedState[i];
    int red = ledstate & 0x3;
    int green = (ledstate >> 4) & 0x3;

    strip.setPixelColor(i, ((red << 4) << 16) + ((green << 4) << 8)+0x3);

  }
  strip.show();
  midiEventPacket_t rx;
  do
  {
    rx = MidiUSB.read();
    switch (rx.header)
    {
      case 0:
        break;
      case 0x9:
        noteOn(
          rx.byte1 & 0xF,  //channel
          rx.byte2,        //pitch
          rx.byte3         //velocity
        );
        break;

      case 0x8:
        noteOff(
          rx.byte1 & 0xF,  //channel
          rx.byte2,        //pitch
          rx.byte3         //velocity
        );
        break;

      case 0xB:
        controlChange(
          rx.byte1 & 0xF,  //channel
          rx.byte2,        //control
          rx.byte3         //value
        );
        break;
    }
  } while (rx.header != 0);

  
}

/*

   void LaunchpadDevice::recvMidi(const MidiMessage &message) {
    if (message.isNoteOn() || message.isNoteOff()) {
        int index = message.note();
        int row = index / 16;
        int col = index % 16;
        if (row < Rows && col < Cols) {
            setButtonState(row, col, message.velocity() != 0);
        } else if (row < Rows && col == Cols) {
            setButtonState(SceneRow, row, message.velocity() != 0);
        }
    } else if (message.isControlChange()) {
        int index = message.controlNumber();
        if (index >= 104 && index < 112) {
            setButtonState(FunctionRow, index - 104, message.controlValue() != 0);
        }
    }
  }
*/
