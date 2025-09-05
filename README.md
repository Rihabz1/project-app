# Smart Waiter Robot System

A comprehensive smart restaurant automation system featuring a line-following robot waiter, Flutter mobile application, FastAPI backend, and Supabase database integration.

## 🎯 Project Overview

This system consists of:
- **Customer Mobile App**: Order food from tables
- **Admin Dashboard**: Manage orders and control robot
- **Line-Following Robot**: Automated food delivery
- **FastAPI Backend**: RESTful API server
- **Supabase Database**: Cloud database for orders and menu

## 🏗️ Project Structure

```
smart-waiter-robot/
├── flutter_app/                    # Flutter mobile application
│   ├── lib/
│   │   ├── models/                # Data models
│   │   ├── services/              # API and Bluetooth services
│   │   ├── screens/               # UI screens
│   │   └── main.dart              # App entry point
│   └── pubspec.yaml              # Flutter dependencies
├── fastapi_backend/               # Python FastAPI server
│   ├── main.py                   # API server
│   ├── requirements.txt          # Python dependencies
│   └── .env.example             # Environment variables template
├── robot_code/                   # Arduino/ESP32 robot code
│   └── smart_waiter_robot.ino   # Robot firmware
├── database/                     # Database schema
│   └── schema.sql               # Supabase database setup
└── README.md                    # This file
```

## 🚀 Features

### Customer Features
- **Table Selection**: Choose your table number
- **Digital Menu**: Browse categorized menu items
- **Shopping Cart**: Add/remove items with quantities
- **Order Placement**: Submit orders to kitchen
- **Real-time Updates**: Order status tracking

### Admin Features
- **Order Management**: View and update order statuses
- **Robot Control**: Send robot to specific tables
- **Bluetooth Setup**: Configure robot connection
- **Dashboard**: Monitor all restaurant operations
- **Status Tracking**: Real-time order and robot status

### Robot Features
- **Line Following**: Autonomous navigation on predefined paths
- **Bluetooth Communication**: Receive commands from mobile app
- **Table Navigation**: Go to specific table numbers
- **Status Reporting**: Send position and status updates
- **Safety Features**: Stop commands and error handling

## 🛠️ Technology Stack

### Frontend
- **Flutter 3.x**: Cross-platform mobile development
- **Provider**: State management
- **GoRouter**: Navigation
- **HTTP**: API communication
- **Flutter Bluetooth Serial**: Robot communication

### Backend
- **FastAPI**: Modern Python web framework
- **Pydantic**: Data validation
- **Uvicorn**: ASGI server
- **Supabase Client**: Database integration
- **Python Bluetooth**: Robot communication

### Database
- **Supabase (PostgreSQL)**: Cloud database
- **Row Level Security**: Data protection
- **Real-time subscriptions**: Live updates

### Hardware
- **ESP32/Arduino**: Microcontroller
- **Bluetooth Module**: Wireless communication
- **Line Sensors**: Path following
- **Motors**: Movement control
- **LED Indicators**: Status display

## 📋 Prerequisites

### Software Requirements
- **Flutter SDK** (3.0+)
- **Python** (3.8+)
- **Arduino IDE** or **PlatformIO**
- **Supabase Account**

### Hardware Requirements
- **ESP32/Arduino board**
- **Bluetooth module** (if not using ESP32)
- **Line following sensors** (3x IR sensors)
- **Motor driver** (L298N or similar)
- **DC motors** (2x for differential drive)
- **Chassis and wheels**
- **Power supply** (battery pack)

## 🔧 Setup Instructions

### 1. Database Setup (Supabase)

