#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>


using namespace std;
class Entity {
protected:
    int id;
    string name;

public:
    Entity(int id, const string& name) : id(id), name(name) {}
    virtual ~Entity() {}

    virtual int getId() const { return id; }
    virtual string getName() const { return name; }
    virtual void display() const = 0; 
};

class Product : public Entity {
private:
    double price;
    int stock;
    vector<pair<string,int>> reviews;

public:
    Product(int id, const string& name, double price, int stock)
        : Entity(id,name), price(price), stock(stock) {}
    double getPrice() const { return price; }
    int getStock() const { return stock; }
    
    void setStock(int newStock) { stock = newStock; }
    
    void addReview(const string &username,int rating){
        reviews.push_back({username,rating});}
    
    const vector<pair<string,int>> &getReviews() const {
        return reviews;
    }
    void displayReviews() const {
        if (reviews.empty()) {
            cout << "No reviews yet.\n";
        } else {
            cout << "\n--- Reviews for " << name << " ---\n";
            for (const auto& review : reviews) {
                cout << "User: " << review.first << " | Rating: " << review.second << "\n";
            }
        }
    }

    void display() const override {
        cout << "Product ID: " << id << ", Name: " << name << ", Price: " << price << ", Stock: " << stock << "\n";
    }

    friend ostream& operator<<(ostream& os, const Product& product) {
        os << "ID: " << product.getId() << ", Name: " << product.getName() << ", Price: " << product.getPrice();
        return os;
    }
};
class Promotion {
private:
    int promotionId;
    string description;
    double discountPercentage;
    vector<int> applicableProductIds;

public:
    Promotion(int id, const string& desc, double discount, const vector<int>& productIds)
        : promotionId(id), description(desc), discountPercentage(discount), applicableProductIds(productIds) {}

    double getDiscount() const { return discountPercentage; }
    
    bool isApplicable(int productId) const {
        return find(applicableProductIds.begin(), applicableProductIds.end(), productId) != applicableProductIds.end();
    }

    string getDescription() const { return description; }

    int getId() const { return promotionId; }

    const vector<int>& getApplicableProducts() const { return applicableProductIds; }
};
class Customer {
private:
    string username;
    string password;
    string email;

public:
    Customer(const string& username, const string& password, const string& email)
        : username(username), password(password), email(email) {}

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getEmail() const { return email; }
};

class Cart {
private:
    vector<pair<Product, int>> items; 

public:
    void addProduct(const Product& product, int quantity) {
        for (auto& item : items) {
            if (item.first.getId() == product.getId()) {
                item.second += quantity;
                return;
            }
        }
        items.push_back({product, quantity});
    }

    void removeProduct(int productId) {
        items.erase(
            remove_if(items.begin(), items.end(), [productId](const pair<Product, int>& item) {
                return item.first.getId() == productId;
            }),
            items.end());
    }
    double calculateTotal(const vector<Promotion>& promotions) const {
        double total = 0.0;
        for (const auto& item : items) {
            double discount = 0.0;
            for (const auto& promo : promotions) {
                if (promo.isApplicable(item.first.getId())) {
                    discount = promo.getDiscount();
                    break;
                }
            }
            total += item.first.getPrice() * item.second * (1 - discount / 100);
        }
        return total;
    }
    void viewCart() const {
        cout << "\n--- Cart ---\n";
        for (const auto& item : items) {
            cout << "Product: " << item.first.getName() << " | Quantity: " << item.second
                 << " | Price: " << item.first.getPrice() << "\n";
        }
    }

    const vector<pair<Product, int>>& getItems() const {
        return items;
    }
    void clearCart() { items.clear(); }

};
enum OrderStatus { Pending, Processing, Completed, Canceled }; 

class Order {
private:
    int orderId;
    string customerUsername;
    vector<pair<Product, int>> items;
    double totalAmount;
    OrderStatus status;
    string phoneNumber;
    string address;

public:
    Order(int orderId, const string& username, const vector<pair<Product, int>>& items, double total, const string &phone, const string &addr, OrderStatus status=Pending)
    : orderId(orderId), customerUsername(username), items(items), totalAmount(total), phoneNumber(phone), status(status), address(addr) {}

