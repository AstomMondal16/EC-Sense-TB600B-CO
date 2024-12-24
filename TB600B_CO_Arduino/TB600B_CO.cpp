#include "TB600B.h"

TB600B::TB600B(HardwareSerial& serial) : _serial(serial) {}

void TB600B::begin(uint32_t baudRate) {
    _serial.begin(baudRate);
}

void TB600B::sendCommand(const byte* command, byte* response, size_t length) {
    _serial.write(command, 9); // Send 9-byte command
    delay(1000); // Processing time
    if (_serial.available() >= length) {
        _serial.readBytes(response, length);
    }
}

void TB600B::changeMode() {
    const byte* cmd = _qnaMode ? ACTIVE_MODE : PASSIVE_MODE;
    sendCommand(cmd, nullptr, 0);
    _qnaMode = !_qnaMode;
}

void TB600B::ledOn() {
    sendCommand(LIGHT_ON, nullptr, 0);
}

void TB600B::ledOff() {
    sendCommand(LIGHT_OFF, nullptr, 0);
}

int TB600B::readGas() {
    if (!_qnaMode) {
        delay(1000);
    }
    byte response[9];
    sendCommand(READ_GAS, response, 9);
    return (response[2] << 8) | response[3];
}

void TB600B::readAll(int& gasUgm3, int& gasPpb, float& temperature, float& humidity) {
    if (!_qnaMode) {
        throw "You must be in QnA mode. Call changeMode()!";
    }
    byte response[13];
    sendCommand(READ_ALL, response, 13);
    gasUgm3 = (response[2] << 8) | response[3];
    gasPpb = (response[6] << 8) | response[7];
    temperature = ((response[8] << 8) | response[9]) / 100.0;
    humidity = ((response[10] << 8) | response[11]) / 100.0;
}
