// ------------------------------------------------------------
// Project: Simple LED State Machine (Full Control Version)
// Board: Arduino Nano 33 BLE Sense
// Author: Lana Popoola
// ------------------------------------------------------------
// Description:
//   - Implements a simple state machine using RGB LED (pins 22, 23, 24)
//   - Switch on D4 toggles between states
//   - Each state explicitly sets LED pins HIGH/LOW
//   - Ensures no mixed colors remain ON
// ------------------------------------------------------------

#define LED_R 22   // Red pin
#define LED_G 23   // Green pin
#define LED_B 24   // Blue pin
#define SW_PIN 4   // Push button (external)

enum State { DARK, RED, BLUE, GREEN };
State currentState = DARK;

unsigned long stateStart = 0;
unsigned long lastButtonTime = 0;
const unsigned long debounceDelay = 200; // debounce time (ms)

// Helper: Read switch press with debounce
bool isButtonPressed() {
  if (digitalRead(SW_PIN) == LOW) {  // Active LOW (pull-up used)
    if (millis() - lastButtonTime > debounceDelay) {
      lastButtonTime = millis();
      return true;
    }
  }
  return false;
}

// Helper: Turn off all LEDs
void allOff() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, HIGH);
}

// Helper: Set specific color
void showRed() {
  digitalWrite(LED_R, LOW);// LOW = ON
  digitalWrite(LED_G, HIGH); // HIGH = OFF
  digitalWrite(LED_B, HIGH);
}

void showBlue() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, LOW);
}

void showGreen() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, HIGH);
}

void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);  // Button to GND

  allOff();
  Serial.begin(9600);
  Serial.println("System started in DARK state");
}

void loop() {
  switch (currentState) {

    // ---------- DARK STATE ----------
    case DARK:
      allOff();
      if (isButtonPressed()) {
        currentState = RED;
        stateStart = millis();
        Serial.println("DARK -> RED");
      }
      break;

    // ---------- RED STATE ----------
    case RED:
      showRed();
      if (millis() - stateStart > 5000) {  // 5s timeout
        currentState = DARK;
        stateStart = millis();
        Serial.println("RED -> DARK (auto 5s)");
      }
      if (isButtonPressed()) {
        currentState = BLUE;
        stateStart = millis();
        Serial.println("RED -> BLUE (button)");
      }
      break;

    // ---------- BLUE STATE ----------
    case BLUE:
      showBlue();
      if (millis() - stateStart > 4000) {  // 4s timeout
        currentState = RED;
        stateStart = millis();
        Serial.println("BLUE -> RED (auto 4s)");
      }
      if (isButtonPressed()) {
        currentState = GREEN;
        stateStart = millis();
        Serial.println("BLUE -> GREEN (button)");
      }
      break;

    // ---------- GREEN STATE ----------
    case GREEN:
      showGreen();
      if (millis() - stateStart > 3000) {  // 3s timeout
        currentState = BLUE;
        stateStart = millis();
        Serial.println("GREEN -> BLUE (auto 3s)");
      }
      if (isButtonPressed()) {
        currentState = DARK;
        stateStart = millis();
        Serial.println("GREEN -> DARK (button)");
      }
      break;
  }
}