    int getId() const { return orderId; }
    string getCustomer() const { return customerUsername; }
    double getTotalAmount() const { return totalAmount; }
    OrderStatus getStatus() const { return status; }
    void setStatus(OrderStatus newStatus) { status = newStatus; }
    string getPhoneNumber() const { return phoneNumber; }
    string getAddress() const { return address; }
    void viewOrder() const {
        cout << "\n--- Order Summary ---\n";
        cout << "Order ID: " << orderId << "\n";
        cout << "Customer: " << customerUsername << "\n";
        cout<<"Phone: "<<phoneNumber<<"\n";
        cout<<"Address: "<<address<<"\n";
        cout << "Status: " << (status == Pending ? "Pending" : status == Processing ? "Processing" : status == Completed ? "Completed" : "Canceled") << "\n";
        for (const auto& item : items) {
            cout << "Product: " << item.first.getName() << " | Quantity: " << item.second
                 << " | Price: " << item.first.getPrice() << "\n";
        }
        cout << "Total Amount: " << totalAmount << "\n";
    }
     const vector<pair<Product, int>>& getItems() const {
        return items;
    }
    string getStatusString() const {
        switch (status) {
        case Pending: return "Pending";
        case Processing: return "Processing";
        case Completed: return "Completed";
        case Canceled: return "Canceled";
        default: return "Unknown";
        }
    }
};

void saveOrdersToFile(const vector<Order>& orders, const string& filename) {
    ofstream outFile(filename);
    for (const auto& order : orders) {
        outFile << order.getId() << "," 
                << order.getCustomer() << ","
                << order.getStatusString() << "," 
                << order.getTotalAmount() << ","
                << order.getPhoneNumber() << "," 
                << order.getAddress() << "\n";

        for (const auto& item : order.getItems()) {
            outFile << item.first << "-" << item.second << ";";
        }
        outFile << "\n";
    }
    outFile.close();
}
Product findProductById(int productId, const vector<Product>& products) {
    for (const auto& product : products) {
        if (product.getId() == productId) {
            return product;
        }
    }
    throw runtime_error("Product ID not found");
}
vector<Order> loadOrdersFromFile(const string& filename, const vector<Product>& products) {
    vector<Order> orders;
    ifstream inFile(filename);
    string line;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        int orderId;
        string username, status, phone, address;
        double totalAmount;
        vector<pair<Product, int>> productItems;

        size_t pos = line.find(',');
        orderId = stoi(line.substr(0, pos));
        line.erase(0, pos + 1);

        pos = line.find(',');
        username = line.substr(0, pos);
        line.erase(0, pos + 1);

        pos = line.find(',');
        status = line.substr(0, pos);
        line.erase(0, pos + 1);

        pos = line.find(',');
        totalAmount = stod(line.substr(0, pos));
        line.erase(0, pos + 1);

        pos = line.find(',');
        phone = line.substr(0, pos);
        line.erase(0, pos + 1);

        address = line;

        getline(inFile, line); 
        while ((pos = line.find("ID: ")) != string::npos) {
            line.erase(0, pos + 4);
            pos = line.find(", Name: ");
            int productId = stoi(line.substr(0, pos));
            line.erase(0, pos + 8);

            pos = line.find(", Price: ");
            line.erase(0, pos + 9);

            pos = line.find("-");
            int quantity = stoi(line.substr(pos + 1));
            line.erase(0, line.find(";") + 1);

            Product product = findProductById(productId, products);
            productItems.emplace_back(product, quantity);
        }

        OrderStatus orderStatus = (status == "Pending" ? Pending :
                                   status == "Processing" ? Processing :
                                   status == "Completed" ? Completed : Canceled);

        orders.emplace_back(orderId, username, productItems, totalAmount, phone, address, orderStatus);
    }
    inFile.close();
    return orders;
}



Order* findOrderById(int orderId, vector<Order>& orders) {
    for (auto& order : orders) {
        if (order.getId() == orderId) {
            return &order;
        }
    }
    return nullptr;
}

struct User {
    string username;
    string password;
};

