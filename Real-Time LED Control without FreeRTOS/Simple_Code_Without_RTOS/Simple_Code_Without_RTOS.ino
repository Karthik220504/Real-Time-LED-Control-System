#define LED1 4 // Pin for the first LED (Red)
#define LED2 5 // Pin for the second LED (Green)
#define BUTTON1 2 // Pin for the first button
#define BUTTON2 3 // Pin for the second button

// Volatile variables for(ISRs)
volatile bool toggleBlinkRate = false; // Variable to toggle blink rate
volatile bool changeColor = false; // Variable to change LED color

// Variables for time management
unsigned long previousMillis = 0; // Stores the last time the LEDs were updated
unsigned long interval = 1000; // Interval for LED blinking (default is 1 Hz)

// Variable to track the current LED color
bool track = true; // Starts with the red LED active

void setup() {
  // Initialize the LED pins as output
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  // Initialize the button pins as input
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  
  // Attach interrupts to the buttons
  attachInterrupt(digitalPinToInterrupt(BUTTON1), toggleRate, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2), changeLEDColor, FALLING);
}

void loop() {
  // Get the current time
  unsigned long currentMillis = millis();
  
  // Check if it's time to toggle the LEDs
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Update the last time the LEDs were updated
    
    // Toggle the LEDs based on the current color state
    if (track) {
      digitalWrite(LED1, !digitalRead(LED1)); // Toggle the red LED
      digitalWrite(LED2, LOW); // Ensure the green LED is off
    } else {
      digitalWrite(LED2, !digitalRead(LED2)); // Toggle the green LED
      digitalWrite(LED1, LOW); // Ensure the red LED is off
    }
  }
}

// Interrupt Service Routine (ISR) to toggle the blink rate
void toggleRate() {
  toggleBlinkRate = !toggleBlinkRate; // Toggle the blink rate state
  if (toggleBlinkRate) {
    interval = 500; // Set interval to 500ms for 2 Hz blink rate
  } else {
    interval = 1000; // Set interval to 1000ms for 1 Hz blink rate
  }
}

// ISR to change the LED color
void changeLEDColor() {
  changeColor = !changeColor; // Toggle the color change state
  track = !track; // Toggle the active LED color
}
