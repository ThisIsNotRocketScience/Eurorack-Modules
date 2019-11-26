//Button
const int pushButton = 2;
int buttonState;

int loopCounter = 0;

//Prints
float analogInput;
float voltInput;
int midiOutput;
float voltOutput;

float outputRange = 5.0f;
float quantSteps = 12.0f;

bool btnC, btnCis, btnD, btnDis, btnE, btnF, btnFis, btnG, btnGis, btnA, btnAis, btnB = 0;
int voltInt;

void setup() {
  Serial.begin(115200);
  pinMode(pushButton, INPUT_PULLUP);
}

float Quantize(float inp)
{
  voltInput = inp / 1024.0f * outputRange; // 0 - 1024 -> 0.0f - 1.0f -> 0.0f - 5.0f  
  midiOutput = round(voltInput * quantSteps); 
  voltOutput = midiOutput / quantSteps;

  voltInt = midiOutput * 100 / quantSteps;
}

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
    Serial.print(voltOutput, 32);
    Serial.print("\tInteger: ");
    Serial.println(voltInt);
  }
  
  if(buttonState == 1) {
    loopCounter = 0;
  }
}
