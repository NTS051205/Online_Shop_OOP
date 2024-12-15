# Simulate_an_online_store.
# README

## Overview
This program is a basic simulation of an e-commerce management system. It provides functionality for both **administrators** (managers) and **customers**, allowing them to interact with products, promotions, user accounts, and orders. The system is designed with a clear menu-driven interface and includes features like login, product management, cart functionality, and user reviews.

---

## Features

### **1. Admin Features**
Administrators have access to advanced features for managing the platform, such as:

- **Add Product:** Add new products to the inventory with details like ID, name, price, and stock.
- **Edit Product:** Modify details of existing products, including price and stock levels.
- **Delete Product:** Remove a product from the inventory.
- **View Products:** Display all available products with their details.
- **Add Promotion:** Create and apply discounts to specific products.
- **View Promotions:** List all current promotions and their applicable products.
- **Manage Users:**
  - View registered users.
  - Edit user details (e.g., reset passwords).
  - Delete user accounts.
- **Manage Orders:**
  - View all orders and their statuses.
  - Update order statuses to "Processing," "Completed," or "Canceled."

### **2. Customer Features**
Customers can interact with products and manage their orders through:

- **View Products:** Browse available products and their details.
- **Add to Cart:** Select and add products to their shopping cart.
- **Remove from Cart:** Remove items from their cart.
- **View Cart:** Display all items in the cart with quantities and prices.
- **Checkout:** Purchase items in the cart, reducing inventory stock.
- **View Order History:** Review details of previous purchases.
- **Leave a Review:** Provide a rating for purchased products.

### **3. Login and Registration**
- **Registration:** Allows new users to create an account by entering a username and password.
- **Login:** Users must log in with valid credentials to access customer features. Invalid attempts trigger a retry with an appropriate error message.

---

## Recent Updates

### **Stock Management in Checkout**
- The program now ensures that product stock is updated after a successful checkout.
- During checkout, if the quantity purchased exceeds available stock, an error message is displayed, and the checkout process is halted for that product.
- Inventory changes are saved back to the file to ensure data persistence.

### **Enhanced Order Processing**
- Administrators can now view detailed order summaries and update order statuses to reflect current progress.
- Orders are stored in a structured format for easy retrieval and updates.

---

## Technical Details

### **Core Classes and Structures**
1. **`Product`**: Represents items in the inventory with attributes like ID, name, price, stock, and reviews.
2. **`Promotion`**: Handles discounts and their applicability to specific products.
3. **`Customer`**: Represents customer accounts with usernames, passwords, and emails.
4. **`Cart`**: Manages items added by the customer, with functionality for adding, removing, and calculating totals.
5. **`Order`**: Stores order details, including customer name, purchased items, and total cost. Tracks the order status (Pending, Processing, Completed, or Canceled).
6. **`User`**: Simple structure to manage user accounts for login and registration.

### **Files Used**
- **`login.txt`**: Stores user account credentials.
- **`admin.txt`**: Stores product data.
- **`admin1.txt`**: Stores promotion details.
- **`review.txt`**: Stores product reviews.
- **`orders.txt`**: Stores order details, including product lists and customer information.

### **Error Handling**
- Invalid login credentials prompt the user to retry.
- Menu choices are validated to ensure the input is within expected options.
- Appropriate messages are displayed for empty carts, invalid inputs, or insufficient stock during checkout.

---

## How to Run

### Prerequisites
1. A C++ compiler (e.g., GCC).
2. Basic understanding of file input/output in C++.
3. A terminal or command-line interface.

### Steps
1. **Compile the Program:**
   ```bash
   g++ nhom4_6.cpp -o nhom4_6
   ```
2. **Run the Program:**
   ```bash
   ./nhom4_6
   ```
3. **Follow the On-Screen Menu:**
   - Choose between Manager or Customer mode.
   - Interact with the system using the menu options provided.

---

## Sample Execution

### Login as Admin
```plaintext
--- Admin Login ---
Username: admin
Password: ****

--- Admin Menu ---
1. Add Product
2. Edit Product
...
Enter your choice: 1

--- Add Product ---
Enter Product ID: 101
Enter Product Name: Laptop
Enter Product Price: 1000.99
Enter Stock Quantity: 50
Product added successfully!
```

### Customer Interaction
```plaintext
--- Customer Menu ---
1. View Products
2. Add to Cart
...
Enter your choice: 2

--- Add to Cart ---
Enter Product ID: 101
Enter Quantity: 2
Product added to cart.
```

### Checkout with Stock Update
```plaintext
--- Checkout ---
Product: Laptop | Quantity: 2 | Stock Remaining: 48
Checkout successful!
```

---

## Known Limitations
1. **File Persistence:** The program overwrites certain files on every execution. Enhancements could include using a database.
2. **Concurrency:** The program is not designed for simultaneous access by multiple users.
3. **User Authentication:** Passwords are stored in plain text; consider encrypting them for better security.

---

## Future Enhancements
- Implement a graphical user interface (GUI) for better user experience.
- Use a database system (e.g., MySQL) for data storage.
- Add features like order tracking and email notifications.
- Encrypt user passwords for enhanced security.

---

## Author
This project was developed as part of a learning exercise in building C++ console applications. Feedback and contributions are welcome!

