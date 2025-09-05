import 'dart:convert';
import 'package:http/http.dart' as http;
import '../models/models.dart';

class ApiService {
  static const String baseUrl = 'http://localhost:8000'; // Change to your FastAPI server URL
  
  // Tables
  Future<List<Table>> getTables() async {
    final response = await http.get(Uri.parse('$baseUrl/tables'));
    if (response.statusCode == 200) {
      final List<dynamic> data = json.decode(response.body);
      return data.map((json) => Table.fromJson(json)).toList();
    } else {
      throw Exception('Failed to load tables');
    }
  }

  // Menu
  Future<List<MenuItem>> getMenu() async {
    final response = await http.get(Uri.parse('$baseUrl/menu'));
    if (response.statusCode == 200) {
      final List<dynamic> data = json.decode(response.body);
      return data.map((json) => MenuItem.fromJson(json)).toList();
    } else {
      throw Exception('Failed to load menu');
    }
  }

  // Orders
  Future<Order> createOrder(Order order) async {
    final response = await http.post(
      Uri.parse('$baseUrl/orders'),
      headers: {'Content-Type': 'application/json'},
      body: json.encode(order.toJson()),
    );
    
    if (response.statusCode == 200) {
      return Order.fromJson(json.decode(response.body));
    } else {
      throw Exception('Failed to create order');
    }
  }

  Future<List<Order>> getOrders({String? status}) async {
    String url = '$baseUrl/orders';
    if (status != null) {
      url += '?status=$status';
    }
    
    final response = await http.get(Uri.parse(url));
    if (response.statusCode == 200) {
      final List<dynamic> data = json.decode(response.body);
      return data.map((json) => Order.fromJson(json)).toList();
    } else {
      throw Exception('Failed to load orders');
    }
  }

  Future<Order> getOrder(int orderId) async {
    final response = await http.get(Uri.parse('$baseUrl/orders/$orderId'));
    if (response.statusCode == 200) {
      return Order.fromJson(json.decode(response.body));
    } else {
      throw Exception('Failed to load order');
    }
  }

  Future<Map<String, dynamic>> updateOrderStatus(int orderId, String status) async {
    final response = await http.put(
      Uri.parse('$baseUrl/orders/$orderId/status'),
      headers: {'Content-Type': 'application/json'},
      body: json.encode({'status': status}),
    );
    
    if (response.statusCode == 200) {
      return json.decode(response.body);
    } else {
      throw Exception('Failed to update order status');
    }
  }

  // Robot Control
  Future<Map<String, dynamic>> sendRobotCommand(RobotCommand command) async {
    final response = await http.post(
      Uri.parse('$baseUrl/robot/command'),
      headers: {'Content-Type': 'application/json'},
      body: json.encode(command.toJson()),
    );
    
    if (response.statusCode == 200) {
      return json.decode(response.body);
    } else {
      throw Exception('Failed to send robot command');
    }
  }

  Future<RobotStatus> getRobotStatus() async {
    final response = await http.get(Uri.parse('$baseUrl/robot/status'));
    if (response.statusCode == 200) {
      return RobotStatus.fromJson(json.decode(response.body));
    } else {
      throw Exception('Failed to get robot status');
    }
  }
}
