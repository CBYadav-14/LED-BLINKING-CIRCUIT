# LED-BLINKING-CIRCUIT
This project demonstrates a real-time reactive system that uses multiple sensors to monitor environmental conditions and provide alerts via LEDs, a buzzer, and a 16x2 LCD display. The main focus is on condition-based LED blinking that reflects the severity or urgency of sensor readings.

## 🔧 Features

- Motion detection using PIR sensor
- Distance sensing using ultrasonic sensor (HC-SR04)
- Gas leak detection with MQ-2/MQ-135 gas sensor
- Temperature monitoring using an analog sensor (NTC or LM35)
- Ambient light detection via LDR
- Condition-based LED blinking rates for alerts
- 16x2 LCD for rotating sensor info and urgent warnings
- Buzzer for proximity alerts

## 🧰 Components Used

- Arduino UNO
- PIR Motion Sensor (D2)
- Ultrasonic Sensor HC-SR04 (TRIG: D7, ECHO: D8)
- Gas Sensor (A2)
- Temperature Sensor (A4)
- LDR (Photoresistor, A5)
- Buzzer (D5)
- LEDs (PIR: D4, TEMP: A0, DIST: A3, GAS: D3, LIGHT: A1)
- 16x2 LCD Display (Pins 13, 12, 11, 10, 9, 6)
- Breadboard, Jumper wires, 220-ohm resistors

Output:
 1) When there is motion, the RED LED blinks.
    ![PIR output](https://github.com/user-attachments/assets/a83919b4-e26f-4ae9-b740-35ef2a3ca1c3)
2) When an object is near the ultrasonic sensor, and warns if too close by blinking the YELLOW LED.
   ![ultrsonic output](https://github.com/user-attachments/assets/402c36b5-b9a2-47bc-9de8-5d3746e15d01)
3)When gas is detected by the gas sensor system warns by blinking BLUE LED while displaying GAS LEAKED!
 ![gas output](https://github.com/user-attachments/assets/38b46f87-0d55-4a21-9c59-cefaf3fb7fa2)
