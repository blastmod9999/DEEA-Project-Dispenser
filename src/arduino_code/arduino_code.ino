#include <SoftwareSerial.h>
#include <Stepper.h>

SoftwareSerial espSerial(4, 5);

const int stepsPerRevolution = 2048; 
Stepper myStepper(stepsPerRevolution, 8, 11, 10, 12); 

const int pinPompa = 9;

long currentPosition = 0; 
long posPahar1 = 0;
long posPahar2 = 0;
long posPahar3 = 0;
long posPahar4 = 0;

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  
  pinMode(pinPompa, OUTPUT);
  myStepper.setSpeed(5);
  
  Serial.println("ARDUINO functional.");
}

void loop() {
  if (espSerial.available()) {
    char cmd = espSerial.read();
    
    if (cmd == '>') {
      myStepper.step(10); 
      currentPosition += 10;
    }
    else if (cmd == '<') {
      myStepper.step(-10);
      currentPosition -= 10;
    }
    
    else if (cmd == 'Z') { currentPosition = 0; }
    else if (cmd == 'A') { posPahar1 = currentPosition; }
    else if (cmd == 'B') { posPahar2 = currentPosition; }
    else if (cmd == 'C') { posPahar3 = currentPosition; }
    else if (cmd == 'D') { posPahar4 = currentPosition; }

    else if (cmd == 'H') { mergiLa(0); }
    else if (cmd == '1') { mergiLa(posPahar1); }
    else if (cmd == '2') { mergiLa(posPahar2); }
    else if (cmd == '3') { mergiLa(posPahar3); }
    else if (cmd == '4') { mergiLa(posPahar4); }

    else if (cmd == 'P') { 
      analogWrite(pinPompa, 255);
    }
    else if (cmd == 'O') { 
      analogWrite(pinPompa, 0);
    }
  }
}

void mergiLa(long destinatie) {
  long pasi = destinatie - currentPosition;
  myStepper.step(pasi);
  currentPosition = destinatie;
  espSerial.write('K'); 
}