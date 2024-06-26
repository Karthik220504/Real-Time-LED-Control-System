#include <Arduino_FreeRTOS.h>
#include <queue.h>

// Pin definitions -> LEDs and buttons
#define red_led 4
#define green_led 5
#define Button1 2
#define Button2 3

// Queue handles for inter-task communication
QueueHandle_t Q_blink;
QueueHandle_t Q_colour;

// Initial blink rate (500 ms) and default LED color (red)
volatile int blinkRate = 500;
bool task = true;

// Function declarations for FreeRTOS tasks and ISRs
void TaskBlink(void *pvParameters);
void TaskHandleButtons(void *pvParameters);
void ISR_button1();
void ISR_button2();

void setup() {
  // Configure LED pins as output
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  
  // Configure button pins as input with internal pull-up resistors
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  
  // Attach interrupts to the buttons, triggering on a falling edge
  attachInterrupt(digitalPinToInterrupt(Button1), ISR_button1, FALLING);
  attachInterrupt(digitalPinToInterrupt(Button2), ISR_button2, FALLING);
  
  // Create queues for blink rate and color change communication
  Q_blink = xQueueCreate(10, sizeof(int));
  Q_colour = xQueueCreate(10, sizeof(bool));
  
  // Create FreeRTOS tasks for LED blinking and button handling
  xTaskCreate(TaskBlink, "Blink", 128, NULL, 1, NULL);
  xTaskCreate(TaskHandleButtons, "HandleButtons", 128, NULL, 2, NULL);
}

void loop() {
  // The main loop is empty because all functionality is managed by FreeRTOS tasks
}

// Task to control the blinking of LEDs
void TaskBlink(void *pvParameters) {
  (void) pvParameters;
  bool currentLEDState = false;  // LED state variable
  unsigned long lastBlinkTime = millis();  // Timestamp of the last blink
  int currentBlinkRate = blinkRate;  // Current blink rate
  
  for (;;) {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= currentBlinkRate) {
      lastBlinkTime = currentTime;
      currentLEDState = !currentLEDState;  // Toggle LED state
      if (task) {
        digitalWrite(red_led, currentLEDState);
        digitalWrite(green_led, LOW);
      } else {
        digitalWrite(red_led, LOW);
        digitalWrite(green_led, currentLEDState);
      }
    }
    
    // Check for new blink rate from the queue
    if (xQueueReceive(Q_blink, &currentBlinkRate, 0) == pdPASS) {
      blinkRate = currentBlinkRate;
    }
    
    // Check for color change request from the queue
    bool changeColor;
    if (xQueueReceive(Q_colour, &changeColor, 0) == pdPASS) {
      task = !task;
    }
    
    vTaskDelay(1);  // Yield to other tasks
  }
}

// Task to handle button debouncing and other logic (currently unused)
void TaskHandleButtons(void *pvParameters) {
  (void) pvParameters;
  
  for (;;) {
    // Button handling logic is done in the ISRs
    vTaskDelay(100);  // Debouncing delay
  }
}

// ISR for Button 1 to toggle the blink rate between 1 Hz and 2 Hz
void ISR_button1() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  
  if (interruptTime - lastInterruptTime > 200) {  // Debounce check
    int newBlinkRate = (blinkRate == 500) ? 250 : 500;
    xQueueSendFromISR(Q_blink, &newBlinkRate, NULL);  // Send new blink rate to the queue
  }
  lastInterruptTime = interruptTime;
}

// ISR for Button 2 to change the LED color
void ISR_button2() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  
  if (interruptTime - lastInterruptTime > 200) {  // Debounce check
    bool changeColor = true;
    xQueueSendFromISR(Q_colour, &changeColor, NULL);  // Send color change request to the queue
  }
  lastInterruptTime = interruptTime;
}
