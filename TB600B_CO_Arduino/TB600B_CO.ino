#include "TB600B.h"

TB600B sensor(Serial1); // Replace Serial1 with the appropriate UART port

void setup() {
    Serial.begin(115200);
    sensor.begin(9600);
    Serial.println("TB600B Sensor Initialized.");
}

void loop() {
    sensor.ledOn();
    if (sensor.readGas() == -1) {
        sensor.changeMode();
    }

    int gasUgm3, gasPpb;
    float temperature, humidity;

    try {
        sensor.readAll(gasUgm3, gasPpb, temperature, humidity);
        Serial.print("Gas (ug/m3): ");
        Serial.println(gasUgm3);
        Serial.print("Gas (ppb): ");
        Serial.println(gasPpb);
        Serial.print("Temperature: ");
        Serial.println(temperature);
        Serial.print("Humidity: ");
        Serial.println(humidity);
    } catch (...) {
        Serial.println("Failed to read values in QnA mode.");
    }

    sensor.ledOff();
    delay(5000); // Wait 5 seconds
}
