/*
  State change detection (edge detection)
 	
 Often, you don't need to know the state of a digital input all the time,
 but you just need to know when the input changes from one state to another.
 For example, you want to know when a button goes from OFF to ON.  This is called
 state change detection, or edge detection.
 
 This example shows how to detect when a button or button changes from off to on
 and on to off.
 	
 The circuit:
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 * LED attached from pin 13 to ground (or use the built-in LED on
   most Arduino boards)
 
 created  27 Sep 2005
 modified 30 Aug 2011
 by Tom Igoe

This example code is in the public domain.
 	
 http://arduino.cc/en/Tutorial/ButtonStateChange
 
 */

// this constant won't change:
const int  buttonPinFirst = 2;    // the pin that the pushbutton is attached to (first)
const int  buttonPinLast = 4;    // the pin that the pushbutton is attached to (last)
const unsigned long debounceDelay = 30;  // the debounce time; increase if the output flickers
// Variables will change:

int buttonState[14] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};         // current state of the button
int lastButtonState[14] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};     // previous state of the button
unsigned long lastDebounceTime[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // the last time the output pin was toggled

int reading = LOW;

void setup() {
  // initialize the button pin as a input:
  for (int buttonPin=buttonPinFirst; buttonPin<=buttonPinLast; buttonPin++) {
	  pinMode(buttonPin, INPUT);
  }	  

  // initialize serial communication:
  Serial.begin(115200);
}


void loop() {

  for (int buttonPin=buttonPinFirst; buttonPin<=buttonPinLast; buttonPin++) {
      reading = digitalRead(buttonPin);
	  // read the pushbutton input pin:
      //	  buttonState[buttonPin] = digitalRead(buttonPin);

	 // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing:
	if (reading != lastButtonState[buttonPin]) {
	// reset the debouncing timer
	lastDebounceTime[buttonPin] = millis();
	}	
	if ((millis() - lastDebounceTime[buttonPin]) > debounceDelay) {
	// whatever the reading is at, it's been there for longer than the debounce
	// delay, so take it as the actual current state:
		// if the button state has changed:
		if (reading != buttonState[buttonPin]) {
		  buttonState[buttonPin] = reading;

		// if the state has changed, increment the counter
 			if (buttonState[buttonPin] == HIGH) {
			  // if the current state is HIGH then the button
			  // wend from off to on:

				  Serial.print("on ");
				  Serial.println(buttonPin);

     		} else {
		  	// if the current state is LOW then the button
		  	// wend from on to off:
		  	Serial.print("off "); 
  		  	Serial.println(buttonPin);
			}
	  }
	  // save the current state as the last state, 
	  //for next time through the loop
	}  
    lastButtonState[buttonPin] = reading;
  }
}









