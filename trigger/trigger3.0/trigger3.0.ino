#include <Bridge.h>
#include <Process.h>

volatile boolean triggered = false;

void setup() {
  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  
  Serial.begin(115200);
  Serial.println("Started.");
  //ADCSRB = 0;
  //ACSR = bit (ACI) | bit (ACIE) | bit (ACIS1);
  ACSR |= ((0<< ACD)|   // Enable Analog comparator
           (1<<ACBG)|   // AIN+ set to bandgap reference voltage 1.1(1)V
           (0<< ACO)|   // Analog comparator output-off
           (1<< ACI)|   // Analog comparator interrupt flag: clear pending interrupt
           (1<<ACIE)|   // Analog comparator interrupt is activated
           (0<<ACIC)|   // Analog comparator input capture-disable
           (1<<ACIS1)|(1<<ACIS0)); // Comparator interrupt on falling output edge 
  ADCSRB |= (1<<ACME);  // AIN- set to ADC7
  ADCSRA &= ~(1<<ADEN);
  ADMUX  |= ((1<<MUX2)|(1<<MUX1)|(1<<MUX0));

  delay(1000);
  digitalWrite(13, LOW);

  sei(); // Enable the global interrupts
}

void loop() {
  if(triggered) {
    Process p; // Create a process "p"
    p.runShellCommandAsynchronously("nc 10.10.85.160 5025");  //10.10.91.91
    p.println("triggered");
    Serial.println("Triggered!");
    triggered = false;

    digitalWrite(13, HIGH);
    delay(20);
    p.close();
    digitalWrite(13, LOW);
  }
}

ISR(ANALOG_COMP_vect) {
  if ((ACSR &(1 << ACO))) {
    triggered = true;
    }else{
    triggered = false;
    }
}