1. Create a new Supabase project at [supabase.com](https://supabase.com)
2. Copy your project URL and API keys
3. Run the SQL schema from `database/schema.sql` in Supabase SQL editor
4. Enable Row Level Security and configure policies as needed

### 2. Backend Setup

1. Navigate to the backend directory:
   ```bash
   cd fastapi_backend
   ```

2. Create a virtual environment:
   ```bash
   python -m venv venv
   source venv/bin/activate  # On Windows: venv\Scripts\activate
   ```

3. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```

4. Create environment file:
   ```bash
   cp .env.example .env
   ```

5. Update `.env` with your Supabase credentials:
   ```env
   SUPABASE_URL=your_supabase_url_here
   SUPABASE_ANON_KEY=your_supabase_anon_key_here
   SUPABASE_SERVICE_KEY=your_supabase_service_key_here
   ```

6. Run the server:
   ```bash
   uvicorn main:app --reload --host 0.0.0.0 --port 8000
   ```

### 3. Flutter App Setup

1. Navigate to the Flutter directory:
   ```bash
   cd flutter_app
   ```

2. Install dependencies:
   ```bash
   flutter pub get
   ```

3. Generate model classes:
   ```bash
   flutter packages pub run build_runner build
   ```

4. Update API base URL in `lib/services/api_service.dart`:
   ```dart
   static const String baseUrl = 'http://YOUR_BACKEND_IP:8000';
   ```

5. Run the app:
   ```bash
   flutter run
   ```

### 4. Robot Hardware Setup

#### Wiring Diagram
```
ESP32 Connections:
├── Line Sensors
│   ├── Left Sensor → GPIO 34
│   ├── Center Sensor → GPIO 35
│   └── Right Sensor → GPIO 32
├── Motors (via L298N)
│   ├── Left Motor
│   │   ├── IN1 → GPIO 14
│   │   └── IN2 → GPIO 27
│   └── Right Motor
│       ├── IN3 → GPIO 26
│       └── IN4 → GPIO 25
├── Status LED → GPIO 2
└── Power → VIN (7-12V)
```

#### Code Upload
1. Open `robot_code/smart_waiter_robot.ino` in Arduino IDE
2. Install required libraries:
   - **ArduinoJson** (for JSON communication)
   - **ESP32 Board Package** (if using ESP32)
3. Select your board and port
4. Upload the code

### 5. Path Layout Setup

Create a line-following path in your restaurant:
1. Use black electrical tape on light-colored floor
2. Create a main path with branches to each table
3. Mark table positions at specific distances from home
4. Adjust `tables[]` array in robot code with actual distances

## 📱 Usage Guide

### For Customers

1. **Launch App**: Open the Smart Waiter app
2. **Select Table**: Choose your table number from the grid
3. **Browse Menu**: View available items by category
4. **Add to Cart**: Tap + to add items, - to remove
5. **Place Order**: Review cart and tap "Place Order"
6. **Wait for Delivery**: Robot will automatically deliver when ready

### For Staff/Admin

1. **Access Dashboard**: Tap "Admin Panel" on home screen
2. **Monitor Orders**: View pending, preparing, and ready orders
3. **Update Status**: Mark orders as preparing/ready/delivered
4. **Control Robot**: Send robot to tables or return home
5. **Bluetooth Setup**: Configure robot connection if needed

### Robot Operation

1. **Power On**: Robot starts at home position
2. **Bluetooth Pairing**: Pair with mobile device as "SmartWaiterRobot"
3. **Automatic Operation**: Receives commands via Bluetooth
4. **Manual Override**: Use admin controls for manual operation

## 🔧 Configuration

### API Endpoints

The FastAPI backend provides these main endpoints:

- `GET /tables` - List all tables
- `GET /menu` - Get menu items
- `POST /orders` - Create new order
- `GET /orders` - List orders (filterable by status)
- `PUT /orders/{id}/status` - Update order status
- `POST /robot/command` - Send command to robot
- `GET /robot/status` - Get robot status

### Bluetooth Commands

Robot accepts JSON commands:
```json
{
  "command": "go_to_table",
  "table_number": 5
}
```

Available commands:
- `go_to_table` - Navigate to specific table
- `return_home` - Return to home position
- `stop` - Emergency stop
- `status` - Request current status

### Database Schema

Key tables:
- **tables**: Restaurant table information
- **menu_items**: Available food items
- **orders**: Customer orders
- **order_items**: Individual items in orders
- **robot_commands**: Command history

## 🐛 Troubleshooting

### Common Issues

**Flutter Build Errors**
```bash
flutter clean
flutter pub get
flutter packages pub run build_runner build --delete-conflicting-outputs
```

**Bluetooth Connection Issues**
- Ensure robot is powered and in pairing mode
- Check Bluetooth permissions in app settings
- Verify device is bonded in system Bluetooth settings

**API Connection Errors**
- Check backend server is running
- Verify IP address in Flutter app
- Ensure firewall allows connections on port 8000

**Robot Not Following Line**
- Check sensor connections and values
- Adjust sensor thresholds in code
- Verify line contrast and lighting conditions

### Performance Optimization

**Database**
- Add indexes for frequently queried columns
- Use connection pooling for high traffic
- Enable query optimization

**Mobile App**
- Implement local caching for menu items
- Use pagination for large order lists
- Optimize image loading and caching

**Robot**
- Calibrate sensors for your environment
- Adjust motor speeds for smooth operation
- Implement PID control for better line following

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🙏 Acknowledgments

- Flutter team for the excellent framework
- FastAPI for the modern Python web framework
- Supabase for the backend-as-a-service platform
- Arduino community for hardware libraries and examples

## 📞 Support

For issues and questions:
- Create an issue in the GitHub repository
- Check the troubleshooting section above
- Review the documentation in each component folder

---

**Happy Automating! 🤖🍽️**
