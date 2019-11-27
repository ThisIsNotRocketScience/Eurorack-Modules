const int pushButton = 2;

int buttonState;
int loopCounter = 0;
float analogInput;
float voltInput;
int midiOutput;
float voltOutput;

void setup() {
  Serial.begin(115200);
  pinMode(pushButton, INPUT_PULLUP);
}

float Quantize(float inp)
{
  voltInput = inp / 1024.0f * 5.0f; // 0 - 1024 -> 0.0f - 1.0f -> 0.0f - 5.0f  
  midiOutput = round(voltInput * 12); 
  voltOutput = midiOutput / 12.0f;
}


#define DEBOUNCE 4

class Quantizer
{
  int Result;
  int LastToggle;
  int ToggleSame ;
  int LastInput;
  
public:  
  
  Quantizer()
  {
    Result = 0;
    LastToggle = 1;
    ToggleSame = 0;
    LastInput = 0;
  }
  
  void FeedValue(int V)
  {
    LastInput = Result;
  }
  
  void Trigger()
  {
    Result = Quantize(LastInput);
  }

  void Release()
  {
    // nothing here for now..
  }
  
  void SetToggle(int T)
  {
    if (LastToggle != T)
    {
      LastToggle = T;
      ToggleSame = 0;
    }
    else
    {

      if (ToggleSame == DEBOUNCE)
      {
        if (T == 0) Trigger();else Release();
      }
      else
      {
         ToggleSame++;
      }
    }
  }

  int GetResult()
  {
    return Result;    
  }
  
};

Quantizer Channels[4];

void loop() {

  buttonState = digitalRead(pushButton);
  analogInput = analogRead(A0);

  

  
  if(buttonState == 0 && loopCounter < 1) {
    loopCounter++;

    Quantize(analogInput);
    
    Serial.print("Input: ");
    Serial.print(analogInput);
    Serial.print("\tScaled: ");
    Serial.print(voltInput);
    Serial.print("\tMidinote: ");
    Serial.print(midiOutput);
    Serial.print("\tQuantized: ");
    Serial.println(voltOutput);
  }
  
  if(buttonState == 1) {
    loopCounter = 0;
  }
}
