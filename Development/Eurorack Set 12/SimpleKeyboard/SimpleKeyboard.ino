#include <Keyboard.h>
#include <Adafruit_NeoPixel.h>

// the data pin for the NeoPixels
int neoPixelPin = A2;

// How many NeoPixels we will be using, charge accordingly
int numPixels = 12;

// Instatiate the NeoPixel from the ibrary
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

int rows[] =  {2, 3, 4, 5, 6};
//             1  2  3   4   5  6   7   8   9
int cols[] = {14, 8, 9, 10, 16, 7, 15, A0, A1};

const int rowCount = sizeof(rows) / sizeof(rows[0]);
const int colCount = sizeof(cols) / sizeof(cols[0]);

byte keys[colCount][rowCount];
byte sendkeys[colCount][rowCount] = 
{
  {'z','s','x','d','c'},
  {'v','g','b','h',KEY_F2},
  {'n','j',KEY_F3,KEY_F4,'m'},
  {'q','2',
  
  '1',
  
  'w','3'},
  {'e','r','5',KEY_F5,KEY_F6},
  {'t','6',KEY_F8,'y','7'},
  {'u','i','9','/','*'},
  {'o','0','p',KEY_LEFT_ARROW,'.'},
  {KEY_DOWN_ARROW,KEY_UP_ARROW,KEY_INSERT,KEY_RIGHT_ARROW,KEY_DELETE},
  }
;
uint32_t timepressed[colCount][rowCount] =
{
 {0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
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






void setup() {
  strip.begin();
  // make pin 2 an input and turn on the
  // pullup resistor so it goes high unless
  // connected to ground:
  for (int x = 0; x < rowCount; x++)
  {
    pinMode(rows[x], INPUT);
  }

  for (int x = 0; x < colCount; x++)
  {
    pinMode(cols[x], INPUT_PULLUP);
  }

  Keyboard.begin();
  
  Serial.begin(31250);
  Serial1.begin(31250);
}


void NoteOn(int idx)
{
  Serial1.write(0x90);
  Serial1.write(idx);
  Serial1.write(127);  
};

void NoteOff(int idx)
{
  Serial1.write(0x80);
  Serial1.write(idx);
  Serial1.write(127);  
};

void Panic()
{
  for (int i =0 ;i<128;i++)
  {
    NoteOff(i);
  }
}
void WinKey()
{
   
   Keyboard.print("!");;
        
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
          Keyboard.press(sendkeys[colIndex][rowIndex]);
          timepressed[colIndex][rowIndex] = current;
          repeats[colIndex][rowIndex] = 0;
          if (midinote[colIndex][rowIndex] > -1)
          {
            NoteOn(midinote[colIndex][rowIndex]+36);
          }
        }
       else
       {
        Keyboard.release(sendkeys[colIndex][rowIndex]);
          if (midinote[colIndex][rowIndex] > -1)
          {
            NoteOff(midinote[colIndex][rowIndex]+36);
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
            Keyboard.release(sendkeys[colIndex][rowIndex]);        
            Keyboard.press(sendkeys[colIndex][rowIndex]);
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

void printMatrix() {
  for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
    if (rowIndex < 10)
      Serial.print(F("0"));
    Serial.print(rowIndex); Serial.print(F(": "));

    for (int colIndex = 0; colIndex < colCount; colIndex++) {
      Serial.print(keys[colIndex][rowIndex]);
      if (colIndex < colCount)
        Serial.print(F(", "));
    }
    Serial.println("");
  }
  Serial.println("");
}

void loop() {
  //if the button is pressed
  for (int i = 0; i < numPixels; i++)
  {
    uint32_t c = 0x100010;
    for (int j = 0;j<9;j++)
    {
      if (keys[i%9][j%5]==LOW)
      {
        c+= 0x101010;
      }
    }
    strip.setPixelColor(i, c);
  }
  strip.show();
  //Serial.println("..");
  readMatrix();
  //delay(10);
}
