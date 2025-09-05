// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'models.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

Table _$TableFromJson(Map<String, dynamic> json) => Table(
  id: (json['id'] as num).toInt(),
  number: (json['number'] as num).toInt(),
  capacity: (json['capacity'] as num).toInt(),
  status: json['status'] as String,
);

Map<String, dynamic> _$TableToJson(Table instance) => <String, dynamic>{
  'id': instance.id,
  'number': instance.number,
  'capacity': instance.capacity,
  'status': instance.status,
};

MenuItem _$MenuItemFromJson(Map<String, dynamic> json) => MenuItem(
  id: (json['id'] as num).toInt(),
  name: json['name'] as String,
  description: json['description'] as String,
  price: (json['price'] as num).toDouble(),
  category: json['category'] as String,
  available: json['available'] as bool,
  imageUrl: json['imageUrl'] as String?,
);

Map<String, dynamic> _$MenuItemToJson(MenuItem instance) => <String, dynamic>{
  'id': instance.id,
  'name': instance.name,
  'description': instance.description,
  'price': instance.price,
  'category': instance.category,
  'available': instance.available,
  'imageUrl': instance.imageUrl,
};

OrderItem _$OrderItemFromJson(Map<String, dynamic> json) => OrderItem(
  menuItemId: (json['menuItemId'] as num).toInt(),
  quantity: (json['quantity'] as num).toInt(),
  specialInstructions: json['specialInstructions'] as String?,
  menuItem: json['menuItem'] == null
      ? null
      : MenuItem.fromJson(json['menuItem'] as Map<String, dynamic>),
);

Map<String, dynamic> _$OrderItemToJson(OrderItem instance) => <String, dynamic>{
  'menuItemId': instance.menuItemId,
  'quantity': instance.quantity,
  'specialInstructions': instance.specialInstructions,
  'menuItem': instance.menuItem,
};

Order _$OrderFromJson(Map<String, dynamic> json) => Order(
  id: (json['id'] as num?)?.toInt(),
  tableId: (json['tableId'] as num).toInt(),
  items: (json['items'] as List<dynamic>)
      .map((e) => OrderItem.fromJson(e as Map<String, dynamic>))
      .toList(),
  status: json['status'] as String,
  totalAmount: (json['totalAmount'] as num).toDouble(),
  customerName: json['customerName'] as String?,
  specialInstructions: json['specialInstructions'] as String?,
  createdAt: json['createdAt'] == null
      ? null
      : DateTime.parse(json['createdAt'] as String),
);

Map<String, dynamic> _$OrderToJson(Order instance) => <String, dynamic>{
  'id': instance.id,
  'tableId': instance.tableId,
  'items': instance.items,
  'status': instance.status,
  'totalAmount': instance.totalAmount,
  'customerName': instance.customerName,
  'specialInstructions': instance.specialInstructions,
  'createdAt': instance.createdAt?.toIso8601String(),
};

RobotCommand _$RobotCommandFromJson(Map<String, dynamic> json) => RobotCommand(
  command: json['command'] as String,
  tableNumber: (json['tableNumber'] as num?)?.toInt(),
  orderId: (json['orderId'] as num?)?.toInt(),
);

Map<String, dynamic> _$RobotCommandToJson(RobotCommand instance) =>
    <String, dynamic>{
      'command': instance.command,
      'tableNumber': instance.tableNumber,
      'orderId': instance.orderId,
    };

RobotStatus _$RobotStatusFromJson(Map<String, dynamic> json) => RobotStatus(
  connected: json['connected'] as bool,
  currentPosition: json['currentPosition'] as String,
  state: json['state'] as String?,
  currentTable: (json['currentTable'] as num?)?.toInt(),
  targetTable: (json['targetTable'] as num?)?.toInt(),
  isAtHome: json['isAtHome'] as bool?,
);

Map<String, dynamic> _$RobotStatusToJson(RobotStatus instance) =>
    <String, dynamic>{
      'connected': instance.connected,
      'currentPosition': instance.currentPosition,
      'state': instance.state,
      'currentTable': instance.currentTable,
      'targetTable': instance.targetTable,
      'isAtHome': instance.isAtHome,
    };
