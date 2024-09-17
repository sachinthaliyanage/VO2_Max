// Arduino code for Zero Crossing with Signal Strength Algorithm using LSM6DS3

#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

const int start = 1;
const int stop = 1600;
const float fs = 100.0;
float filt1[stop - start + 1];
float Gyro[stop - start + 1]; // Placeholder for gyro data
int maincount = 0;
bool gyro_count_enable = false;
float gyro_up_peak = 0;
float gyro_down_peak = 0;
const float threshold1 = 0.05;

Adafruit_SSD1306 display = Adafruit_SSD1306(128,64,&Wire);

void setup() {
    Serial.begin(9600);
    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1);
    }
    display.begin(SSD1306_SWITCHCAPVCC,0x3c);
}

void loop() {
    static float in[7] = {0};
    static float Ou[7] = {0};
    static int counter1 = 0;
    static int counter2 = 0;
    static int var_k = 0;

    // Read gyroscope data
    float gx, gy, gz;
    if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(gx, gy, gz);
        // Store the gyroscope data in the array
        for (int i = 0; i < stop - start; i++) {
            Gyro[i] = Gyro[i + 1]; // Shift data
        }
        Gyro[stop - start] = gx; // Use gx for filtering
    }

    for (int i = start; i <= stop; i++) {
        in[6] = Gyro[i]; // Current gyro reading

        // Shift input and output arrays
        for (int j = 0; j < 6; j++) {
            in[j] = in[j + 1];
            Ou[j] = Ou[j + 1];
        }

        // Apply Butterworth filter (simplified)
        filt1[i] = ((in[0] + in[6]) * 0.000000000853160) +
                    ((in[1] + in[5]) * 0.000000005118957) +
                    ((in[2] + in[4]) * 0.000000012797393) +
                    (in[3] * 0.000000017063191) +
                    (Ou[0] * -0.7844171769) +
                    (Ou[1] * 4.8969248914) +
                    (Ou[2] * -12.7416173292) +
                    (Ou[3] * 17.6873761799) +
                    (Ou[4] * -13.8155108061) +
                    (Ou[5] * 5.7572441862);
        Ou[6] = filt1[i];

        // Detecting Zero Cross
        if ((Ou[6] < 0) && (Ou[5] > 0) && gyro_count_enable) {
            counter1 = 1;
            counter2 = 0;
            var_k = i;
            if (gyro_up_peak > threshold1) {
                maincount++;
            }
        }

        if ((Ou[6] > 0) && (Ou[5] < 0) && gyro_count_enable) {
            counter2 = 1;
            counter1 = 0;
            var_k = i;
            if (gyro_down_peak < -threshold1) {
                maincount++;
            }
        }

        // Detecting Upward and Downward Peaks
        if ((Ou[6] < Ou[5]) && (Ou[5] > Ou[4] && counter2 == 1)) {
            gyro_up_peak = Ou[5];
        }
        if ((Ou[6] > Ou[5]) && (Ou[5] < Ou[4] && counter1 == 1)) {
            gyro_down_peak = Ou[5];
        }

        // Sample Time Out
        if (((counter1 == 1) && (i < var_k + 15)) || ((counter2 == 1) && (i < var_k + 15))) {
            gyro_count_enable = false;
        } else {
            gyro_count_enable = true;
        }
    }

    Serial.println(maincount);
    delay(1000); // Adjust delay as needed
    display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.setCursor(0,16);
  display.print(maincount);

  display.display();

}