import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:go_router/go_router.dart';
import 'screens/home_screen.dart';
import 'screens/customer_menu_screen.dart';
import 'screens/admin_dashboard_screen.dart';
import 'screens/bluetooth_setup_screen.dart';
import 'services/api_service.dart';
import 'services/bluetooth_service.dart';

void main() {
  runApp(const SmartWaiterApp());
}

class SmartWaiterApp extends StatelessWidget {
  const SmartWaiterApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MultiProvider(
      providers: [
        Provider<ApiService>(create: (_) => ApiService()),
        Provider<BluetoothService>(create: (_) => BluetoothService()),
      ],
      child: MaterialApp.router(
        title: 'Smart Waiter Robot',
        theme: ThemeData(
          primarySwatch: Colors.blue,
          visualDensity: VisualDensity.adaptivePlatformDensity,
        ),
        routerConfig: _router,
      ),
    );
  }
}

final GoRouter _router = GoRouter(
  routes: [
    GoRoute(
      path: '/',
      builder: (context, state) => const HomeScreen(),
    ),
    GoRoute(
      path: '/customer/:tableNumber',
      builder: (context, state) {
        final tableNumber = int.parse(state.pathParameters['tableNumber']!);
        return CustomerMenuScreen(tableNumber: tableNumber);
      },
    ),
    GoRoute(
      path: '/admin',
      builder: (context, state) => const AdminDashboardScreen(),
    ),
    GoRoute(
      path: '/bluetooth',
      builder: (context, state) => const BluetoothSetupScreen(),
    ),
  ],
);

