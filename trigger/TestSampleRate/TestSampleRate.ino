//////
// speed up sampling rate -- by [email]tsaiwn@cs.nctu.edu.tw[/email]
const int pin = A0;
const int n = 1000;  // sample 採樣 1000 次
void setup() {
  Serial.begin(230400);
  //setP32( ); // Prescaler = 32
  //setP8( );   // Prescaler = 8
  //setP16( );   // Prescaler = 16
  //setP128( ); // Prescaler = 128 = default
  setP4();
  for(int i=0; i< 543; i++) analogRead(A0); 
  Serial.println(String("Sample ") + n + " times, pin=" + pin);
  Serial.flush( );
  delay(568);
}
void loop( ) { // 
  long begt, runt, total;
  total = 0;  // clear before sampling
  begt = micros();
  for(int i=0; i< n; i++) {
     total += analogRead(pin);
  }
  runt = micros() - begt;  // elapsed time
  Serial.println(String("Average=") + total/n);
  Serial.print(String("Time per sample: ")+runt/1.0/n +"us");
  Serial.println(String(", Frequency: ")+1000000.0/runt*n +" Hz");
  delay(5566);
}// loop(
void setP32( ) {
  Serial.println("ADC Prescaler = 32");  // 101
  ADCSRA |=  (1 << ADPS2);  // 1
  ADCSRA &=  ~(1 << ADPS1);  // 0
  ADCSRA |=  (1 << ADPS0);  // 1
}
void setP16( ) {
  Serial.println("ADC Prescaler = 16");  // 100
  ADCSRA |=  (1 << ADPS2);  // 1
  ADCSRA &=  ~(1 << ADPS1);  // 0
  ADCSRA &=  ~(1 << ADPS0);  // 0
}
void setP8( ) {  // prescaler = 8
  Serial.println("ADC Prescaler = 8");  // 011
  ADCSRA &=  ~(1 << ADPS2);  // 0
  ADCSRA |=  (1 << ADPS1);  // 1
  ADCSRA |=  (1 << ADPS0);  // 1
}
void setP4( ) {  // prescaler = 4
  Serial.println("ADC Prescaler = 4");  // 010
  ADCSRA &=  ~(1 << ADPS2);  // 0
  ADCSRA |=  (1 << ADPS1);  // 1
  ADCSRA &=  ~(1 << ADPS0);  // 0
}
void setP128( ) { // default
  Serial.println("ADC Prescaler = 128");  // 111
  ADCSRA |=  (1 << ADPS2);  // 1
  ADCSRA |=  (1 << ADPS1);  // 1
  ADCSRA |=  (1 << ADPS0);  // 1
} // setP128
