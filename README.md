# ESP32-CAM QR Document Verification & Storage System

This repository contains the firmware for an automated document reading, verification, and storage system prototype. Built around the **ESP32-CAM** platform, the system integrates a hardware-based QR-code scanning module, non-volatile data persistence via EEPROM, and real-time wireless data logging using Bluetooth Serial.

## 📌 Overview & Operational Logic
The system automates document workflow management by tracking, counting, and validating incoming documents. Upon detecting a document via the QR reader, the system triggers an integrated motor/relay mechanism to process the document physically. An internal non-blocking timer manages the operational cycles and safeguards the motor hardware with built-in timeout routines.

---

## 🚀 Key Features

- **Hardware QR Code Scanning:** Interfaced with a dedicated UART-based QR scanning module for rapid and reliable data acquisition.
- **Wireless Telemetry (Bluetooth Serial):** Transmits scanned document data wirelessly to a connected PC or mobile device using the ESP32's native `BluetoothSerial` library. Accepts incoming Bluetooth commands for remote control (e.g., triggering motors, resetting counters).
- **Automated Motor Control:** Drives a DC motor/relay subsystem with full directional control (`FORWARD`, `REVERSE`, `STOP`) based on authorization states.
- **Hardware Protection (Timeout Management):** Implements a firmware timer (`MotorTimeoutT1`) that automatically stops the motor after a defined execution window to prevent mechanical or electrical stress.
- **Data Persistence (EEPROM):** Persists total processed document counts inside the microcontroller's EEPROM. Data survives power cycles and system reboots, ensuring structural continuity.
- **Audiovisual Diagnostic Alerts:** Direct status feedback (Success, Failure, System Idle) via onboard LED and Buzzer indicators.

---

## 🛠️ Tech Stack & Hardware Components

- **Main Controller:** ESP32-CAM Modality
- **Communication Module:** Internal Bluetooth (via `BluetoothSerial` library)
- **Scanning Peripheral:** UART/RS232-based QR-Code Reader Module
- **Actuators:** DC Motor and Relay Circuitry
- **Indicators:** Standard LED and Active/Passive Buzzer
- **Development Environment:** Arduino IDE (with ESP32 Core extension)

---

## 🔌 Hardware Pin Mapping

| Peripheral | ESP32-CAM Pin / GPIO | Interface Type | Function |
| :--- | :--- | :--- | :--- |
| **QR Reader Module** | GPIO 1 (TX) & GPIO 3 (RX) | UART Hardware Serial | Data stream reception from the scanner |
| **Bluetooth Interface** | Internal SoC Antenna | Bluetooth Serial | Wireless host communication |
| **Motor Drive Control** | GPIO 12 & GPIO 14 | Digital Output (Relay) | Actuator direction and state execution |
| **Status LED** | GPIO 33 | Digital Output | Visual system alerts |
| **Buzzer** | GPIO 4 | PWM / Digital Output | Audible feedback tones |

---

## 🧠 Firmware Architecture & Key Subsystems

The source code is structured modularly around asynchronous tasks and non-blocking timers:

1. **`QRReadFunction`**: Monitors the custom UART bus. Once a QR string payload is decoded, it packs the data and places it into a transmission queue.
2. **`BluetoothFunction`**: Handles bidirectional wireless data packages. It reports metrics to the host and parses incoming operational overrides.
3. **`MotorRun`**: Controls the physical relay bridge. Operates in `FORWARD`, `REVERSE`, or `STOP` states and is strictly monitored by the `MotorTimeoutT1` function.
4. **EEPROM Storage Layer**: Writes data increments directly to persistent sectors after every successful scan cycle, keeping the system state resilient.

---

## 💻 Installation & Deployment

1. Set up the **ESP32 Board Support Package (BSP)** within your Arduino IDE environment.
2. Install necessary dependencies (Internal `BluetoothSerial.h` and `EEPROM.h` libraries).
3. Connect the QR module and motor-relay boards according to the **Hardware Pin Mapping** table above.
4. Compile and flash the firmware onto your target ESP32-CAM module.
5. Launch a Bluetooth serial terminal on your host machine/smartphone, pair with the device, and monitor incoming logs or inject control frames.
---

## 🗺️ Future Roadmap / Phase 2: Python & OpenCV Integration

The next phase of this project focuses on migrating from a dedicated hardware QR scanning peripheral to an intelligent, software-driven computer vision pipeline, eliminating extra hardware costs and expanding system capabilities.

### 🔄 Planned System Architecture
1. **Edge Video Streaming:** The ESP32-CAM will be configured as a lightweight Wi-Fi/HTTP or WebSocket streaming server to broadcast live JPEG video frames of the document.
2. **Python Backend & OpenCV Pipeline:** A companion host application written in Python will capture the live stream. Utilizing the **OpenCV** and **ZBar** libraries, the backend will perform real-time image pre-processing (grayscale conversion, thresholding, and binarization) to detect and decode QR codes via software.
3. **Closed-Loop IoT Feedback:** Once the Python backend validates the extracted document token, it will transmit a secure execution command back to the ESP32-CAM via Wi-Fi or Bluetooth Serial to trigger the motor/relay subsystem.

This migration will demonstrate high-speed wireless video networking, multi-platform IoT integration (Microcontroller to PC), and advanced image processing capabilities.
















