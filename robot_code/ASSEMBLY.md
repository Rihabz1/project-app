# Robot Hardware Assembly Guide

This guide will help you assemble the line-following robot waiter.

## Components Needed

### Electronics
- **ESP32 Development Board** (or Arduino Uno + Bluetooth module)
- **L298N Motor Driver Module**
- **3x IR Line Following Sensors** (or TCRT5000 modules)
- **2x DC Geared Motors** (with wheels)
- **Jumper Wires** (Male-to-Male, Male-to-Female)
- **Breadboard** (optional)
- **LED** (for status indication)
- **Resistor** (220Ω for LED)

### Mechanical
- **Robot Chassis** (acrylic, wood, or 3D printed)
- **Wheels** (2x drive wheels + 1x caster wheel)
- **Battery Pack** (7.4V Li-Po or 6x AA battery holder)
- **Mounting Hardware** (screws, standoffs)
- **Wire Management** (zip ties, adhesive mounts)

## Assembly Steps

### 1. Chassis Preparation

1. **Mount Motors**: Attach DC motors to the rear of the chassis
2. **Install Caster Wheel**: Mount caster wheel at the front for stability
3. **Sensor Mount**: Create a front-mounted sensor array holder
4. **Electronics Area**: Reserve space for control board and motor driver

### 2. Sensor Installation

```
Sensor Layout (Front View):
[Left]  [Center]  [Right]
  34      35        32
```

1. Mount three IR sensors facing downward at the front
2. Adjust height to 5-10mm above ground
3. Space sensors approximately 30-40mm apart
4. Ensure sensors can detect line width (typically 20-25mm black tape)

### 3. Motor Wiring

**L298N to ESP32 Connections:**
```
L298N    ESP32
IN1   →  GPIO 14  (Left Motor Forward)
IN2   →  GPIO 27  (Left Motor Backward)
IN3   →  GPIO 26  (Right Motor Forward)
IN4   →  GPIO 25  (Right Motor Backward)
ENA   →  3.3V     (Left Motor Enable)
ENB   →  3.3V     (Right Motor Enable)
VCC   →  VIN      (Motor Power)
GND   →  GND      (Common Ground)
```

**Motors to L298N:**
```
Motor A (Left):  Connect to OUT1 and OUT2
Motor B (Right): Connect to OUT3 and OUT4
```

### 4. Sensor Wiring

**IR Sensors to ESP32:**
```
Sensor Pin    ESP32 Pin
VCC       →   3.3V
GND       →   GND
OUT (Left)   → GPIO 34
OUT (Center) → GPIO 35
OUT (Right)  → GPIO 32
```

### 5. Power System

1. **Battery Pack**: Use 7.4V Li-Po or 6x AA batteries
2. **Power Distribution**:
   - Battery + → L298N VCC and ESP32 VIN
   - Battery - → Common GND
3. **Power Switch**: Install between battery and system
4. **Voltage Check**: Ensure ESP32 gets 5V through VIN pin

### 6. Status LED

```
LED Connections:
ESP32 GPIO 2 → 220Ω Resistor → LED (+)
LED (-) → GND
```

## Calibration

### 1. Sensor Calibration

1. Place robot on white surface
2. Read sensor values (should be HIGH/1)
3. Place black line under each sensor
4. Read sensor values (should be LOW/0)
5. Adjust sensor height/angle if needed

### 2. Motor Calibration

1. Test individual motor directions
2. Ensure both motors rotate in correct direction for forward movement
3. Adjust motor speeds for straight-line movement
4. Fine-tune turning radii

### 3. Line Following Test

1. Create a test track with black tape on white surface
2. Start with simple straight lines
3. Add gentle curves
4. Test sharp turns and intersections

## Track Layout Design

### Basic Layout
```
    [Kitchen/Home]
         |
    ┌────┴────┐
    │         │
[Table 1] [Table 2]
    │         │
    └────┬────┘
         |
    ┌────┴────┐
    │         │
[Table 3] [Table 4]
```

### Advanced Layout
- Use 20-25mm wide black electrical tape
- Ensure good contrast with floor
- Add intersection markers for table identification
- Consider traffic flow and collision avoidance

## Programming Notes

### Distance Measurement
The robot uses time-based navigation. Calibrate these values:

```cpp
// Table distances in milliseconds of travel time
TablePosition tables[] = {
  {1, 5000, false},   // Table 1: 5 seconds from home
  {2, 8000, false},   // Table 2: 8 seconds from home
  // Add more tables as needed
};
```

### Fine-tuning Parameters
- **Motor Speed**: Adjust `motorSpeed` constant (0-255)
- **Turn Sensitivity**: Modify turn motor speed ratios
- **Sensor Threshold**: Adjust if using analog sensors

## Troubleshooting

### Robot Not Following Line
- Check sensor connections and power
- Verify sensor detection (use serial monitor)
- Ensure adequate lighting conditions
- Check line contrast and width

### Erratic Movement
- Calibrate motor speeds
- Check for loose connections
- Verify power supply voltage
- Test motors individually

### Bluetooth Issues
- Ensure ESP32 Bluetooth is enabled
- Check device pairing
- Verify baud rate settings
- Test with simple commands

## Safety Considerations

1. **Emergency Stop**: Always implement software emergency stop
2. **Physical Stop**: Include manual power switch
3. **Speed Limits**: Keep speeds reasonable for safety
4. **Obstacle Detection**: Consider adding ultrasonic sensors
5. **Battery Safety**: Use proper charging practices

## Maintenance

### Regular Checks
- Clean sensors monthly
- Check wheel alignment
- Tighten loose connections
- Monitor battery health
- Update firmware as needed

### Performance Optimization
- Log movement data for analysis
- Adjust speeds based on load
- Implement PID control for smoother operation
- Add encoders for precise distance measurement

---

**Note**: This is a basic assembly guide. Adapt components and connections based on your specific hardware choices and requirements.
