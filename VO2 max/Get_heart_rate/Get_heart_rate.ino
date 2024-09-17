unsigned long previousMillis = 0;  // Store the time of the previous peak
unsigned long currentMillis = 0;   // Store the current time
int threshold = 900;               // Set an arbitrary threshold for R-peak detection
int bpm = 0;

void setup() {
  Serial.begin(9600);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
}

void loop() {
  int ecgValue = analogRead(A0);   // Read the ECG signal

  // Detect the R-wave (when the signal goes above a threshold)
  if (ecgValue > threshold) {
    currentMillis = millis();      // Get the current time in milliseconds
    unsigned long interval = currentMillis - previousMillis;  // Calculate time between peaks

    if (interval > 300) {  // Ignore very short intervals to avoid noise
      bpm = 60000 / interval;  // Convert the interval to BPM (60000 ms in 1 minute)
      previousMillis = currentMillis;  // Update the previous peak time
      Serial.print("Heart Rate: ");
      Serial.print(bpm);
      Serial.println(" BPM");
    }
  } else {
    // Print ECG signal for debugging (optional)
    // Serial.println(ecgValue);
  }

  delay(1);  // Small delay for stability
}
