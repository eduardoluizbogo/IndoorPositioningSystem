### Indoor Localization for Automated Guided Vehicle (AGV)

#### Overview

This project focuses on the development of an indoor localization system for an Automated Guided Vehicle (AGV) using Bluetooth technology and Arduino microcontrollers. The main goal is to create an accurate and cost-effective solution for indoor navigation in environments such as factories, warehouses, and hospitals.

#### Objectives
- Achieve localization accuracy within 2 meters.
- Provide location updates every 20 seconds.

#### Hardware Selection
- **Bluetooth Beacons**: Adapted or alternative Bluetooth beacons are used for indoor communication.
- **ESP32**: Chosen as the receiver due to its Bluetooth communication capabilities, efficient processing, and affordability.
- **Other Components**: 
  - 1 ESP32
  - 3 HC-06 Bluetooth modules (slave only)
  - 3 Arduino Uno boards
  - Connection cables
  - Computers and/or batteries

#### Software Development
- **Beacon Programming**: Beacons are programmed to emit signals at regular intervals.
- **ESP32 Programming**: The ESP32 measures the signal strength from each beacon, uses a triangulation algorithm to estimate the position, and controls the AGV's movements accordingly. An interface is developed for visualizing the location and diagnosing errors.

#### Methodology
1. **Signal Measurement**: The AGV measures signals from multiple beacons.
2. **Distance Calculation**: Using the signal strength, the system estimates the distance between the AGV and each beacon.
3. **Position Determination**: The triangulation algorithm calculates the AGV's position using geometric techniques.

#### Key Techniques
- **RSSI (Received Signal Strength Indication)**: Measures the signal strength to estimate distance.
- **Triangulation**: Uses distances from at least three beacons to determine the position.
- **Web Server**: A web server on the ESP32 collects position data and plots it for real-time visualization.

#### Implementation
- **System Setup**: Beacons are strategically placed in the designated area to ensure full coverage. Initial tests ensure proper connectivity and operation between beacons and the ESP32.
- **Data Transmission**: Position data is sent from the Arduino to MATLAB via serial communication, although a web server is preferred for robustness and efficiency.

#### Challenges and Solutions
- **Initial Attempts**: The project initially used HC-06 Bluetooth modules, which only support slave mode, making them unsuitable for network scanning. Alternatives like the ESP8266 were considered but lacked Bluetooth capabilities. Ultimately, the ESP32 was selected for its ability to act as a master device, scan networks, and connect to other Bluetooth devices.

#### References
1. Adaptive Path Loss Model for BLE Indoor Positioning System
2. System of Indoor Localization Using Bluetooth Antennas
3. Trajectory Tracking Control Method and Experiment of AGV
4. Indoor Positioning System based on Bluetooth Low Energy
5. Automatic Calibration for Log-Normal Path Loss Model Based on Bluetooth Low Energy Beacons
6. Indoor Positioning Technologies
7. BLUEPATH: Indoor Localization System
8. A Localization Algorithm for Wireless Networks Using Signal Strength Levels
9. Environment Adaptive Localization Method Using Wi-Fi and Bluetooth Low Energy

This overview provides a concise summary of the project, highlighting the objectives, hardware and software choices, methodology, key techniques, and challenges encountered during development. For detailed code and further implementation details, please refer to the source code and documentation provided in the repository.
