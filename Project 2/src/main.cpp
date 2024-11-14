#include <Arduino.h>

const int redLEDPin = 5;
const int yellowLEDPin = 4;
const int greenLEDPin = 3;
const int blueLEDPin = 2;

const int redSwitchPin = 6;
const int yellowSwitchPin = 7;
const int greenSwitchPin = 8;
const int blueSwitchPin = 9;

const int maxSeqLen = 100;
const int tonec4 = 261;
const int toneg4 = 392;
const int tonec5 = 523;
const int tonef5 = 698;
const int tonefs5 = 740;
const int toneg5 = 784;
const int tonec6 = 1046;

int sequence[maxSeqLen];  // Array to store the sequence of LED colors
int sequenceLength = 0;  // Length of the sequence
int playerStep = 0;  // Player's current step to check against sequence

int whichButton();
void showSequence();
void addRandomColorToSequence();
void doOne(int pin, int toneFrequency);
void endGame();
void successTone();
void gameStartTone();
void gameFailTone();
void gameWinTone();

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
        gameFailTone();
        endGame();  // Player pressed the wrong button, game ends
        return;  // End the game if the player is wrong
      }
    }
  }

  //we have reached the end of the max sequence. the player has WON simon. somehow.
  if(playerStep == maxSeqLen) { 
    gameWinTone();
    endGame();
    return;

  }
  
  //otherwise: we are not at the end of the game, continue on.
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
        doOne(redLEDPin, tonec4);  // Red LED
        break;
      case 2:
        doOne(yellowLEDPin, toneg4);  // Yellow LED
        break;
      case 3:
        doOne(greenLEDPin, tonec5);  // Green LED
        break;
      case 4:
        doOne(blueLEDPin, toneg5);  // Blue LED
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
  tone(10, tonec6, 500);  // Success tone at 1046Hz (Roughly C6) for 500ms
  delay(500);
}

// Play a series of tones to indicate the game beginning 
void gameStartTone() {
  tone(10, tonec4, 125);
  delay(125);
  tone(10, toneg4, 125);
  delay(125);
  tone(10, tonec5, 125);
  delay(125);
  tone(10, toneg5, 125);
  delay(625);
  tone(10, tonec6, 125);
  delay(125);
}

//Play a series of tones to indicate inputting the wrong solution and the game ending
void gameFailTone() {
  tone(10, tonefs5, 125);
  delay(125);
  tone(10, tonef5, 125);
  delay(125);
  tone(10, toneg5, 250);
  delay(750);
  tone(10, tonec4, 125);
  delay(125);
}

//Play a tone to indicate you've won the game (somehow doing the WHOLE sequence)
void gameWinTone() {
  for(int i = 0; i < 3; ++i) {
    tone(10, toneg4, 125);
    delay(125);
    tone(10, tonec5, 125);
    delay(125);
    tone(10, toneg5, 125);
    delay(125);
    tone(10, tonec6, 125);
    delay(125);
  }
  tone(10, tonec4, 125);
  delay(250);
  tone(10, tonec4, 125);
  delay(250);
}


// End the game by resetting the sequence and player step
void endGame() {
  Serial.println("Game Over! Restarting...");
  sequenceLength = 0;  // Reset the sequence length
  playerStep = 0;  // Reset the player's step
  delay(2000);  // Wait before restarting the game
  addRandomColorToSequence();  // Add the first color to start the new game
}
