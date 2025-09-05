# Smart Waiter Robot - Arduino Code

## Overview
This is the Arduino code for the Smart Waiter Robot that follows a predefined line path and communicates with the Flutter app via Bluetooth.

## Files Structure
```
robot_code/
├── smart_waiter_robot.ino         # Main Arduino sketch
├── smart_waiter_robot_simple.ino  # Simple version (text commands only)
├── README.md                      # This documentation
└── .vscode/                       # VS Code configuration
    ├── c_cpp_properties.json      # C/C++ IntelliSense config
    └── arduino.json               # Arduino IDE settings
```

## Hardware Requirements

### Arduino Board
- **Arduino Uno/Nano/Pro Mini** (recommended)
- Or any Arduino-compatible board with sufficient I/O pins

### Components
- **HC-05/HC-06 Bluetooth module** - For wireless communication
- **L298N motor driver** - To control the DC motors
- **3x IR line sensors** - For line detection (analog output preferred)
- **2x DC motors** - For robot movement
- **LED** - Status indicator (built-in LED on pin 13)
- **Jumper wires and breadboard** - For connections

## Pin Connections

### Motors (L298N Motor Driver)
```
Left Motor:  Pin 5 (PWM) & Pin 6 (PWM)
Right Motor: Pin 9 (PWM) & Pin 10 (PWM)
```

### Line Following Sensors
```
Left Sensor:   A0 (Analog)
Center Sensor: A1 (Analog) 
Right Sensor:  A2 (Analog)
```

### Bluetooth Module (HC-05)
```
HC-05 RX → Arduino Pin 2
HC-05 TX → Arduino Pin 3
HC-05 VCC → 5V
HC-05 GND → GND
```

### LED Indicator
```
Built-in LED: Pin 13 (automatically configured)
```

## Software Setup

### Arduino IDE Setup
1. **Install Arduino IDE** (version 1.8+ or Arduino IDE 2.x)
2. **Open the sketch**: `smart_waiter_robot.ino`
3. **Select board**: Arduino Uno (or your specific board)
4. **Select port**: Choose the correct COM port
5. **Upload**: Click the upload button

### VS Code Setup (Optional)
1. **Install extensions**:
   - Arduino extension
   - C/C++ extension
2. **Open project folder** in VS Code
3. **Arduino configuration** is already set up in `.vscode/arduino.json`

## Communication Protocol

The robot accepts commands via Bluetooth in two formats:

### JSON Commands (for Flutter app)
```json
{"command": "go_to_table", "table_number": 3}
{"command": "return_home"}
{"command": "stop"}
{"command": "status"}
```

### Simple Text Commands (for testing)
```
GO3     # Go to table 3
HOME    # Return home
STOP    # Stop robot
STATUS  # Get status
```

## Robot Behavior

### States
- **IDLE**: Robot is stopped and waiting for commands
- **GOING_TO_TABLE**: Robot is following the line to a specific table
- **AT_TABLE**: Robot has arrived at the target table
- **RETURNING_HOME**: Robot is returning to the home position

### Line Following Logic
- **Center sensor detects line**: Move forward
- **Left sensor detects line**: Turn left
- **Right sensor detects line**: Turn right
- **No sensors detect line**: Stop

### Table Navigation
- Uses time-based navigation (configurable in `tableDistances[]`)
- Table 1: 5 seconds travel time
- Table 2: 8 seconds travel time
- Table 3: 12 seconds travel time
- Table 4: 15 seconds travel time
- Table 5: 18 seconds travel time

## Calibration

### Sensor Threshold
Adjust the `threshold` value in the `followLine()` function:
```cpp
int threshold = 512; // Adjust based on your sensors
```

### Motor Speed
Adjust the motor speed (0-255):
```cpp
const int motorSpeed = 200; // Increase for faster movement
```

### Table Distances
Modify travel times in the `tableDistances[]` array:
```cpp
int tableDistances[] = {0, 5000, 8000, 12000, 15000, 18000};
```

## Testing

### 1. Serial Monitor Test
1. Open Arduino IDE Serial Monitor (9600 baud)
2. Send test commands: `GO1`, `HOME`, `STOP`, `STATUS`
3. Verify robot responds correctly

### 2. Bluetooth Test
1. Pair HC-05 module with phone/computer
2. Use Bluetooth terminal app
3. Send JSON commands
4. Verify responses

### 3. Hardware Test
1. Place robot on line-following track
2. Test line following behavior
3. Calibrate sensors if needed
4. Test motor movements

## Troubleshooting

### Common Issues

**Robot doesn't respond to commands**
- Check Bluetooth connection
- Verify baud rate (9600)
- Check HC-05 wiring

**Poor line following**
- Adjust sensor threshold
- Check sensor alignment
- Verify lighting conditions
- Clean sensors

**Motors don't work**
- Check L298N connections
- Verify power supply
- Test motors individually
- Check PWM pin connections

**Compilation errors**
- Ensure Arduino IDE is updated
- Check board selection
- Verify all connections

### Serial Output
The robot provides detailed serial output for debugging:
```
Smart Waiter Robot Ready!
Waiting for Bluetooth connection...
Robot initialized and ready!
Received command: GO3
Going to table 3
Arrived at table 3
```

## Integration with Flutter App

The robot communicates with the Flutter app using JSON messages over Bluetooth. The main commands supported are:

- **go_to_table**: Navigate to a specific table
- **return_home**: Return to home position
- **stop**: Emergency stop
- **status**: Get current robot state

## Next Steps

1. **Upload the code** to your Arduino
2. **Test basic functionality** using Serial Monitor
3. **Connect hardware** according to pin diagram
4. **Calibrate sensors** for your track
5. **Test Bluetooth connectivity** with Flutter app
6. **Fine-tune parameters** for optimal performance

The robot is now ready for integration with the complete Smart Waiter system!
