# Catching the TTL trigger



Our group need to build a new DAQ system for planned nuclear decay experiments, which require a stable and reliable procedure to remote-control all network-connected devices (a spectrum analyzer, a IQ recorder and a server) to successively collecting large amount of data. However, the original trigger system of IQ recorder can not satisfy the requirment. It can only work steadily when executing the _recording mode_ each time the trigger signal comes.

***An independent trigger system to catch the TTL trigger signal is in high demand!***


**Goal**

Catching the TTL trigger signal and sending the *triggered* message to server, so that the server can execute the IQ recorder to collect data.

**material**

 * Arduino Yun: ATmega 32U4 (16MHz) and built-in Ethernet support
 * resistances, PCB, soldering iron (tin)

**Content**
 * designing concept
 * circuit diagram
 * software 1.0: resetting prescaler
 * software 2.0: using analog comparator

## Designing concept

The TTL trigger signal: 
 * falling edge
 * width: 50 μs
 * amplitude range: 0~4V

![image](/Pic/triggerSignal.png)

Design index:
 * sampling frequency > 100 kHz
 * amplitude < 4V, triggered

Process:

1. measuring TTL signal: when amplitude < 4V, triggered
2. netcat: using `nc` ShellCommand to send the message to server


## Circuit diagram

A simple circuit is for measuring the voltage of the resistance (1kΩ), which is connected with another resistance (the same 1kΩ) in series. The total voltage of this circuit is the input voltage between TTL and GND. The voltage between ADC input A0 and GND is what we measure.

![image](/Pic/Circuit.png)

<h2 id="resetPrescaler"> software 1.0: resetting prescaler</h2>

This idea was used in our beam test 2016.

The main idea is to resetting prescaler. The `analogRead()` usually needs 100μs to read the analog input. Since the default prescaler is 128.

ADC clock = 16 MHz / 128 = 125 kHz

One ADC transfer needs 13 ticks. The final sampling rate

125 kHz / 13 = 9600 Hz

***It is far from our design index!***

Since we do not need a very high accuracy of the voltage. We reset the prescaler to instead 4 of 128. The ideal sampling rate is 308 kHz (3.24 μs per point). The test result is 9.42 μs per point.

**Code**
```Arduino
// Define various ADC prescaler
const unsigned char PS_2 = (1 << ADPS0);
const unsigned char PS_4 = (1 << ADPS1);
const unsigned char PS_8 = (1 << ADPS1) | (1 << ADPS0);
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

// To change the prescaler modify setup
ADCSRA |= PS_16;  // 16 prescaler
ADCSRA |= PS_8;   // 8 prescaler
ADCSRA |= PS_4;   // 4 prescaler
```

**Reference link**

[Advanced Arduino ADC - Faster analogRead()](http://www.microsmart.co.za/technical/2014/03/01/advanced-arduino-adc/)

[The Analog to Digital Converter(ADC)](http://www.avrbeginners.net/architecture/adc/adc.html)

**Problem**

The new setting has a high risk of shortening life of the Arduino. Since the suggested prescaler is not least than 16. 

In our one-week test experiment, 2016, the trigger was working fairly fine all the time. Nevertheless, bad luck comes in 2017.


<h2 id="analogComparator"> software 2.0: using analog comparator</h2>

This idea will be used in our beam test 2017.

The main idea is using analog comparator, comparing the input values on the positive pin AIN+ and negative pin AIN-. When the voltage on the AIN+ is higher than the voltage on the AIN-, the Analog Comparator output, ACO, is set.
The comparator can trigger a separate interrupt, exclusive to the Analog Comparator.

In this case, the AIN+ we choose is the Bandgap reference (1.1(1)V) and the AIN- is ADC input A0 (ATmega 32U4 mapping pin ADC7).

**Code**
```Arduino
ACSR |= ((0<<   ACD)|                  // Enable Analog comparator
         (1<<  ACBG)|                  // AIN+ set to bandgap reference voltage 1.1(1)V
         (0<<   ACO)|                  // Analog comparator output-off
         (1<<   ACI)|                  // Analog comparator interrupt flag: clear pending interrupt
         (1<<  ACIE)|                  // Analog Comparator interrupt is activated
         (0<<  ACIC)|                  // Disable analog comparator input capture
         (1<< ACIS1)|(1<<ACIS0));      // Comparator interrupt on riing output edge
ADCSRB |= (1<<ACME);                   // AIN- set to ADC7(A0)
ADCSRA &= ~(1<<ADEN);  
ADMUX |= ((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
DIDR1 |= (1<<AIN0D);                   // disable the digital input buffer

sei();                                 // Enable the global interrupts
```

**Reference link**

[ATmega 32U4 datasheet](http://www.atmel.com/Images/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf)

[Using the Arduino Analog Comparator](http://www.gammon.com.au/forum/?id=11916)

**Result**

No one trigger missing!

The DC Characteristics shows the typical propagation delay is 500 ns at VCC = 4.0V, TA = -40 to 85 Celsius degree.
