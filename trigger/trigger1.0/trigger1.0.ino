#include <Bridge.h>
#include <Process.h>


void setup() {
  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  Serial.begin(9600);

  delay(1000);
  digitalWrite(13, LOW);

  setP2();
}


void loop() {
  int value = analogRead(A0);
  float voltage = value * 0.00568;

  Serial.println(voltage);

  if (voltage < 0.5) {
    Process p; // Create a process "p"
    p.runShellCommandAsynchronously("nc 10.10.91.91 5025");  //10.10.1.62
    p.println("triggered");
    
    digitalWrite(13, HIGH);

    delay(20); //ms
    p.close();
    digitalWrite(13, LOW);
  }
}


void setP2( ) {
  // ADC prescaler = 2
  ADCSRA &=  ~(1 << ADPS2);  // 0
  ADCSRA &=  ~(1 << ADPS1);  // 0
  ADCSRA |=  (1 << ADPS0);  // 1
}
