========================================================
           Real-Time LED Control System Using FreeRTOS
========================================================

Overview
--------
This project uses FreeRTOS on an Arduino Uno to control two LEDs and two push buttons, simulated in Proteus. 
It demonstrates real-time task management and interrupt handling.

Hardware Setup
--------------
Components:
  - Arduino Uno
  - Red LED (connected to pin 4)
  - Green LED (connected to pin 5)
  - Button 1 (connected to pin 2, internal pull-up)
  - Button 2 (connected to pin 3, internal pull-up)
  - Appropriate resistors for LEDs and buttons

Functionality
-------------
1. LED Blinking:
   - Default blink rate: 1 Hz (500 ms on/off).
   - Button 1 toggles blink rate between 1 Hz and 2 Hz (250 ms on/off).

2. LED Color Change:
   - Button 2 toggles the blinking LED color between red and green.

Software Setup
--------------
Development Tools:
  - Arduino IDE for coding and uploading
  - FreeRTOS library for task management
  - Proteus for hardware simulation

Instructions
------------
1. Setup the Simulation:
   - Use Proteus to connect the Arduino, LEDs, and buttons as described.

2. Upload the Code:
   - Open the provided code in Arduino IDE.
   - Upload the code to the Arduino Uno in Proteus.

Code Summary
------------
Tasks:
  - TaskBlink: Controls LED blinking.
  - TaskHandleButtons: Manages button logic (handled mainly in ISRs).

Queues:
  - blinkRateQueue: Communicates blink rate changes.
  - colorChangeQueue: Communicates color change requests.

ISRs:
  - ISR_button1: Toggles blink rate.
  - ISR_button2: Toggles LED color.
