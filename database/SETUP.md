# Supabase Setup Instructions

Follow these steps to set up your Supabase database for the Smart Waiter Robot system.

## 1. Create Supabase Project

1. Go to [supabase.com](https://supabase.com) and sign up/login
2. Click "New Project"
3. Choose your organization
4. Enter project name: "smart-waiter-robot"
5. Enter a strong database password
6. Select your region
7. Click "Create new project"

## 2. Get Your Credentials

Once your project is created:

1. Go to Settings → API
2. Copy the following values:
   - **Project URL** (SUPABASE_URL)
   - **anon/public key** (SUPABASE_ANON_KEY)
   - **service_role key** (SUPABASE_SERVICE_KEY)

## 3. Run Database Schema

1. Go to SQL Editor in your Supabase dashboard
2. Copy the contents of `schema.sql` from this project
3. Paste it into the SQL editor
4. Click "Run" to execute the schema

## 4. Configure Row Level Security (Optional)

The schema includes basic RLS policies that allow all operations. For production, you may want to:

1. Create user authentication
2. Implement proper RLS policies based on user roles
3. Restrict admin operations to authenticated admin users

## 5. Test Your Setup

You can test your database by:

1. Going to Table Editor in Supabase
2. Checking that all tables were created
3. Verifying sample data was inserted
4. Testing API calls from your backend

## Environment Variables

Add these to your FastAPI backend `.env` file:

```env
SUPABASE_URL=https://your-project-ref.supabase.co
SUPABASE_ANON_KEY=your-anon-key-here
SUPABASE_SERVICE_KEY=your-service-key-here
```

## Sample Data

The schema includes sample data for:
- 10 restaurant tables
- 10 menu items across different categories
- Proper table relationships

You can modify this sample data as needed for your restaurant.
