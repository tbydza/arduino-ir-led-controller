#include <IRremote.h>

const int RECV_PIN = 11;
const int LED_PIN = 5;
const int STEP_SIZE = 15;
const unsigned long BUTTON_UP_ID = 0xFF02FD;
const unsigned long BUTTON_DOWN_ID = 0xFF9867;
const unsigned long REPEAT_ID = 0xFFFFFFFF;
const unsigned long BUTTON_POWER_ID = 0xFFA25D;
const byte UP_LIMIT = 255;
const byte LOW_LIMIT = 15;
const byte LIGHT_OFF = 0;
const byte DEFAULT_BRIGHTNESS = 120;

IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long oldResult = 0xFFFFFFFF;
unsigned long newResult;

bool isPowered = true;
byte brightness = DEFAULT_BRIGHTNESS;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(LED_PIN, OUTPUT);
}

void decreaseBrightness()
{
  if ((brightness > LOW_LIMIT) && isPowered)
  {
    brightness -= STEP_SIZE;
  }
}
void increaseBrightness()
{
  if ((brightness < UP_LIMIT) && isPowered)
  {
    brightness += STEP_SIZE;
  }
}
void switchPower()
{
  if (isPowered)
  {
    isPowered = false;
    brightness = LIGHT_OFF;
  }
  else
  {
    isPowered = true;
    brightness = DEFAULT_BRIGHTNESS;
  }
}
void decide(unsigned long result)
{
  switch (result)
  {
    case BUTTON_UP_ID:
      increaseBrightness();
      break;
    case BUTTON_DOWN_ID:
      decreaseBrightness();
      break;
    case BUTTON_POWER_ID:
      switchPower();
      break;
    default:
      break;
  }
}
void loop()
{
  if (irrecv.decode(&results))
  {
    //Serial.println(results.value, HEX);
    //debug info

    newResult = results.value;
    if (newResult != REPEAT_ID)
    {
      decide(newResult);
      oldResult = newResult;
    }
    else
    {
      if (oldResult != BUTTON_POWER_ID)
      {
        decide(oldResult);
      }
    }
    irrecv.resume(); // Receive the next value
  }


  //Serial.println(brightness);

  analogWrite(LED_PIN, brightness);

}