bool login(vector<User>& users, const string& filename, string& loggedInUser) {
    int choice;
    do {
        system("cls"); 
        cout << "\n--- Login Menu ---\n";
        cout << "1. Register\n2. Login\n3. Exit\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            system("cls");
            User newUser;
            cout << "--- Register ---\n";
            cout << "Enter Username: ";
            cin >> newUser.username;
            cout << "Enter Password: ";
            cin >> newUser.password;

            users.push_back(newUser);
            ofstream outFile(filename, ios::app);
            outFile << newUser.username << "," << newUser.password << "\n";
            outFile.close();

            loggedInUser = newUser.username;
            cout << "Registration successful! Please log in.\n";
            system("pause");
        } else if (choice == 2) {
            string username, password;
            do {
                system("cls");
                cout << "--- Login ---\n";
                cout << "Enter Username: ";
                cin >> username;
                cout << "Enter Password: ";
                cin >> password;
                bool isValid = false;
                for (const auto& user : users) {
                    if (user.username == username && user.password == password) {
                        loggedInUser = username;
                        cout << "Login successful! Welcome, " << loggedInUser << ".\n";
                        system("pause");
                        return true; 
                    }
                }

                cout << "Invalid username or password. Please try again.\n";
                system("pause");

            } while (true);

        } else if (choice == 3) {
            cout << "Exiting login menu.\n";
            return false; 
        } else {
            cout << "Invalid choice. Please try again.\n";
            system("pause");
        }
    } while (true); 
}

vector<User> loadUsersFromFile(const string& filename) {
    vector<User> users;
    ifstream inFile(filename);
    string line;
    while (getline(inFile, line)) {
        User user;
        size_t pos = line.find(',');
        user.username = line.substr(0, pos);
        line.erase(0, pos + 1);
        user.password = line;
        users.push_back(user);
    }
    inFile.close();
    return users;
}

void saveUsersToFile(const vector<User>& users, const string& filename) {
    ofstream outFile(filename);
    for (const auto& user : users) {
        outFile << user.username << "," << user.password << "\n";
    }
    outFile.close();
}

unordered_map<string, string> loadAdminCredentials(const string& filename) {
    unordered_map<string, string> credentials;
    ifstream inFile(filename);
    string line;
    while (getline(inFile, line)) {
        size_t pos = line.find(',');
        string username = line.substr(0, pos);
        string password = line.substr(pos + 1);
        credentials[username] = password;
    }
    inFile.close();
    return credentials;
}
void savePromotionsToFile(const vector<Promotion>& promotions, const string& filename) {
    ofstream outFile(filename);
    for (const auto& promo : promotions) {
        outFile << promo.getId() << "," << promo.getDescription() << "," << promo.getDiscount();
        for (const auto& productId : promo.getApplicableProducts()) {
            outFile << "," << productId;
        }
        outFile << "\n";
    }
    outFile.close();
}

vector<Promotion> loadPromotionsFromFile(const string& filename) {
    vector<Promotion> promotions;
    ifstream inFile(filename);
    string line;
    while (getline(inFile, line)) {
        int promoId;
        string description;
        double discount;
        vector<int> productIds;

        size_t pos = 0;
        pos = line.find(',');
        promoId = stoi(line.substr(0, pos));
        line.erase(0, pos + 1);

        pos = line.find(',');
        description = line.substr(0, pos);
        line.erase(0, pos + 1);

        pos = line.find(',');
        discount = stod(line.substr(0, pos));
        line.erase(0, pos + 1);

        while ((pos = line.find(',')) != string::npos) {
            productIds.push_back(stoi(line.substr(0, pos)));
            line.erase(0, pos + 1);
        }
        productIds.push_back(stoi(line));

        promotions.emplace_back(promoId, description, discount, productIds);
    }
    inFile.close();
    return promotions;
}
void saveProductsToFile(const vector<Product>& products, const string& filename) {
    ofstream outFile(filename);
    for (const auto& product : products) {
        outFile << product.getId() << "," << product.getName() << "," << product.getPrice()
                << "," << product.getStock() << "\n";
    }
    outFile.close();
}

