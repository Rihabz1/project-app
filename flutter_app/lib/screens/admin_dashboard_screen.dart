import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:go_router/go_router.dart';
import '../models/models.dart';
import '../services/api_service.dart';
import '../services/bluetooth_service.dart';

class AdminDashboardScreen extends StatefulWidget {
  const AdminDashboardScreen({super.key});

  @override
  State<AdminDashboardScreen> createState() => _AdminDashboardScreenState();
}

class _AdminDashboardScreenState extends State<AdminDashboardScreen> with TickerProviderStateMixin {
  late TabController _tabController;
  List<Order> orders = [];
  RobotStatus? robotStatus;
  bool isLoading = true;

  @override
  void initState() {
    super.initState();
    _tabController = TabController(length: 3, vsync: this);
    _loadData();
    _startPeriodicUpdates();
  }

  @override
  void dispose() {
    _tabController.dispose();
    super.dispose();
  }

  Future<void> _loadData() async {
    await _loadOrders();
    await _loadRobotStatus();
    setState(() {
      isLoading = false;
    });
  }

  Future<void> _loadOrders() async {
    try {
      final apiService = Provider.of<ApiService>(context, listen: false);
      final loadedOrders = await apiService.getOrders();
      setState(() {
        orders = loadedOrders;
      });
    } catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(content: Text('Failed to load orders: $e')),
        );
      }
    }
  }

  Future<void> _loadRobotStatus() async {
    try {
      final apiService = Provider.of<ApiService>(context, listen: false);
      final status = await apiService.getRobotStatus();
      setState(() {
        robotStatus = status;
      });
    } catch (e) {
      // Robot status might not be available if robot is not connected
      print('Robot status unavailable: $e');
    }
  }

  void _startPeriodicUpdates() {
    // Update data every 10 seconds
    Future.delayed(const Duration(seconds: 10), () {
      if (mounted) {
        _loadData();
        _startPeriodicUpdates();
      }
    });
  }

  Future<void> _updateOrderStatus(int orderId, String newStatus) async {
    try {
      final apiService = Provider.of<ApiService>(context, listen: false);
      await apiService.updateOrderStatus(orderId, newStatus);
      await _loadOrders();
      
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('Order status updated to $newStatus'),
            backgroundColor: Colors.green,
          ),
        );
      }
    } catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('Failed to update order: $e'),
            backgroundColor: Colors.red,
          ),
        );
      }
    }
  }

  Future<void> _sendRobotToTable(int tableNumber) async {
    try {
      final apiService = Provider.of<ApiService>(context, listen: false);
      final command = RobotCommand(
        command: 'go_to_table',
        tableNumber: tableNumber,
      );
      await apiService.sendRobotCommand(command);
      
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('Robot sent to table $tableNumber'),
            backgroundColor: Colors.blue,
          ),
        );
      }
    } catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('Failed to send robot: $e'),
            backgroundColor: Colors.red,
          ),
        );
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Admin Dashboard'),
        backgroundColor: Colors.orange,
        foregroundColor: Colors.white,
        bottom: TabBar(
          controller: _tabController,
          labelColor: Colors.white,
          unselectedLabelColor: Colors.white70,
          tabs: const [
            Tab(text: 'Orders', icon: Icon(Icons.list_alt)),
            Tab(text: 'Robot Control', icon: Icon(Icons.smart_toy)),
            Tab(text: 'Bluetooth', icon: Icon(Icons.bluetooth)),
          ],
        ),
        actions: [
          IconButton(
            icon: const Icon(Icons.refresh),
            onPressed: _loadData,
          ),
        ],
      ),
      body: isLoading
          ? const Center(child: CircularProgressIndicator())
          : TabBarView(
              controller: _tabController,
              children: [
                _buildOrdersTab(),
                _buildRobotControlTab(),
                _buildBluetoothTab(),
              ],
            ),
    );
  }

  Widget _buildOrdersTab() {
    final pendingOrders = orders.where((o) => o.status == 'pending').toList();
    final preparingOrders = orders.where((o) => o.status == 'preparing').toList();
    final readyOrders = orders.where((o) => o.status == 'ready').toList();

    return SingleChildScrollView(
      padding: const EdgeInsets.all(16),
      child: Column(
        children: [
          _buildOrderSection('Pending Orders', pendingOrders, Colors.orange),
          const SizedBox(height: 20),
          _buildOrderSection('Preparing', preparingOrders, Colors.blue),
          const SizedBox(height: 20),
          _buildOrderSection('Ready for Delivery', readyOrders, Colors.green),
        ],
      ),
    );
  }

  Widget _buildOrderSection(String title, List<Order> sectionOrders, Color color) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          '$title (${sectionOrders.length})',
          style: TextStyle(
            fontSize: 18,
            fontWeight: FontWeight.bold,
            color: color,
          ),
        ),
        const SizedBox(height: 10),
        if (sectionOrders.isEmpty)
          const Card(
            child: Padding(
              padding: EdgeInsets.all(16),
              child: Text('No orders in this section'),
            ),
          )
        else
          ...sectionOrders.map((order) => _buildOrderCard(order)),
      ],
    );
  }

  Widget _buildOrderCard(Order order) {
    return Card(
      margin: const EdgeInsets.only(bottom: 8),
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Text(
                  'Order #${order.id} - Table ${order.tableId}',
                  style: const TextStyle(fontWeight: FontWeight.bold),
                ),
                Chip(
                  label: Text(order.status.toUpperCase()),
                  backgroundColor: _getStatusColor(order.status),
                ),
              ],
            ),
            const SizedBox(height: 8),
            Text('Total: \$${order.totalAmount.toStringAsFixed(2)}'),
            const SizedBox(height: 8),
            Text('Items: ${order.items.length}'),
            const SizedBox(height: 12),
            Row(
              children: [
                if (order.status == 'pending')
                  ElevatedButton(
                    onPressed: () => _updateOrderStatus(order.id!, 'preparing'),
                    style: ElevatedButton.styleFrom(backgroundColor: Colors.blue),
                    child: const Text('Start Preparing', style: TextStyle(color: Colors.white)),
                  ),
                if (order.status == 'preparing') ...[
                  ElevatedButton(
                    onPressed: () => _updateOrderStatus(order.id!, 'ready'),
                    style: ElevatedButton.styleFrom(backgroundColor: Colors.green),
                    child: const Text('Mark Ready', style: TextStyle(color: Colors.white)),
                  ),
                ],
                if (order.status == 'ready') ...[
                  ElevatedButton(
                    onPressed: () => _sendRobotToTable(order.tableId),
                    style: ElevatedButton.styleFrom(backgroundColor: Colors.purple),
                    child: const Text('Send Robot', style: TextStyle(color: Colors.white)),
                  ),
                  const SizedBox(width: 8),
                  ElevatedButton(
                    onPressed: () => _updateOrderStatus(order.id!, 'delivered'),
                    style: ElevatedButton.styleFrom(backgroundColor: Colors.grey),
                    child: const Text('Mark Delivered', style: TextStyle(color: Colors.white)),
                  ),
                ],
              ],
            ),
          ],
        ),
      ),
    );
  }

  Color _getStatusColor(String status) {
    switch (status) {
      case 'pending':
        return Colors.orange;
      case 'preparing':
        return Colors.blue;
      case 'ready':
        return Colors.green;
      case 'delivered':
        return Colors.grey;
      default:
        return Colors.grey;
    }
  }

  Widget _buildRobotControlTab() {
    return Padding(
      padding: const EdgeInsets.all(16),
      child: Column(
        children: [
          Card(
            child: Padding(
              padding: const EdgeInsets.all(16),
              child: Column(
                children: [
                  const Text(
                    'Robot Status',
                    style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                  ),
                  const SizedBox(height: 16),
                  if (robotStatus != null) ...[
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceBetween,
                      children: [
                        const Text('Connected:'),
                        Icon(
                          robotStatus!.connected ? Icons.check_circle : Icons.error,
                          color: robotStatus!.connected ? Colors.green : Colors.red,
                        ),
                      ],
                    ),
                    const SizedBox(height: 8),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceBetween,
                      children: [
                        const Text('Position:'),
                        Text(robotStatus!.currentPosition),
                      ],
                    ),
                  ] else
                    const Text('Robot status unavailable'),
                ],
              ),
            ),
          ),
          const SizedBox(height: 20),
          Card(
            child: Padding(
              padding: const EdgeInsets.all(16),
              child: Column(
                children: [
                  const Text(
                    'Robot Controls',
                    style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                  ),
                  const SizedBox(height: 16),
                  Row(
                    children: [
                      Expanded(
                        child: ElevatedButton.icon(
                          onPressed: () => _sendRobotCommand('return_home'),
                          icon: const Icon(Icons.home),
                          label: const Text('Return Home'),
                          style: ElevatedButton.styleFrom(
                            backgroundColor: Colors.blue,
                            foregroundColor: Colors.white,
                          ),
                        ),
                      ),
                      const SizedBox(width: 16),
                      Expanded(
                        child: ElevatedButton.icon(
                          onPressed: () => _sendRobotCommand('stop'),
                          icon: const Icon(Icons.stop),
                          label: const Text('Stop'),
                          style: ElevatedButton.styleFrom(
                            backgroundColor: Colors.red,
                            foregroundColor: Colors.white,
                          ),
                        ),
                      ),
                    ],
                  ),
                  const SizedBox(height: 16),
                  const Text('Send to Table:'),
                  const SizedBox(height: 8),
                  Wrap(
                    spacing: 8,
                    children: List.generate(10, (index) {
                      final tableNumber = index + 1;
                      return ElevatedButton(
                        onPressed: () => _sendRobotToTable(tableNumber),
                        child: Text('Table $tableNumber'),
                      );
                    }),
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildBluetoothTab() {
    return Padding(
      padding: const EdgeInsets.all(16),
      child: Column(
        children: [
          Card(
            child: Padding(
              padding: const EdgeInsets.all(16),
              child: Column(
                children: [
                  const Text(
                    'Bluetooth Setup',
                    style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                  ),
                  const SizedBox(height: 16),
                  ElevatedButton.icon(
                    onPressed: () => context.go('/bluetooth'),
                    icon: const Icon(Icons.bluetooth),
                    label: const Text('Configure Bluetooth'),
                    style: ElevatedButton.styleFrom(
                      backgroundColor: Colors.blue,
                      foregroundColor: Colors.white,
                    ),
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }

  Future<void> _sendRobotCommand(String command) async {
    try {
      final apiService = Provider.of<ApiService>(context, listen: false);
      final robotCommand = RobotCommand(command: command);
      await apiService.sendRobotCommand(robotCommand);
      
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('Command sent: $command'),
            backgroundColor: Colors.green,
          ),
        );
      }
    } catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('Failed to send command: $e'),
            backgroundColor: Colors.red,
          ),
        );
      }
    }
  }
}
