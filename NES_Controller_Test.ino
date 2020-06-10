/*
  NES Controller Button Reader
  Arduino Uno

  Controller Button States:
  nesRegister [7..0] = [Right, Left, Down, Up, Start, Select, B, A]

  NES controller pinout:
               _________
             /          |
            /        O  | Ground
           /            |
     +VCC  |  O      O  | Clock
           |            |
     N.C.  |  O      O  | Latch
           |            |
     N.C.  |  O      O  | Data Out (To NES)
           |____________|

  Gadget Reboot
*/


// bit positions of each controller button in the status register
const int A_BUTTON         = 0;
const int B_BUTTON         = 1;
const int SELECT_BUTTON    = 2;
const int START_BUTTON     = 3;
const int UP_BUTTON        = 4;
const int DOWN_BUTTON      = 5;
const int LEFT_BUTTON      = 6;
const int RIGHT_BUTTON     = 7;

const int shiftDelay       = 20;    // delay in microseconds to help with shift register setup/hold timing

byte nesRegister           = 255;   // store button states. 0=pressed 1=released  default all released

// NES Pins
const int nesClock          = 2;    // NES controller 4021 clock pin
const int nesLatch          = 3;    // NES controller 4021 latch pin
const int nesData           = 4;    // NES controller 4021 data pin

// led pins
const int ledLeft           = 12;   // test LEDs to show controller button activity
const int ledUp             = 11;
const int ledDn             = 10;
const int ledRight          = 9;
const int ledSelect         = 8;
const int ledStart          = 7;
const int ledB              = 6;
const int ledA              = 5;


void setup()
{
  // configure NES pins
  pinMode(nesData, INPUT);
  pinMode(nesClock, OUTPUT);
  pinMode(nesLatch, OUTPUT);

  digitalWrite(nesClock, LOW);      // NES control lines idle low
  digitalWrite(nesLatch, LOW);

  // configure LED output pins
  for (int i = 5; i < 13; i++) {
    digitalWrite(i, LOW);
    pinMode(i, OUTPUT);
  }
}

void loop()
{
  // read the 8 controller buttons into the nesRegister
  // NES controller button states are loaded into the 4021 while nesLatch is high
  // When nesLatch goes low, the first data bit is shifted to nesData
  // Button data is shifted to nesData on each low to high transition of nesClock

  digitalWrite(nesLatch, HIGH);       // latch button states into shift register
  delayMicroseconds(shiftDelay);      // impose a delay to allow a setup and hold time for any 4021 pins
  digitalWrite(nesLatch, LOW);

  for (int x = 0; x <= 7; x++) {                    // read in the 8 controller buttons that were latched 
    bitWrite(nesRegister, x, digitalRead(nesData)); // store the current button states into the nesRegister
    digitalWrite(nesClock, HIGH);                   // generate a clock pulse 
    delayMicroseconds(shiftDelay);
    digitalWrite(nesClock, LOW);
  }

  // assign the 8 NES controller button states to the test LEDs.
  // 0 = button pressed so set LED port high
  digitalWrite(ledLeft   , !bitRead(nesRegister, LEFT_BUTTON));
  digitalWrite(ledUp     , !bitRead(nesRegister, UP_BUTTON));
  digitalWrite(ledDn     , !bitRead(nesRegister, DOWN_BUTTON));
  digitalWrite(ledRight  , !bitRead(nesRegister, RIGHT_BUTTON));
  digitalWrite(ledSelect , !bitRead(nesRegister, SELECT_BUTTON));
  digitalWrite(ledStart  , !bitRead(nesRegister, START_BUTTON));
  digitalWrite(ledB      , !bitRead(nesRegister, B_BUTTON));
  digitalWrite(ledA      , !bitRead(nesRegister, A_BUTTON));
  
}
