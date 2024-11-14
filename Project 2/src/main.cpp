#include <Arduino.h>

const int redLEDPin = 5;
const int yellowLEDPin = 4;
const int greenLEDPin = 3;
const int blueLEDPin = 2;

const int redSwitchPin = 6;
const int yellowSwitchPin = 7;
const int greenSwitchPin = 8;
const int blueSwitchPin = 9;

int sequence[100];  // Array to store the sequence of LED colors
int sequenceLength = 0;  // Length of the sequence
int playerStep = 0;  // Player's current step to check against sequence

int whichButton();
void showSequence();
void addRandomColorToSequence();
void doOne(int pin, int toneFrequency);
void endGame();
void successTone();

// SOUND
// tone(10,pitch,time in milis);
// delay(10);

// LED
// digitalWrite(LEDPin, HIGH or LOW);

// BUTTONS
// switchState = digitalRead(2);

void setup() {
  Serial.begin(9600);
  pinMode(redLEDPin, OUTPUT);
  pinMode(yellowLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);

  pinMode(redSwitchPin, INPUT);
  pinMode(yellowSwitchPin, INPUT);
  pinMode(greenSwitchPin, INPUT);
  pinMode(blueSwitchPin, INPUT);

  randomSeed(analogRead(0));  // Initialize random number generator
  addRandomColorToSequence();  // Start the game by adding the first color
}

void loop() {
  // Show the sequence to the player
  showSequence();

  // Wait for the player to input their response
  playerStep = 0;  // Ensure playerStep starts from 0 at the start of the round

  // Wait for the player to input and match the sequence
  while (playerStep < sequenceLength) {
    int buttonPressed = whichButton();  // Check which button is pressed

    if (buttonPressed > 0) {
      Serial.print("Button pressed: ");
      Serial.println(buttonPressed);

      // Check if the button pressed matches the sequence
      if (buttonPressed == sequence[playerStep]) {
        playerStep++;
        delay(300);  // Debounce delay to prevent multiple presses being detected too quickly
      } else {
        endGame();  // Player pressed the wrong button, game ends
        return;  // End the game if the player is wrong
      }
    }
  }

  // Play a success tone after completing the sequence
  successTone();
  delay(1000);  // Short delay before adding a new color to the sequence
  addRandomColorToSequence();  // Add a new color to the sequence for the next round
  // No need to reset `playerStep` here, because we already reset it at the beginning of the round
}

// Check which button was pressed
int whichButton() {
  if (digitalRead(redSwitchPin) == HIGH) {
    return 1;  // Red button
  } else if (digitalRead(yellowSwitchPin) == HIGH) {
    return 2;  // Yellow button
  } else if (digitalRead(greenSwitchPin) == HIGH) {
    return 3;  // Green button
  } else if (digitalRead(blueSwitchPin) == HIGH) {
    return 4;  // Blue button
  } else {
    return 0;  // No button pressed
  }
}

// Show the sequence to the player
void showSequence() {
  Serial.println("Showing sequence...");
  for (int i = 0; i < sequenceLength; i++) {
    switch (sequence[i]) {
      case 1:
        doOne(redLEDPin, 300);  // Red LED
        break;
      case 2:
        doOne(yellowLEDPin, 400);  // Yellow LED
        break;
      case 3:
        doOne(greenLEDPin, 500);  // Green LED
        break;
      case 4:
        doOne(blueLEDPin, 600);  // Blue LED
        break;
    }
    delay(500);  // Delay between each LED flash
  }
  delay(1000);  // Pause before player input
}

// Add a random color to the sequence
void addRandomColorToSequence() {
  int color = random(1, 5);  // Random number between 1 and 4
  sequence[sequenceLength] = color;
  sequenceLength++;
}

// Flash the LED and play a tone for each color
void doOne(int pin, int toneFrequency) {
  digitalWrite(pin, HIGH);
  tone(10, toneFrequency, 200);  // Different tones for each color LED
  delay(300);
  digitalWrite(pin, LOW);
}

// Play a success tone after completing the sequence
void successTone() {
  // Play a success tone (higher pitch for success)
  tone(10, 1000, 500);  // Success tone at 1000Hz for 500ms
  delay(500);
}

// End the game by resetting the sequence and player step
void endGame() {
  Serial.println("Game Over! Restarting...");
  sequenceLength = 0;  // Reset the sequence length
  playerStep = 0;  // Reset the player's step
  delay(2000);  // Wait before restarting the game
  addRandomColorToSequence();  // Add the first color to start the new game
}