void saveReviewsToFile(const vector<Product>& products, const string& filename) {
    ofstream outFile(filename, ios::app); // Open in append mode
    for (const auto& product : products) {
        for (const auto& review : product.getReviews()) {
            outFile << product.getId() << "," << product.getName() << "," << review.first << "," << review.second << "\n";
        }
    }
    outFile.close();
}
vector<Product> loadProductsFromFile(const string& filename) {
    vector<Product> products;
    ifstream inFile(filename);
    string line;
    while (getline(inFile, line)) {
        int id, stock;
        double price;
        string name;
        
        size_t pos = 0;

        pos = line.find(',');
        id = stoi(line.substr(0, pos));
        line.erase(0, pos + 1);

        pos = line.find(',');
        name = line.substr(0, pos);
        line.erase(0, pos + 1);

        pos = line.find(',');
        price = stod(line.substr(0, pos));
        line.erase(0, pos + 1);

        stock = stoi(line);

        products.emplace_back(id, name, price, stock);
    }
    inFile.close();
    return products;
}

void managerMenu(vector<Product>& products, vector<Promotion>& promotions, vector<User>& users, const string& userFile,vector<Order>& orders) {
    const string adminUsername = "nguyentienson0512@gmail.com";
    const string adminPassword = "son05122005";

    cout << "\n--- Admin Login ---\n";
    string username, password;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (username == adminUsername && password == adminPassword) {
        int choice;
        do {
            system("cls");
            cout << "\n--- Admin Menu ---\n";
            cout << "1. Add Product\n2. Edit Product\n3. Delete Product\n4. View Products\n5. Add Promotion\n";
            cout << "6. View Promotions\n7. View Users\n8. Edit Users\n9. Delete Users\n";
            cout<<"10. View All Orders\n11.Update Order Status\n12.Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            system("cls");
            switch (choice) {
                case 1: { 
                    cout << "--- Add Product ---\n";
                    int id, stock;
                    double price;
                    string name;
                    cout << "Enter Product ID: ";
                    cin >> id;
                    cout << "Enter Product Name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter Product Price: ";
                    cin >> price;
                    cout << "Enter Stock Quantity: ";
                    cin >> stock;
                    products.emplace_back(id, name, price, stock);
                    saveProductsToFile(products, "admin.txt");
                    cout << "Product added successfully!\n";
                    system("pause"); 
                    break;
                }
                case 2: { 
                    cout << "--- Edit Product ---\n";
                    int id;
                    cout << "Enter Product ID to Edit: ";
                    cin >> id;
                    for (auto& product : products) {
                        if (product.getId() == id) {
                            double price;
                            int stock;
                            string name;
                            cout << "Enter New Product Name: ";
                            cin.ignore();
                            getline(cin, name);
                            cout << "Enter New Product Price: ";
                            cin >> price;
                            cout << "Enter New Stock Quantity: ";
                            cin >> stock;
                            product = Product(id, name, price, stock);
                            saveProductsToFile(products, "admin.txt");
                            cout << "Product updated successfully.\n";
                            break;
                        }
                    }
                    system("pause");
                    break;
                }
                case 3: { 
                    cout << "--- Delete Product ---\n";
                    int id;
                    cout << "Enter Product ID to Delete: ";
                    cin >> id;
                    products.erase(remove_if(products.begin(), products.end(), [id](const Product& p) {
                        return p.getId() == id;
                    }), products.end());
                    saveProductsToFile(products, "admin.txt");
                    cout << "Product deleted successfully.\n";
                    system("pause");
                    break;
                }
                case 4: { 
                    cout << "--- View Products ---\n";
                    for (const auto& product : products) {
                        cout << "ID: " << product.getId() << " | Name: " << product.getName()
                             << " | Price: " << product.getPrice() << " | Stock: " << product.getStock() << "\n";
                    }
                    system("pause");
                    break;
                }
                case 5: { 
                    cout << "--- Add Promotion ---\n";
                    int promoId;
                    string description;
                    double discount;
                    int productId;
                    cout << "Enter Promotion ID: ";
                    cin >> promoId;
                    cout << "Enter Description: ";
                    cin.ignore();
                    getline(cin, description);
                    cout << "Enter Discount Percentage: ";
                    cin >> discount;
                    cout << "Enter Product ID to Apply Promotion: ";
                    cin >> productId;
                    promotions.emplace_back(promoId, description, discount, vector<int>{productId});
                    savePromotionsToFile(promotions, "admin1.txt");
                    cout << "Promotion added successfully.\n";
                    system("pause");
                    break;
                }
                case 6: { 
                    cout << "--- View Promotions ---\n";
                    promotions = loadPromotionsFromFile("admin1.txt");
                    for (const auto& promo : promotions) {
                        cout << "Promotion ID: " << promo.getId() << " | Description: " << promo.getDescription()
                             << " | Discount: " << promo.getDiscount() << "%\n";
                    }
                    system("pause");
                    break;
                }
                case 7: { 
                    cout << "--- View Users ---\n";
                    for (const auto& user : users) {
                        cout << "Username: " << user.username << "\n";
                    }
                    system("pause");
                    break;
                }
                case 8: { 
                    cout << "--- Edit Users ---\n";
                    string editUsername;
                    cout << "Enter the username to edit: ";
                    cin >> editUsername;
                    auto it = find_if(users.begin(), users.end(), [&](const User& u) {
                        return u.username == editUsername;
                    });
                    if (it != users.end()) {
                        cout << "Enter new password for " << editUsername << ": ";
                        cin >> it->password;
                        saveUsersToFile(users, userFile);
                        cout << "Password updated successfully!\n";
                    } else {
                        cout << "User not found!\n";
                    }
                    system("pause");
                    break;
                }
                case 9: { 
                    cout << "--- Delete Users ---\n";
                    string deleteUsername;
                    cout << "Enter the username to delete: ";
                    cin >> deleteUsername;
                    auto it = remove_if(users.begin(), users.end(), [&](const User& u) {
                        return u.username == deleteUsername;
                    });
                    if (it != users.end()) {
                        users.erase(it, users.end());
                        saveUsersToFile(users, userFile);
                        cout << "User deleted successfully!\n";
                    } else {
                        cout << "User not found!\n";
                    }
                    system("pause");
                    break;
                }
                case 10: {
    cout << "--- All Orders ---\n";
    cout << "Number of orders: " << orders.size() << "\n"; // Debug số lượng đơn hàng
    for (const auto& order : orders) {
        order.viewOrder(); // Hiển thị từng đơn hàng
    }
    system("pause"); // Dừng màn hình để xem kết quả
    break;
}
                case 11: {
                    int orderId;
            cout << "Enter Order ID to update: ";
            cin >> orderId;
            Order* order = findOrderById(orderId, orders);
            if (order) {
                int statusChoice;
                cout << "Select new status:\n1. Pending\n2. Processing\n3. Completed\n4. Canceled\n";
                cout << "Enter your choice: ";
                cin >> statusChoice;
                order->setStatus(static_cast<OrderStatus>(statusChoice - 1));
                cout << "Order status updated successfully!\n";
             saveOrdersToFile(orders, "orders.txt");
            } else {
                cout << "Order not found!\n";
            }
            break;
                }
                case 12: {
                    cout << "Exiting Admin Menu...\n";
                    break;
                }
                default: {
                    cout << "Invalid choice. Please try again.\n";
                    system("pause");
                    break;
                }
            }
        } while (choice != 12);
    } else {
        cout << "Invalid admin credentials!\n";
    }
};

