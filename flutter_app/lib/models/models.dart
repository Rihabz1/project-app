import 'package:json_annotation/json_annotation.dart';

part 'models.g.dart';

@JsonSerializable()
class Table {
  final int id;
  final int number;
  final int capacity;
  final String status;

  Table({
    required this.id,
    required this.number,
    required this.capacity,
    required this.status,
  });

  factory Table.fromJson(Map<String, dynamic> json) => _$TableFromJson(json);
  Map<String, dynamic> toJson() => _$TableToJson(this);
}

@JsonSerializable()
class MenuItem {
  final int id;
  final String name;
  final String description;
  final double price;
  final String category;
  final bool available;
  final String? imageUrl;

  MenuItem({
    required this.id,
    required this.name,
    required this.description,
    required this.price,
    required this.category,
    required this.available,
    this.imageUrl,
  });

  factory MenuItem.fromJson(Map<String, dynamic> json) => _$MenuItemFromJson(json);
  Map<String, dynamic> toJson() => _$MenuItemToJson(this);
}

@JsonSerializable()
class OrderItem {
  final int menuItemId;
  final int quantity;
  final String? specialInstructions;
  final MenuItem? menuItem;

  OrderItem({
    required this.menuItemId,
    required this.quantity,
    this.specialInstructions,
    this.menuItem,
  });

  factory OrderItem.fromJson(Map<String, dynamic> json) => _$OrderItemFromJson(json);
  Map<String, dynamic> toJson() => _$OrderItemToJson(this);
}

@JsonSerializable()
class Order {
  final int? id;
  final int tableId;
  final List<OrderItem> items;
  final String status;
  final double totalAmount;
  final String? customerName;
  final String? specialInstructions;
  final DateTime? createdAt;

  Order({
    this.id,
    required this.tableId,
    required this.items,
    required this.status,
    required this.totalAmount,
    this.customerName,
    this.specialInstructions,
    this.createdAt,
  });

  factory Order.fromJson(Map<String, dynamic> json) => _$OrderFromJson(json);
  Map<String, dynamic> toJson() => _$OrderToJson(this);
}

@JsonSerializable()
class RobotCommand {
  final String command;
  final int? tableNumber;
  final int? orderId;

  RobotCommand({
    required this.command,
    this.tableNumber,
    this.orderId,
  });

  factory RobotCommand.fromJson(Map<String, dynamic> json) => _$RobotCommandFromJson(json);
  Map<String, dynamic> toJson() => _$RobotCommandToJson(this);
}

@JsonSerializable()
class RobotStatus {
  final bool connected;
  final String currentPosition;
  final String? state;
  final int? currentTable;
  final int? targetTable;
  final bool? isAtHome;

  RobotStatus({
    required this.connected,
    required this.currentPosition,
    this.state,
    this.currentTable,
    this.targetTable,
    this.isAtHome,
  });

  factory RobotStatus.fromJson(Map<String, dynamic> json) => _$RobotStatusFromJson(json);
  Map<String, dynamic> toJson() => _$RobotStatusToJson(this);
}
