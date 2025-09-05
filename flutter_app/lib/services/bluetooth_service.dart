import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import '../models/models.dart';

class BluetoothService {
  BluetoothConnection? _connection;
  bool get isConnected => _connection?.isConnected ?? false;
  
  Future<List<BluetoothDevice>> getBondedDevices() async {
    return await FlutterBluetoothSerial.instance.getBondedDevices();
  }

  Future<bool> connectToDevice(BluetoothDevice device) async {
    try {
      _connection = await BluetoothConnection.toAddress(device.address);
      return true;
    } catch (e) {
      print('Error connecting to device: $e');
      return false;
    }
  }

  Future<void> disconnect() async {
    _connection?.dispose();
    _connection = null;
  }

  Future<bool> sendCommand(RobotCommand command) async {
    if (!isConnected) return false;
    
    try {
      String jsonCommand = json.encode(command.toJson());
      _connection!.output.add(Uint8List.fromList(utf8.encode('$jsonCommand\n')));
      await _connection!.output.allSent;
      return true;
    } catch (e) {
      print('Error sending command: $e');
      return false;
    }
  }

  Stream<String>? getDataStream() {
    if (!isConnected) return null;
    
    return _connection!.input!
        .map((Uint8List data) => utf8.decode(data))
        .transform(StreamTransformer<String, String>.fromHandlers(
          handleData: (data, sink) {
            // Handle incoming data from robot
            sink.add(data);
          },
        ));
  }

  Future<bool> goToTable(int tableNumber) async {
    final command = RobotCommand(
      command: 'go_to_table',
      tableNumber: tableNumber,
    );
    return await sendCommand(command);
  }

  Future<bool> returnHome() async {
    final command = RobotCommand(command: 'return_home');
    return await sendCommand(command);
  }

  Future<bool> stopRobot() async {
    final command = RobotCommand(command: 'stop');
    return await sendCommand(command);
  }

  Future<bool> requestStatus() async {
    final command = RobotCommand(command: 'status');
    return await sendCommand(command);
  }
}
