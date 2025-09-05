import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../models/models.dart';
import '../services/api_service.dart';

class CustomerMenuScreen extends StatefulWidget {
  final int tableNumber;

  const CustomerMenuScreen({super.key, required this.tableNumber});

  @override
  State<CustomerMenuScreen> createState() => _CustomerMenuScreenState();
}

class _CustomerMenuScreenState extends State<CustomerMenuScreen> {
  List<MenuItem> menuItems = [];
  List<OrderItem> cart = [];
  bool isLoading = true;
  String? error;

  @override
  void initState() {
    super.initState();
    _loadMenu();
  }

  Future<void> _loadMenu() async {
    try {
      final apiService = Provider.of<ApiService>(context, listen: false);
      final items = await apiService.getMenu();
      setState(() {
        menuItems = items;
        isLoading = false;
      });
    } catch (e) {
      setState(() {
        error = e.toString();
        isLoading = false;
      });
    }
  }

  void _addToCart(MenuItem item) {
    setState(() {
      final existingItemIndex = cart.indexWhere((cartItem) => cartItem.menuItemId == item.id);
      if (existingItemIndex >= 0) {
        final existingItem = cart[existingItemIndex];
        cart[existingItemIndex] = OrderItem(
          menuItemId: item.id,
          quantity: existingItem.quantity + 1,
          menuItem: item,
        );
      } else {
        cart.add(OrderItem(
          menuItemId: item.id,
          quantity: 1,
          menuItem: item,
        ));
      }
    });
  }

  void _removeFromCart(int menuItemId) {
    setState(() {
      final existingItemIndex = cart.indexWhere((cartItem) => cartItem.menuItemId == menuItemId);
      if (existingItemIndex >= 0) {
        final existingItem = cart[existingItemIndex];
        if (existingItem.quantity > 1) {
          cart[existingItemIndex] = OrderItem(
            menuItemId: existingItem.menuItemId,
            quantity: existingItem.quantity - 1,
            menuItem: existingItem.menuItem,
          );
        } else {
          cart.removeAt(existingItemIndex);
        }
      }
    });
  }

  double get totalAmount {
    return cart.fold(0.0, (total, item) => total + (item.menuItem!.price * item.quantity));
  }

  Future<void> _placeOrder() async {
    if (cart.isEmpty) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Please add items to your cart first')),
      );
      return;
    }

    try {
      final apiService = Provider.of<ApiService>(context, listen: false);
      final order = Order(
        tableId: widget.tableNumber,
        items: cart,
        status: 'pending',
        totalAmount: totalAmount,
      );

      await apiService.createOrder(order);
      
      setState(() {
        cart.clear();
      });

      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(
            content: Text('Order placed successfully!'),
            backgroundColor: Colors.green,
          ),
        );
      }
    } catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('Failed to place order: $e'),
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
        title: Text('Table ${widget.tableNumber} - Menu'),
        backgroundColor: Colors.green,
        foregroundColor: Colors.white,
        actions: [
          Stack(
            children: [
              IconButton(
                icon: const Icon(Icons.shopping_cart),
                onPressed: () => _showCart(context),
              ),
              if (cart.isNotEmpty)
                Positioned(
                  right: 8,
                  top: 8,
                  child: Container(
                    padding: const EdgeInsets.all(2),
                    decoration: BoxDecoration(
                      color: Colors.red,
                      borderRadius: BorderRadius.circular(10),
                    ),
                    constraints: const BoxConstraints(
                      minWidth: 16,
                      minHeight: 16,
                    ),
                    child: Text(
                      '${cart.length}',
                      style: const TextStyle(
                        color: Colors.white,
                        fontSize: 12,
                      ),
                      textAlign: TextAlign.center,
                    ),
                  ),
                ),
            ],
          ),
        ],
      ),
      body: isLoading
          ? const Center(child: CircularProgressIndicator())
          : error != null
              ? Center(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Text('Error: $error'),
                      ElevatedButton(
                        onPressed: _loadMenu,
                        child: const Text('Retry'),
                      ),
                    ],
                  ),
                )
              : _buildMenuList(),
    );
  }

  Widget _buildMenuList() {
    final categories = menuItems.map((item) => item.category).toSet().toList();
    
    return ListView.builder(
      itemCount: categories.length,
      itemBuilder: (context, index) {
        final category = categories[index];
        final categoryItems = menuItems.where((item) => item.category == category).toList();
        
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Padding(
              padding: const EdgeInsets.all(16.0),
              child: Text(
                category,
                style: const TextStyle(
                  fontSize: 20,
                  fontWeight: FontWeight.bold,
                  color: Colors.green,
                ),
              ),
            ),
            ...categoryItems.map((item) => _buildMenuItem(item)),
          ],
        );
      },
    );
  }

  Widget _buildMenuItem(MenuItem item) {
    final cartItem = cart.firstWhere(
      (cartItem) => cartItem.menuItemId == item.id,
      orElse: () => OrderItem(menuItemId: item.id, quantity: 0),
    );

    return Card(
      margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 4),
      child: ListTile(
        leading: const CircleAvatar(
          backgroundColor: Colors.green,
          child: Icon(Icons.restaurant, color: Colors.white),
        ),
        title: Text(item.name),
        subtitle: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(item.description),
            const SizedBox(height: 4),
            Text(
              '\$${item.price.toStringAsFixed(2)}',
              style: const TextStyle(
                fontWeight: FontWeight.bold,
                color: Colors.green,
              ),
            ),
          ],
        ),
        trailing: Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            if (cartItem.quantity > 0) ...[
              IconButton(
                icon: const Icon(Icons.remove),
                onPressed: () => _removeFromCart(item.id),
              ),
              Text('${cartItem.quantity}'),
            ],
            IconButton(
              icon: const Icon(Icons.add),
              onPressed: () => _addToCart(item),
            ),
          ],
        ),
      ),
    );
  }

  void _showCart(BuildContext context) {
    showModalBottomSheet(
      context: context,
      isScrollControlled: true,
      builder: (context) => Container(
        height: MediaQuery.of(context).size.height * 0.7,
        padding: const EdgeInsets.all(16),
        child: Column(
          children: [
            const Text(
              'Your Order',
              style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
            ),
            const Divider(),
            Expanded(
              child: cart.isEmpty
                  ? const Center(child: Text('Your cart is empty'))
                  : ListView.builder(
                      itemCount: cart.length,
                      itemBuilder: (context, index) {
                        final item = cart[index];
                        return ListTile(
                          title: Text(item.menuItem!.name),
                          subtitle: Text('\$${item.menuItem!.price.toStringAsFixed(2)} x ${item.quantity}'),
                          trailing: Text('\$${(item.menuItem!.price * item.quantity).toStringAsFixed(2)}'),
                        );
                      },
                    ),
            ),
            const Divider(),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                const Text(
                  'Total:',
                  style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                ),
                Text(
                  '\$${totalAmount.toStringAsFixed(2)}',
                  style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                ),
              ],
            ),
            const SizedBox(height: 16),
            SizedBox(
              width: double.infinity,
              child: ElevatedButton(
                onPressed: cart.isEmpty ? null : _placeOrder,
                style: ElevatedButton.styleFrom(
                  backgroundColor: Colors.green,
                  foregroundColor: Colors.white,
                  padding: const EdgeInsets.symmetric(vertical: 16),
                ),
                child: const Text('Place Order'),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
