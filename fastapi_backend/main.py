from fastapi import FastAPI, HTTPException, Depends
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from typing import List, Optional
import os
from dotenv import load_dotenv
from supabase import create_client, Client
import asyncio
from datetime import datetime

# Load environment variables
load_dotenv()

app = FastAPI(title="Smart Waiter Robot API", version="1.0.0")

# CORS middleware for Flutter app
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # Configure properly for production
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Supabase client
supabase: Client = create_client(
    os.getenv("SUPABASE_URL"),
    os.getenv("SUPABASE_ANON_KEY")
)

# Pydantic models
class Table(BaseModel):
    id: int
    number: int
    capacity: int
    status: str = "available"  # available, occupied, reserved

class MenuItem(BaseModel):
    id: int
    name: str
    description: str
    price: float
    category: str
    available: bool = True

class OrderItem(BaseModel):
    menu_item_id: int
    quantity: int
    special_instructions: Optional[str] = None

class Order(BaseModel):
    id: Optional[int] = None
    table_id: int
    items: List[OrderItem]
    status: str = "pending"  # pending, preparing, ready, delivered
    total_amount: float
    created_at: Optional[datetime] = None

class RobotCommand(BaseModel):
    command: str  # "go_to_table", "return_home", "stop"
    table_number: Optional[int] = None
    order_id: Optional[int] = None

# Bluetooth robot communication (placeholder)
class RobotController:
    def __init__(self):
        self.connected = False
        self.current_position = "home"
    
    async def send_command(self, command: RobotCommand):
        """Send command to robot via Bluetooth"""
        try:
            # This would be replaced with actual Bluetooth communication
            print(f"Sending command to robot: {command.dict()}")
            
            if command.command == "go_to_table":
                # Simulate robot movement
                await asyncio.sleep(1)  # Simulate time to reach table
                self.current_position = f"table_{command.table_number}"
                return {"status": "success", "message": f"Robot moving to table {command.table_number}"}
            
            elif command.command == "return_home":
                await asyncio.sleep(1)
                self.current_position = "home"
                return {"status": "success", "message": "Robot returning home"}
            
            elif command.command == "stop":
                return {"status": "success", "message": "Robot stopped"}
            
        except Exception as e:
            return {"status": "error", "message": str(e)}

robot_controller = RobotController()

# API Routes

@app.get("/")
async def root():
    return {"message": "Smart Waiter Robot API"}

@app.get("/tables", response_model=List[Table])
async def get_tables():
    """Get all tables"""
    try:
        response = supabase.table("tables").select("*").execute()
        return response.data
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/tables", response_model=Table)
async def create_table(table: Table):
    """Create a new table"""
    try:
        response = supabase.table("tables").insert(table.dict()).execute()
        return response.data[0]
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/menu", response_model=List[MenuItem])
async def get_menu():
    """Get all menu items"""
    try:
        response = supabase.table("menu_items").select("*").execute()
        return response.data
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/menu", response_model=MenuItem)
async def create_menu_item(item: MenuItem):
    """Create a new menu item"""
    try:
        response = supabase.table("menu_items").insert(item.dict()).execute()
        return response.data[0]
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/orders", response_model=Order)
async def create_order(order: Order):
    """Create a new order"""
    try:
        order_data = order.dict()
        order_data["created_at"] = datetime.now().isoformat()
        
        response = supabase.table("orders").insert(order_data).execute()
        return response.data[0]
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/orders", response_model=List[Order])
async def get_orders(status: Optional[str] = None):
    """Get all orders, optionally filtered by status"""
    try:
        query = supabase.table("orders").select("*")
        if status:
            query = query.eq("status", status)
        
        response = query.execute()
        return response.data
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/orders/{order_id}", response_model=Order)
async def get_order(order_id: int):
    """Get a specific order"""
    try:
        response = supabase.table("orders").select("*").eq("id", order_id).execute()
        if not response.data:
            raise HTTPException(status_code=404, detail="Order not found")
        return response.data[0]
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.put("/orders/{order_id}/status")
async def update_order_status(order_id: int, status: str):
    """Update order status"""
    try:
        response = supabase.table("orders").update({"status": status}).eq("id", order_id).execute()
        
        # If order is ready, notify robot to deliver
        if status == "ready":
            order = response.data[0]
            table_response = supabase.table("tables").select("number").eq("id", order["table_id"]).execute()
            table_number = table_response.data[0]["number"]
            
            command = RobotCommand(
                command="go_to_table",
                table_number=table_number,
                order_id=order_id
            )
            robot_result = await robot_controller.send_command(command)
            
            return {
                "order": response.data[0],
                "robot_command": robot_result
            }
        
        return response.data[0]
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/robot/command")
async def send_robot_command(command: RobotCommand):
    """Send command to robot"""
    try:
        result = await robot_controller.send_command(command)
        return result
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/robot/status")
async def get_robot_status():
    """Get current robot status"""
    return {
        "connected": robot_controller.connected,
        "current_position": robot_controller.current_position
    }

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
