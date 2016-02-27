/*
Pin identifier for interrupt masking
D0    PCINT16 (PCMSK2 / PCIF2 / PCIE2)
D1    PCINT17 (PCMSK2 / PCIF2 / PCIE2)
D2    PCINT18 (PCMSK2 / PCIF2 / PCIE2)
D3    PCINT19 (PCMSK2 / PCIF2 / PCIE2)
D4    PCINT20 (PCMSK2 / PCIF2 / PCIE2)
D5    PCINT21 (PCMSK2 / PCIF2 / PCIE2)
D6    PCINT22 (PCMSK2 / PCIF2 / PCIE2)
D7    PCINT23 (PCMSK2 / PCIF2 / PCIE2)
D8    PCINT0  (PCMSK0 / PCIF0 / PCIE0)
D9    PCINT1  (PCMSK0 / PCIF0 / PCIE0)
D10   PCINT2  (PCMSK0 / PCIF0 / PCIE0)
D11   PCINT3  (PCMSK0 / PCIF0 / PCIE0)
D12   PCINT4  (PCMSK0 / PCIF0 / PCIE0)
D13   PCINT5  (PCMSK0 / PCIF0 / PCIE0)
A0    PCINT8  (PCMSK1 / PCIF1 / PCIE1)
A1    PCINT9  (PCMSK1 / PCIF1 / PCIE1)
A2    PCINT10 (PCMSK1 / PCIF1 / PCIE1)
A3    PCINT11 (PCMSK1 / PCIF1 / PCIE1)
A4    PCINT12 (PCMSK1 / PCIF1 / PCIE1)
A5    PCINT13 (PCMSK1 / PCIF1 / PCIE1)
*/

// modified on 07-feb-2016

#include <rotary.h>
#include <blaupunktremote.h>
#include <Bounce2.h>

const byte DEBOUNCE_MS = 20;

// interrupt vector 0 - Pins D8 to D13
const byte R1_BUTTON = 4; // black
const byte R1_RIGHT  = 3; // yellow
const byte R1_LEFT   = 2; // yellow

// Interrupt vector 1 - Pins A0 to A5
// This is closer to driver and will control volume
const byte R2_BUTTON  = A0; // yellow
const byte R2_RIGHT   = A1; // red
const byte R2_LEFT    = A2; // purple

Rotary r1Enc = Rotary(R1_RIGHT, R1_LEFT); 
Bounce r1Button = Bounce();  

Rotary r2Enc = Rotary(R2_RIGHT, R2_LEFT); 
Bounce r2Button = Bounce();  

BlaupunktRemote rc10 = BlaupunktRemote(6);

void setup() {

  pinMode(R1_BUTTON, INPUT);
  digitalWrite(R1_BUTTON, HIGH);

  pinMode(R2_BUTTON, INPUT);
  digitalWrite(R2_BUTTON, HIGH);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Serial.begin(9600);

  r1Button.attach(R1_BUTTON);
  r1Button.interval(DEBOUNCE_MS);

  r2Button.attach(R2_BUTTON);
  r2Button.interval(DEBOUNCE_MS);
  
  // enable interrupts
  interrupts();
  Serial.println("READY");
}

void loop() {
  
  if (r1Button.update())
  {   
    if (r1Button.read() == LOW)
    {
      digitalWrite(13, HIGH);
      Serial.write("R1B\n");
    }
    else
    {
      digitalWrite(13, LOW);      
    }
  }

  if (r2Button.update())
  {   
    if (r2Button.read() == LOW)
    {
      digitalWrite(13, HIGH);
      Serial.write("R2B\n");
    }
    else
    {
      digitalWrite(13, LOW);      
    }
  }
  
  unsigned char resultR1 = r1Enc.process();
  
  if (resultR1) {
    if (resultR1 == DIR_CCW) {
      // turned LEFT      
      Serial.write("R1L\n");
    }
    else {
      // turned RIGHT      
      Serial.write("R1R\n");
    }
  } 

  unsigned char resultR2 = r2Enc.process();
  
  if (resultR2) {
    if (resultR2 == DIR_CCW) {
      // turned LEFT      
      rc10.volumeDown();
      //Serial.write("R2L\n");
    }
    else {
      // turned RIGHT
      rc10.volumeUp();      
      //Serial.write("R2R\n");
    }
  }   
}

