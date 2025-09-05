-- Smart Waiter Robot Database Schema for Supabase

-- Enable RLS (Row Level Security)
ALTER DATABASE postgres SET "app.jwt_secret" TO 'your-jwt-secret-here';

-- Tables table
CREATE TABLE IF NOT EXISTS tables (
    id SERIAL PRIMARY KEY,
    number INTEGER UNIQUE NOT NULL,
    capacity INTEGER NOT NULL DEFAULT 4,
    status VARCHAR(20) DEFAULT 'available' CHECK (status IN ('available', 'occupied', 'reserved')),
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW()
);

-- Menu items table
CREATE TABLE IF NOT EXISTS menu_items (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    description TEXT,
    price DECIMAL(10,2) NOT NULL,
    category VARCHAR(100) NOT NULL,
    available BOOLEAN DEFAULT TRUE,
    image_url TEXT,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW()
);

-- Orders table
CREATE TABLE IF NOT EXISTS orders (
    id SERIAL PRIMARY KEY,
    table_id INTEGER REFERENCES tables(id) ON DELETE CASCADE,
    status VARCHAR(20) DEFAULT 'pending' CHECK (status IN ('pending', 'preparing', 'ready', 'delivered', 'cancelled')),
    total_amount DECIMAL(10,2) NOT NULL,
    customer_name VARCHAR(255),
    special_instructions TEXT,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW()
);

-- Order items table (junction table for orders and menu items)
CREATE TABLE IF NOT EXISTS order_items (
    id SERIAL PRIMARY KEY,
    order_id INTEGER REFERENCES orders(id) ON DELETE CASCADE,
    menu_item_id INTEGER REFERENCES menu_items(id) ON DELETE CASCADE,
    quantity INTEGER NOT NULL DEFAULT 1,
    unit_price DECIMAL(10,2) NOT NULL,
    special_instructions TEXT,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW()
);

-- Robot commands table (for tracking robot movements)
CREATE TABLE IF NOT EXISTS robot_commands (
    id SERIAL PRIMARY KEY,
    command VARCHAR(50) NOT NULL,
    table_number INTEGER,
    order_id INTEGER REFERENCES orders(id) ON DELETE SET NULL,
    status VARCHAR(20) DEFAULT 'pending' CHECK (status IN ('pending', 'executing', 'completed', 'failed')),
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    completed_at TIMESTAMP WITH TIME ZONE
);

-- Create indexes for better performance
CREATE INDEX idx_orders_table_id ON orders(table_id);
CREATE INDEX idx_orders_status ON orders(status);
CREATE INDEX idx_orders_created_at ON orders(created_at);
CREATE INDEX idx_order_items_order_id ON order_items(order_id);
CREATE INDEX idx_order_items_menu_item_id ON order_items(menu_item_id);
CREATE INDEX idx_robot_commands_status ON robot_commands(status);

-- Create updated_at trigger function
CREATE OR REPLACE FUNCTION update_updated_at_column()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = NOW();
    RETURN NEW;
END;
$$ language 'plpgsql';

-- Create triggers for updated_at
CREATE TRIGGER update_tables_updated_at BEFORE UPDATE ON tables
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_menu_items_updated_at BEFORE UPDATE ON menu_items
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_orders_updated_at BEFORE UPDATE ON orders
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

-- Insert sample data
INSERT INTO tables (number, capacity) VALUES 
(1, 2),
(2, 4),
(3, 4),
(4, 6),
(5, 2),
(6, 4),
(7, 8),
(8, 4),
(9, 2),
(10, 6);

INSERT INTO menu_items (name, description, price, category) VALUES 
('Margherita Pizza', 'Classic pizza with tomato sauce, mozzarella, and basil', 12.99, 'Pizza'),
('Chicken Caesar Salad', 'Grilled chicken breast over romaine lettuce with caesar dressing', 10.99, 'Salads'),
('Beef Burger', 'Juicy beef patty with lettuce, tomato, onion, and pickles', 14.99, 'Burgers'),
('Chicken Wings', 'Spicy buffalo wings served with ranch dressing', 8.99, 'Appetizers'),
('Chocolate Cake', 'Rich chocolate cake with chocolate frosting', 6.99, 'Desserts'),
('Coca Cola', 'Classic soft drink', 2.99, 'Beverages'),
('Coffee', 'Freshly brewed coffee', 3.99, 'Beverages'),
('Fish and Chips', 'Beer-battered fish with french fries', 15.99, 'Main Course'),
('Vegetable Pasta', 'Pasta with seasonal vegetables in marinara sauce', 11.99, 'Pasta'),
('Ice Cream Sundae', 'Vanilla ice cream with chocolate sauce and whipped cream', 5.99, 'Desserts');

-- Enable Row Level Security
ALTER TABLE tables ENABLE ROW LEVEL SECURITY;
ALTER TABLE menu_items ENABLE ROW LEVEL SECURITY;
ALTER TABLE orders ENABLE ROW LEVEL SECURITY;
ALTER TABLE order_items ENABLE ROW LEVEL SECURITY;
ALTER TABLE robot_commands ENABLE ROW LEVEL SECURITY;

-- Create policies (allow all for now, customize based on your auth needs)
CREATE POLICY "Allow all operations for tables" ON tables FOR ALL USING (true);
CREATE POLICY "Allow all operations for menu_items" ON menu_items FOR ALL USING (true);
CREATE POLICY "Allow all operations for orders" ON orders FOR ALL USING (true);
CREATE POLICY "Allow all operations for order_items" ON order_items FOR ALL USING (true);
CREATE POLICY "Allow all operations for robot_commands" ON robot_commands FOR ALL USING (true);