void customerMenu(vector<Product>& products, vector<Promotion>& promotions, vector<Order>& orders) {
    string username;
    cout << "Enter Username: ";
    cin >> username;

    Cart cart;
    int choice;
    do {
        system("cls"); 
        cout << "\n--- Customer Menu ---\n";
        cout << "1. View Products\n2. Add to Cart\n3. Remove from Cart\n4. View Cart\n";
        cout << "5. Checkout\n6. View Order History\n7. Leave a Review\n8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        system("cls");
        switch (choice) {
            case 1: {
                cout << "--- Available Products ---\n";
                for (const auto& product : products) {
                    cout << "ID: " << product.getId() << " | Name: " << product.getName()
                         << " | Price: " << product.getPrice() << " | Stock: " << product.getStock() << "\n";
                }
                system("pause"); 
                break;
            }
            case 2: { 
                cout << "--- Add to Cart ---\n";
                cout << "\n--- Available Products ---\n";
                for (const auto& product : products) {
                    cout << "ID: " << product.getId() << " | Name: " << product.getName()
                         << " | Price: " << product.getPrice() << " | Stock: " << product.getStock() << "\n";
                }
                int id, quantity;
                cout << "Enter Product ID: ";
                cin >> id;
                cout << "Enter Quantity: ";
                cin >> quantity;
                for (auto& product : products) {
                    if (product.getId() == id) {
                        cart.addProduct(product, quantity);
                        cout << "Product added to cart.\n";
                        break;
                    }
                }
                system("pause");
                break;
            }
            case 3: {
                cout << "--- Remove from Cart ---\n";
                int id;
                cout << "Enter Product ID to remove: ";
                cin >> id;
                cart.removeProduct(id);
                cout << "Product removed from cart.\n";
                system("pause");
                break;
            }
            case 4: { 
                cout << "--- View Cart ---\n";
                cart.viewCart();
                system("pause");
                break;
            }
            case 5: { 
    cout << "--- Checkout ---\n";
    double total = cart.calculateTotal(promotions);
    cout << "\nTotal Amount: " << total;

    string phone, address;
    cout << "\nEnter Phone Number: ";
    cin.ignore();
    getline(cin, phone);
    cout << "Enter Shipping Address: ";
    getline(cin, address);

    cout << "\nConfirm Checkout? (y/n): ";
    char confirm;
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        static int orderId = 1; 
        orders.emplace_back(orderId++, username, cart.getItems(), total, phone, address);
        for (const auto& item : cart.getItems()) {
            for (auto& product : products) {
                if (product.getId() == item.first.getId()) {
                    int newStock = product.getStock() - item.second;
                    if (newStock < 0) {
                        cout << "Insufficient stock for product: " << product.getName() << "\n";
                        system("pause");
                        break;
                    }
                    product.setStock(newStock);
                }
            }
        }
        saveProductsToFile(products, "admin.txt");
        saveOrdersToFile(orders, "orders.txt");
        cart.clearCart();
        cout << "Checkout successful!\n";
    }
    system("pause");
    break;
}
            case 6: { 
                cout << "--- Order History ---\n";
                for (const auto& order : orders) {
                    if(order.getCustomer()==username){
                    order.viewOrder();
                }}
                system("pause");
                break;
            }
            case 7: { 
                cout << "--- Leave a Review ---\n";
                int id, rating;
                cout << "Enter Product ID to review: ";
                cin >> id;
                cout << "Enter Rating (1-5): ";
                cin >> rating;
                for (auto& product : products) {
                    if (product.getId() == id) {
                        product.addReview(username, rating);
                        saveReviewsToFile(products, "review.txt");
                        cout << "Review added successfully!\n";
                        break;
                    }
                }
                system("pause");
                break;
            }
            case 8: {
                cout << "Exiting Customer Menu...\n";
                break;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
                system("pause");
                break;
            }
        }
    } while (choice != 8);
}

int main() {
    vector<User> users = loadUsersFromFile("login.txt");
    vector<Product> products = loadProductsFromFile("admin.txt");
    vector<Promotion> promotions; 
vector<Order> orders = loadOrdersFromFile("orders.txt", products);
    string loggedInUser;    
    
    int userType;
    do {
        cout << "\n--- Online Store Menu ---\n";
        cout << "1. Manager\n2. Customer\n3. Exit\n";
        cout << "Enter your choice: ";
        cin >> userType;

        if (userType == 1) {
if (userType == 1) {
    
managerMenu(products, promotions, users, "login.txt", orders);
        saveOrdersToFile(orders, "orders.txt");
}
        } else if (userType == 2) {
            if (!login(users, "login.txt", loggedInUser)) {
                cout << "Exiting program.\n";
            return 0;
          }
            customerMenu(products, promotions, orders);
        }
    } while (userType != 3);

    return 0;
}
