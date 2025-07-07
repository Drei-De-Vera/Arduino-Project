// Read distance from TFmini-S LiDAR
int readLidar() {
    // Clear the buffer to avoid reading old data
    while (tfMiniSerial.available()) {
        tfMiniSerial.read();
    }

    unsigned long startTime = millis();

    while (millis() - startTime < 100) {  // Wait up to 100ms
        if (tfMiniSerial.available() >= 9) {
            if (tfMiniSerial.read() == 0x59 && tfMiniSerial.peek() == 0x59) {
                tfMiniSerial.read(); // Read second header byte
                uint8_t data[7];
                for (int i = 0; i < 7; i++) {
                    data[i] = tfMiniSerial.read();
                }
                 // Validate checksum
                uint8_t frame[9] = {0x59, 0x59, data[0], data[1], data[2], data[3], data[4], data[5], data[6]};
                int checksum = 0;
                for (int i = 0; i < 8; i++) {
                    checksum += frame[i];
                }

                if ((checksum & 0xFF) == frame[8]) {
                    int distance = frame[2] + (frame[3] << 8);
                    return distance;
                }
            } else {
                tfMiniSerial.read(); // discard and resync
            }
        }
    }

    return -1;  // Return -1 on failure
}
