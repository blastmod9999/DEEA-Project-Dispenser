#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h> 

#define OLED_MOSI     23
#define OLED_CLK      18
#define OLED_DC       2
#define OLED_CS       5
#define OLED_RESET    4
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);

#define RXp2 16
#define TXp2 17

#define CLK 25
#define DT 26
#define SW 27

#define PIN_TEMP           35 
#define PIN_PAHAR_1_ANALOG 34 
#define PIN_PAHAR_2_DIGIT  13 
#define PIN_PAHAR_SW3      32 
#define PIN_PAHAR_SW4      33 

#define PIN_VIBRATIE       14 

int mlSelectati = 50; 
int currentStateCLK;
int lastStateCLK;

void verificaSiguranta();

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  
  delay(250); 
  display.begin(0, true); 
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 20); display.println("SISTEM PORNIT"); display.display();
  delay(1000);

  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  
  pinMode(PIN_PAHAR_1_ANALOG, INPUT);       
  pinMode(PIN_PAHAR_2_DIGIT, INPUT_PULLUP); 
  pinMode(PIN_PAHAR_SW3, INPUT_PULLUP);     
  pinMode(PIN_PAHAR_SW4, INPUT_PULLUP);     

  pinMode(PIN_VIBRATIE, INPUT);

  lastStateCLK = digitalRead(CLK);

  runCalibrationSequence();

  display.clearDisplay();
  display.setCursor(10, 20); display.println("MA ODIHNESC.... :)"); display.display();
  Serial2.write('H'); 
  delay(3000);
}

void loop() {
  verificaSiguranta();

  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    if (digitalRead(DT) != currentStateCLK) mlSelectati += 50; 
    else mlSelectati -= 50;
    
    if(mlSelectati < 50) mlSelectati = 50;
    if(mlSelectati > 300) mlSelectati = 300;
  }
  lastStateCLK = currentStateCLK;

  float tempC = citesteTemperatura();

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0); display.println("ALEGE CANTITATEA:");
  
  display.setTextSize(3);
  display.setCursor(20, 20); 
  display.print(mlSelectati); 
  display.setTextSize(1); 
  display.print(" ml");
  
  display.drawLine(0, 50, 128, 50, SH110X_WHITE);
  
  display.setCursor(0, 55); 
  display.print("Temp: "); 
  display.print(tempC, 1); 
  display.print(" C");

  display.display();

  if (digitalRead(SW) == LOW) {
    delay(300); 
    pornesteCicluServire(mlSelectati);
  }
}

void verificaSiguranta() {
  if (digitalRead(PIN_VIBRATIE) == HIGH) {
    
    Serial2.write('O'); 

    display.clearDisplay();
    display.setTextSize(2); 
    display.setCursor(10, 10); display.println("ALERTA!");
    display.setTextSize(1);
    display.setCursor(10, 35); display.println("IMPACT DETECTAT");
    display.setCursor(10, 50); display.println("SISTEM OPRIT");
    display.display();

    for(int i=0; i<6; i++) {
       display.invertDisplay(true);  
       delay(100);
       display.invertDisplay(false); 
       delay(100);
       Serial2.write('O'); 
    }

    while(1) {
       Serial2.write('O'); 
       delay(1000); 
    }
  }
}

float citesteTemperatura() {
  int raw = analogRead(PIN_TEMP);
  float celsius = 99.0 - (0.02 * raw);
  return celsius;
}

void pornesteCicluServire(int ml) {
  int timpPompare = ml * 160; 

  verificaSiServeste(1, timpPompare);
  verificaSiServeste(2, timpPompare);
  verificaSiServeste(3, timpPompare);
  verificaSiServeste(4, timpPompare);

  display.clearDisplay();
  display.setCursor(10, 20); display.println("NOROC !"); 
  display.display();
  Serial2.write('H'); 
  delay(3000);
}

void verificaSiServeste(int numarPahar, int durata) {
  verificaSiguranta();

  bool existaPahar = false;

  if (numarPahar == 1) {
    if (analogRead(PIN_PAHAR_1_ANALOG) < 2000) existaPahar = true;
  } 
  else if (numarPahar == 2) {
    if (digitalRead(PIN_PAHAR_2_DIGIT) == LOW) existaPahar = true;
  }
  else if (numarPahar == 3) {
    if (digitalRead(PIN_PAHAR_SW3) == LOW) existaPahar = true;
  }
  else if (numarPahar == 4) {
    if (digitalRead(PIN_PAHAR_SW4) == LOW) existaPahar = true;
  }

  if (existaPahar) {
    display.clearDisplay();
    display.setCursor(0, 20);
    display.print("Torn in paharul : "); display.println(numarPahar);
    display.display();

    Serial2.write(numarPahar + '0'); 
    
    for(int i=0; i<30; i++) { 
       delay(100); 
       verificaSiguranta(); 
    }

    Serial2.write('P'); 
    
    unsigned long startTurnare = millis();
    while(millis() - startTurnare < durata) {
       verificaSiguranta(); 
       delay(10);
    }
    
    Serial2.write('O'); 
    delay(500); 
  }
}

void runCalibrationSequence() {
  calibreazaPunct("POZITIE START", 'Z');
  calibreazaPunct("PAHAR 1", 'A');
  calibreazaPunct("PAHAR 2", 'B');
  calibreazaPunct("PAHAR 3", 'C');
  calibreazaPunct("PAHAR 4", 'D');
  display.clearDisplay(); display.setCursor(10, 20); display.println("SPOR LA SERVIT!"); display.display(); delay(1000);
}

void calibreazaPunct(String nume, char cmd) {
  bool gata = false;
  while (!gata) {
    display.clearDisplay();
    display.setCursor(0,0); display.println("CALIBRARE:");
    display.setCursor(0,20); display.println(nume);
    display.setCursor(0,50); display.println("<- ROTESTE ->  OK=Save");
    display.display();
    
    currentStateCLK = digitalRead(CLK);
    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
      if (digitalRead(DT) != currentStateCLK) Serial2.write('>'); 
      else Serial2.write('<');
    }
    lastStateCLK = currentStateCLK;

    if (digitalRead(SW) == LOW) {
      delay(300); Serial2.write(cmd); gata = true;
    }
  }
}