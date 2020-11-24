#include <Adafruit_NeoPixel.h>
#include "MIDIUSB.h"


int InOrder[] =
{
  0, 1, 2, 3, 4, 5, 6, 7, 8,
  17, 16, 15, 14, 13, 12, 11, 10, 9,
  18, 19, 20, 21, 22, 23, 24, 25, 26,
  35, 34, 33, 32, 31, 30, 29, 28, 27,
  36, 37, 38, 39, 40, 41, 42, 43, 44,
  53, 52, 51, 50, 49, 48, 47, 46, 45,
  54, 55, 56, 57, 58, 59, 60, 61, 62,
  71, 70, 69, 68, 67, 66, 65, 64, 63,
  72, 73, 74, 75, 76, 77, 78, 79, 80,
  89, 88, 87, 86, 85, 84, 83, 82, 81,
  90, 91, 92, 93, 94, 95, 96, 97, 98,
  99, 100
};

#define XCOUNT 9
#define YCOUNT 10

int neoPixelPin = 6;
int numPixels = 100;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);
int LedState[100] = {0};

int cols[] = {4,17, 13, 12, 11, 10,  9,  8,  7,  5};  // 10 in Y dir
int rows[] = {30, A0,  2,  3, A5, A4, A3, A2, A1}; // 9 in X dir


const int rowCount = sizeof(rows) / sizeof(rows[0]);
const int colCount = sizeof(cols) / sizeof(cols[0]);

byte keys[YCOUNT][XCOUNT];

short midinote[YCOUNT][XCOUNT] =
{
  {104, 105, 106, 107, 108, 109, 110, 111, -1},
  { -1, -1, -1, -1, -1, -1, -1, -1, -1},
  {16, 17, 18, 19, 20, 21, 22, 23, 24},
  {32, 33, 34, 35, 36, 37, 38, 39, 40},
  {48, 49, 50, 51, 52, 53, 54, 55, 56},
  {64, 65, 66, 67, 68, 69, 70, 71, 72},
  {80, 81, 82, 83, 84, 85, 86, 87, 88},
  {96, 97, 98, 99, 100, 101, 102, 103, 104},
  {112, 113, 114, 115, 116, 117, 118, 119, 120},
};


byte lastkeys[YCOUNT][XCOUNT];



void SendNoteOn(byte channel, byte pitch, byte velocity)
{
  // LedState[pitch % 100] = 0xff;
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void SendNoteOff(byte channel, byte pitch, byte velocity)
{
  // LedState[pitch % 100] = 0;
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}

void SendControlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}



void noteOn(int channel, int note, int velocity)
{
  int col = 0;
  int row = 0;
  if (note >= 104)
  {
    col = note - 104;
    row = 0;
  }
  else
  {
    int y = note / 16;
    int x = note % 16;
    if (x < 8 && y < 8)
    {
      col = x;
      row = y+2;
    }
    else
    {
      if (x == 8)
      {
        col = 8;
        row = y + 2;
      }
    }
  }

  uint16_t idx = row * 9 + col;
  if (idx < 100)
  {
    LedState[InOrder[idx]] = velocity;
  }
}

void noteOff(int channel, int note, int velocity)
{
  noteOn(channel, note, 0);
}

void controlChange(int channel, int cc, int value)
{
  if (cc >= 104)
  {
    int col = cc - 104;
    if (col < 8)
    {
      LedState[InOrder[col]] = value;
    }
  }
}

void setup()
{
  strip.begin();
  strip.show();

  for (int i = 0 ; i < colCount; i++)
  {
    pinMode(cols[i], INPUT_PULLUP);
  }
  for (int i = 0 ; i < rowCount; i++)
  {
    pinMode(rows[i], INPUT);
  }
  int cur =0 ;
  for (int y= 0;y<YCOUNT;y++)
  {
    for(int x = 0;x<XCOUNT;x++)
    {
      InOrder[x + y *XCOUNT] = (((y%2)==1)?(x):(8-x))+y*9;
    }
  }
}


void readMatrix()
{

  for (int colIndex = 0; colIndex < colCount; colIndex++)
  {
    int y = colIndex;
    byte curCol = cols[colIndex];
    pinMode(curCol, OUTPUT);
    digitalWrite(curCol, LOW);
    uint32_t current = millis();
    for (int rowIndex = 0; rowIndex < rowCount; rowIndex++)
    {
      int x = rowIndex;
      byte rowCol = rows[rowIndex];
      pinMode(rowCol, INPUT_PULLUP);
      byte b  = digitalRead(rowCol);;
      if (b != keys[y][x] )
      {

        if (0)
        { Serial.print(colIndex);

          Serial.print(" ");
          Serial.println(rowIndex);
        }
        keys[y][x] = b;
        if (b == LOW)
        {
     
          LedState[InOrder[x + y * 9]] = 3;
          if (midinote[y][x] > -1)
          {
            SendNoteOn(0, midinote[y][x] , 127);
          }
        }
        else
        {
          LedState[InOrder[x + y * 9]] = 0;
          if (midinote[y][x] > -1)
          {
            SendNoteOff(0, midinote[y][x] , 0);
          }

        }
      }

      else
      {
        if (b == LOW)
        {
         
        }
      }

      pinMode(rowCol, INPUT);
    }
    pinMode(curCol, INPUT_PULLUP);
    digitalWrite(curCol, HIGH);
  }
}

int lasttime = millis();
void loop()
{
  readMatrix();
  if (millis() - lasttime > 30)
  {
    for (int i = 0 ; i < 100; i++)
    {
      uint32_t ledstate = LedState[i];
      uint32_t red = ledstate & 0x3;
      uint32_t green = (ledstate >> 4) & 0x3;
uint32_t C = red;
C<<=18;
C+= (green<<4)<<8;
C+= 3;
      strip.setPixelColor(i,C);// ((red << 6) << 16) + ((green << 4) << 8) + 0x3);

    }
  //  UECONX = (1<<STALLRQ) | (1<<EPEN);
    strip.show();
    //  UECONX = (1<<STALLRQC) | (1<<EPEN);
  lasttime = millis();
  }
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
  } while (MidiUSB.available()>0);
  MidiUSB.flush();
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
