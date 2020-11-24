#include <Adafruit_NeoPixel.h>


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

int cols[] = {4, 17, 13, 12, 11, 10,  9,  8,  7,  5}; // 10 in Y dir
int rows[] = {30, A0,  2,  3, A5, A4, A3, A2, A1}; // 9 in X dir


const int rowCount = sizeof(rows) / sizeof(rows[0]);
const int colCount = sizeof(cols) / sizeof(cols[0]);

byte keys[YCOUNT][XCOUNT];


byte lastkeys[YCOUNT][XCOUNT];



void SendNoteOn(byte channel, byte pitch, byte velocity)
{
  MIDIEvent noteOn = {0x09, 0x90 | channel, pitch, velocity};
  HandleEvents();
  MIDIUSB.write(noteOn);
  MIDIUSB.flush();
  HandleEvents();
}

void SendNoteOff(byte channel, byte pitch, byte velocity)
{
  // LedState[pitch % 100] = 0;
  MIDIEvent noteOff = {0x08, 0x80 | channel, pitch, velocity};
  HandleEvents();
  MIDIUSB.write(noteOff);
  MIDIUSB.flush();
  HandleEvents();
}

void SendControlChange(byte channel, byte control, byte value) {
  MIDIEvent event = {0x0B, 0xB0 | channel, control, value};
  HandleEvents();
    MIDIUSB.write(event);
  MIDIUSB.flush();
  HandleEvents();
}



void noteOn(int channel, int note, int velocity)
{
  int col = 0;
  int row = 0;

  int y = note / 16;
  int x = note % 16;
  if (x < 8 && y < 8)
  {
    col = x;
    row = y + 2;
  }
  else
  {
    if (x == 8)
    {
      col = 8;
      row = y + 2;
    }

  }

  SetLed(col, row, velocity);
}

void noteOff(int channel, int note, int velocity)
{
  noteOn(channel, note, 0);
}

void SetLed(unsigned int x, unsigned int y, int value)
{
  if (x < XCOUNT && y < YCOUNT) {
    LedState[InOrder[x + y * XCOUNT]] = value;
  }
}

void controlChange(int channel, int cc, int value)
{
  if (cc >= 104)
  {
    int col = cc - 104;
    if (col < 8)
    {

      SetLed(col, 0, value);
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
  int cur = 0 ;
  for (int y = 0; y < YCOUNT; y++)
  {
    for (int x = 0; x < XCOUNT; x++)
    {
      InOrder[x + y * XCOUNT] = (((y % 2) == 1) ? (x) : (8 - x)) + y * 9;
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
      HandleEvents();
      int x = rowIndex;
      byte rowCol = rows[rowIndex];
      pinMode(rowCol, INPUT_PULLUP);
      byte b  = digitalRead(rowCol);;
      if (b != keys[y][x] )
      {

       
        keys[y][x] = b;
        int sy = y;
        if (b == LOW)
        {
          int sy = y;
          if (sy == 0 )
          {
            SendControlChange(0, 104 + x, 127);
          }
          if (sy >= 2)
          {
            sy -= 2;
            SendNoteOn(0, sy * 16 + x , 127);
          }
        }
        else
        {
          if (sy == 0 )
          {
            SendControlChange(0, 104 + x, 0);
          }
          if (sy >= 2)
          {
            sy -= 2;
            SendNoteOff(0, sy * 16 + x , 0);
          }
        }
      }



      pinMode(rowCol, INPUT);
    }
    pinMode(curCol, INPUT_PULLUP);
    digitalWrite(curCol, HIGH);
  }
}

int lasttime = millis();
void DirectProcess(MIDIEvent rx)
{
      switch (rx.type)
    {
      case 0:
        break;
      case 0x9:
        noteOn(
          rx.m1 & 0xF,  //channel
          rx.m2,        //pitch
          rx.m3         //velocity
        );
        break;

      case 0x8:
        noteOff(
          rx.m1 & 0xF,  //channel
          rx.m2,        //pitch
          rx.m3         //velocity
        );
        break;

      case 0xB:
        controlChange(
          rx.m1 & 0xF,  //channel
          rx.m2,        //control
          rx.m3         //value
        );
        break;
    }
}

void HandleEvents()
{
  return;
  while (MIDIUSB.available())
  {
    MIDIEvent rx;
    rx = MIDIUSB.read();
  DirectProcess(rx);

  }
}

void loop()
{
  readMatrix();
  if (millis() - lasttime > 30)
  {
    for (int i = 0 ; i < 100; i++)
    {
      uint32_t ledstate = LedState[i];
      uint32_t red = (ledstate & 0x3) << 6;
      uint32_t green = ((ledstate >> 4) & 0x3) << 6;
      red = (red * 63)>>8;
      green = (green *63)>>8;
      uint32_t C = green << 16;
      C += green << 8;
      C += red;

      //      C += 3;
      HandleEvents();
      strip.setPixelColor(i, C); // ((red << 6) << 16) + ((green << 4) << 8) + 0x3);

    }
   // UENUM = 2;
   // UECONX = (1<<EPEN) | (1<<STALLRQ);
    strip.show();
   // UECONX = (1<<EPEN) | (1<<STALLRQC);
    lasttime = millis();
  }
  HandleEvents();
}

