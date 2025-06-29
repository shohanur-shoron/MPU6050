# Real-Time MPU6050 IMU 3D Visualization

This project captures orientation data (roll, pitch, and yaw) from an MPU6050 Inertial Measurement Unit (IMU) connected to an ESP8266 (or other Arduino-compatible board). The data is sent via a serial connection to a Python script, which uses PyOpenGL and PyGame to render a real-time 3D visualization of the sensor's orientation.

Additionally, the Arduino code demonstrates how to control a servo motor based on the calculated roll angle.


*(A sample GIF showing the 3D model rotating in sync with the physical IMU board)*

## Features

- **Real-time Sensor Fusion:** Reads accelerometer and gyroscope data from the MPU6050.
- **Complementary Filter:** A simple and effective filter is used on the Arduino to combine sensor readings for stable angle calculations.
- **3D Visualization:** A Python script uses PyOpenGL to draw a 3D cuboid that visually represents the IMU's current orientation.
- **Serial Communication:** Efficient request/response communication between the Arduino and the Python script.
- **Interactive Controls:** Toggle yaw tracking on the fly and quit the application gracefully.
- **Servo Control:** The Arduino sketch includes code to control a servo motor based on the roll angle, perfect for projects like self-balancing robots or camera gimbals.

## How It Works

1.  **Arduino/ESP8266:**
    - The microcontroller continuously reads raw data from the MPU6050 sensor.
    - It applies a complementary filter to the raw values to calculate stable roll, pitch, and yaw angles.
    - It listens for a `.` character over the serial port. Upon receiving it, it sends back the latest calculated angles as a comma-separated string (e.g., `-10.24, 5.12, 95.80`).
    - It also drives a connected servo motor to an angle corresponding to the current roll value.

2.  **Python Script:**
    - Initializes a PyGame window with an OpenGL context.
    - In a loop, it sends a `.` character to the specified serial port to request data from the Arduino.
    - It reads and parses the incoming line of data to get the roll, pitch, and yaw angles.
    - It uses these angles to apply rotations (`glRotatef`) to the 3D model, updating its orientation on the screen.
    - It handles user input for quitting the application or toggling yaw mode.



## How Accelerometers works
![mainStructure](https://github.com/user-attachments/assets/9dd896da-d331-4ef7-8c9e-fde9a694f9b6)

![proof mass](https://github.com/user-attachments/assets/28fed43b-ec96-4b46-9737-9cebe35dcb7a)
![substrate](https://github.com/user-attachments/assets/d448f572-7aa5-4c53-a6cc-885261406377)
![senceFinger](https://github.com/user-attachments/assets/ab13776f-835a-49ff-b6b9-d8df190672de)
![electrodes](https://github.com/user-attachments/assets/58ad79f9-2863-4b73-9a94-ed3ed04ae844)


![capacitance](https://github.com/user-attachments/assets/66bf8b38-2dc6-4344-8e8f-f0ba71940c3f)

![capacitance charge changes](https://github.com/user-attachments/assets/ddb07410-322a-4c8f-b97b-10adc26bc4f1)

![substrateGIF](https://github.com/user-attachments/assets/2e437e96-6e40-4ddc-9061-1e4326a49127)

![movement](https://github.com/user-attachments/assets/87a873c0-df48-4878-aca0-b4162bfc714e)

![three axis](https://github.com/user-attachments/assets/68d741a1-b709-4ff0-b490-1aab2e53a03e)

## How Gyroscopes works

![gyro01](https://github.com/user-attachments/assets/64f2992c-de86-40fd-b57d-27c7f159a873)
![gyro04](https://github.com/user-attachments/assets/a9784315-e2f0-40b4-a4ce-e401bfb59ea4)
![gyro03](https://github.com/user-attachments/assets/0f6c4196-ea1a-4f98-aecf-b2a7827d6d16)
![gyro02](https://github.com/user-attachments/assets/46453876-b3ee-471a-ac0b-5b4f0a9e56b5)


## Hardware Requirements

- An Arduino-compatible board (the code is configured for an ESP8266 like a Wemos D1 Mini or NodeMCU).
- MPU6050 Gyroscope/Accelerometer Module.
- A standard servo motor (optional).
- Jumper wires.

### Wiring (for ESP8266)

- **MPU6050 VCC** -> **3.3V**
- **MPU6050 GND** -> **GND**
- **MPU6050 SCL** -> **D1** (GPIO5)
- **MPU6050 SDA** -> **D2** (GPIO4)
- **Servo Signal** -> **D4** (GPIO2)
- **Servo VCC** -> **5V / VIN**
- **Servo GND** -> **GND**

## Software & Dependencies

### Arduino

- [Arduino IDE](https://www.arduino.cc/en/software)
- Board support for your microcontroller (e.g., ESP8266 Core).
- `Wire.h` and `Servo.h` libraries (typically included with the board core).

### Python

- Python 3.x
- PyOpenGL
- PyGame
- PySerial

You can install the required Python packages using pip:

```bash
pip install PyOpenGL PyGame pyserial
```

## Setup and Usage

1.  **Hardware:** Connect the components as described in the wiring diagram above.
2.  **Arduino:**
    - Open the `.ino` sketch in the Arduino IDE.
    - Select your board (e.g., "LOLIN(WEMOS) D1 R2 & mini") and the correct COM port from the `Tools` menu.
    - Upload the sketch to your board.
3.  **Python:**
    - Open the `main.py` file.
    - **Crucially, find this line and change `'COM3'` to the COM port your Arduino is using:**
      ```python
      ser = serial.Serial('COM3', 115200)
      ```
4.  **Run:**
    - Execute the Python script from your terminal:
      ```bash
      python main.py
      ```
    - A window should appear showing the 3D model. Tilt your MPU6050 sensor to see it move!

## Controls

- **`z` key:** Toggles yaw tracking on or off. When toggled on, it also resets the current yaw angle to zero.
- **`Esc` key:** Closes the application.
