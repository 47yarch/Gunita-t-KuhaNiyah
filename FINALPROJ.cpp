#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"


void displayLogo() {
    cout << CYAN << "\tGunita't KuhanIyah\n";
    cout << CYAN << " .----------------.  .----------------. \n";
    cout << "| .--------------. || .--------------. |\n";
    cout << "| |    " << YELLOW << "______" << CYAN << "    | || |  " << GREEN << "___  ____" << CYAN << "   | |\n";
    cout << "| |  .' " << YELLOW << "___  " << CYAN << "|   | || | |_  ||_  _|  | |\n";
    cout << "| | / .'   \\_|   | || |   | |_/ /    | |\n";
    cout << "| | | |    ____  | || |   |  __'.    | |\n";
    cout << "| | \\ `.___]  _| | || |  _| |  \\ \\_  | |\n";
    cout << "| |  `._____.'   | || | |____||____| | |\n";
    cout << "| |              | || |              | |\n";
    cout << "| '--------------' || '--------------' |\n";
    cout << " '----------------'  '----------------' " << RESET << endl;
    cout << CYAN << "    Mga kuhang binubuhay ng gunita\n" << RESET;
}

void saveStock(int stock[], int productCount) {
    ofstream out("GKiya_Stock.txt");
    for (int i = 0; i < productCount; i++)
    out << stock[i] << endl;
    out.close();
}

void loadStock(int stock[], int productCount) {
    ifstream in("GKiya_Stock.txt");
    if (!in) return;
    for (int i = 0; i < productCount; i++)
        in >> stock[i];
}

void restockAll(int stock[], int productCount) {
    for (int i = 0; i < productCount; i++) {
        stock[i] = 50;
    }
    saveStock(stock, productCount);
    cout << GREEN << "All items have been restocked!\n" << RESET;
}

void viewReceipt() {
    ifstream in("Gkiya_Receipt.txt");
    if (!in) { cout << RED << "No receipt file found!\n" << RESET; return; }
    string line;
    while (getline(in, line)) cout << line << endl;
    in.close();
}

void viewStockFile(string productName[], int productID[], int stock[], int productCount) {
    cout << BLUE << "\n========= PRODUCT STOCK LIST =========\n" << RESET;
    cout << left << setw(6) << "ID" << setw(20) << "Name" << "Stock\n";

    for (int i = 0; i < productCount; i++) {
        cout << left << setw(6) << productID[i]<< setw(20) << productName[i]<< stock[i] << "/50" << endl;
    }
}

bool adminLogin() {
    string username, password;
    int attempts = 3;
    while (attempts > 0) {
        cout << MAGENTA << "\n+=========== ADMIN LOGIN ===========+\n" << RESET;
        cout << "Username: "; cin >> username;
        cout << "Password: "; cin >> password;
        cout << MAGENTA << "\n+===================================+\n" << RESET;

        if (username == "gkiya" && password == "admin123") {
            cout << GREEN << "Access Granted!\n" << RESET;
            return true;
        } else {
            attempts--;
            cout << RED << "Invalid credentials! Attempts left: " << attempts << RESET << endl;
        }
    }
    cout << RED << "Too many failed attempts. Returning to main menu.\n" << RESET;
    return false;
}

int getTierDiscount(int count) {
	if (count >= 10) return 10; //dia
	if (count >= 5) return 5; //gold
	if (count >= 3) return 3; //sil
return 0;
}

string getTierName(int count) {
	if (count >= 10) return "DIAMOND";
	if (count >= 5) return "GOLD";
	if (count >= 3) return "SILVER";
return "NONE";
}

int getNextTierRequirement(int count) {
	if (count < 3) return 3 - count; 
	if (count < 5) return 5 - count; 
	if (count < 10) return 10 - count; 
return 0;
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

int getPurchaseCount(const string& customerName) {
    ifstream in("Gkiya_Loyalty.txt");
    if (!in) return 0;

    string line;
    while (getline(in, line)) {
        if (line.find('-') == string::npos) continue;

        string name = trim(line.substr(0, line.find('-')));
        string countStr = trim(line.substr(line.find('-') + 1));

        if (name == customerName) return stoi(countStr);
    }
    return 0;
}

void updatePurchaseCount(const string& customerName) {
    string names[1000];
    int counts[1000];
    int size = 0;

    ifstream in("Gkiya_Loyalty.txt");
    string line;
    bool found = false;

    while (getline(in, line)) {
        if (line.find('-') == string::npos) continue;

        string name = trim(line.substr(0, line.find('-')));
        string countStr = trim(line.substr(line.find('-') + 1));
        if (name == "" || countStr == "") continue;
        int count = stoi(countStr);

        if (name == customerName) {
		count++; 
		found = true;}

        names[size] = name; counts[size] = count; size++;
    }
    in.close();

    if (!found) names[size]= customerName; counts[size] =1; size++;     

    ofstream out("Gkiya_Loyalty.txt");
    for (int i = 0; i < size; i++) { out << names[i] << "-" << counts[i] << endl; }
    out.close();
}

void saveReceipt(int cartID[], string cartName[], double cartPrice[], int cartQty[], int cartCount,double discountPercent, double totalPaid, string customerName) {
    ofstream out("Gkiya_Receipt.txt", ios::app); 

    int purchaseCount = getPurchaseCount(customerName);
    string tier = getTierName(purchaseCount);
    int nextTierNeed = getNextTierRequirement(purchaseCount);

    double subtotal = 0;

    out << "+======================= RECEIPT =======================+\n";
    out << "Customer Name: " << customerName << "\n";
    out << "Tier: " << tier << "\n";
    out << "Purchase Count: " << purchaseCount << "\n";
    out << "Next Tier in: " << nextTierNeed << " purchase(s)\n";
    out << "--------------------------------------------------------\n";
    out << left << setw(5) << "No." << setw(20) << "Item" << setw(8) << "Qty" << setw(10) << "Price" << "Total" << endl;
    out << "--------------------------------------------------------\n";

    for (int i = 0; i < cartCount; i++) {
        double totalItem = cartPrice[i] * cartQty[i];
        out << left << setw(5) << (i + 1) << setw(20) << cartName[i] << setw(8) << cartQty[i] << setw(10) << cartPrice[i] << totalItem << endl;
        subtotal += totalItem;
    }

    double discountAmount = subtotal * (discountPercent / 100.0);
    double totalDue = subtotal - discountAmount;
    double change = totalPaid - totalDue;

    out << "--------------------------------------------------------\n";
    out << left << setw(33) << "SUBTOTAL:" << subtotal << endl;
    out << left << setw(33) << "DISCOUNT:" << "-" << discountAmount << endl;
    out << left << setw(33) << "TOTAL DUE:" << totalDue << endl;
    out << left << setw(33) << "AMOUNT PAID:" << totalPaid << endl;
    out << left << setw(33) << "CHANGE DUE:" << change << endl;
    out << "+=======================================================+\n\n";
}

void viewCustomerLoyalty() {
    ifstream in("Gkiya_Loyalty.txt");
    if (!in) { cout << RED << "No loyalty data found!\n" << RESET; return; }

    cout << CYAN << "\n+=========== CUSTOMER LOYALTY LIST ===========+\n" << RESET;

    string line;
    while (getline(in, line)) { cout << line << endl; }
    in.close();
}

int main() {
    string productName[50] = {"Frame 4x6","Frame 5x7","Frame 8x10","GlowFrame","Collage Frame", "Instax Mini 12","Instax Mini 90","Instax SQ1","Film 10s","Film 20s"};
    double price[50] = {50,75,120,300,450,4200,6800,5300,350,650};
    int productID[50] = {101,102,103,104,105,201,202,203,301,302};
    int stock[50] = {20,15,10,8,12,5,4,6,30,20};
    int productCount = 10;
    
    ifstream in("GKiya_Stock.txt");
    if (!in) { restockAll(stock, productCount); } 
	else { loadStock(stock, productCount); }
    in.close();
    
    int cartID[50], cartQty[50], cartCount = 0;
    string cartName[50];
    double cartPrice[50];
    string customerName;
    
    char continueShopping = 'Y';
    while (continueShopping == 'Y' || continueShopping == 'y') {
        displayLogo();
        
        int choice;
        cout << YELLOW << "+========= MENU =========+\n" << RESET;
        cout << "[1] View Products" <<endl;
		cout << "[2] Add to Cart" <<endl;
		cout << "[3] View Cart" <<endl;
		cout << "[4] Delete Item"<<endl;
		cout << "[5] Checkout" <<endl; 
		cout << "[6] Admin" <<endl; 
		cout << "[7] Exit\n";
		cout << YELLOW << "+=======================+\n" << RESET;
        cout << "Enter choice: "; 
		cin >> choice;

        if (choice == 1) {
            cout << BLUE << "\n+========== PRODUCTS ==========+\n" << RESET;
            cout << left << setw(6) << "ID" << setw(20) << "Name" << setw(10) << "Price" << "Stock\n";
            for (int i=0;i<productCount;i++)
                cout << left << setw(6) << productID[i] << setw(20) << productName[i] << setw(10) << price[i] << stock[i] << endl;
        } 
        else if (choice == 2) {
    		cout << BLUE << "\n+========== PRODUCTS ==========+\n" << RESET;
    		cout << left << setw(6) << "ID" << setw(20) << "Name" << setw(10) << "Price" << "Stock\n";
    		for (int i = 0; i < productCount; i++)
        		cout << left << setw(6) << productID[i] << setw(20) << productName[i] << setw(10) << price[i] << stock[i] << endl;
    		int id, qty;
		    cout << "Enter Product ID: "; cin >> id;
		    if(cin.fail()) { cin.clear(); cin.ignore(10000, '\n');
    		cout << "Invalid input. Please enter a number.\n";
    		continue;}
    		
    		cout << "Enter Quantity: "; cin >> qty;
			int index = -1;
    		for (int i = 0; i < productCount; i++) if (productID[i] == id) index = i;

    		if (index == -1) { cout << RED << "Product not found!\n" << RESET; continue;}
    		if (qty > stock[index]) { cout << RED << "Not enough stock!\n" << RESET; continue;}

    		stock[index] -= qty;                  
    		saveStock(stock, productCount);       

    		bool foundInCart = false;
			for (int i=0; i<cartCount; i++) {
    			if (cartID[i] == id) {cartQty[i] += qty; foundInCart = true; break;}
			} if (!foundInCart) {
    			cartID[cartCount] = productID[index]; cartName[cartCount] = productName[index];
    			cartPrice[cartCount] = price[index]; cartQty[cartCount] = qty;
    			cartCount++; }
	    	cout << GREEN << "Added to cart!\n" << RESET;
			}
        else if (choice == 3) {
            cout << MAGENTA << "\n========YOUR CART=========\n" << RESET;
            for (int i=0;i<cartCount;i++)
                cout << i+1 << ". " << cartName[i] << " x" << cartQty[i] << " = Php " << cartPrice[i]*cartQty[i] << endl;
        }
        else if (choice == 4) {
    	int id; 
    	cout << "Enter Product ID to delete: "; cin >> id; 
		    if(cin.fail()) { cin.clear(); cin.ignore(10000, '\n');
    		cout << "Invalid input. Please enter a number.\n";
    		continue;}  	
    	
    	int pos = -1, index = -1;
    	for (int i = 0; i < cartCount; i++) if (cartID[i] == id) pos = i;
    	for (int i = 0; i < productCount; i++) if (productID[i] == id) index = i;

    	if (pos == -1) { cout << RED << "Item not found in cart!\n" << RESET; continue;}

   		stock[index] += cartQty[pos];        
    	saveStock(stock, productCount);      

    	for (int i = pos; i< cartCount - 1; i++) {
        	cartID[i] = cartID[i + 1]; cartName[i] = cartName[i + 1]; 
       		cartPrice[i] = cartPrice[i + 1]; cartQty[i] = cartQty[i + 1]; 
    		}
    	cartCount--;
    	cout << GREEN << "Item deleted!\n" << RESET;
	}	 	
        else if(choice==5){
        	cout << "\nEnter Customer Name (ALL CAPS): ";
    		cin.ignore();              
    		getline(cin, customerName);
    		
			int purchaseCount = getPurchaseCount(customerName);
			string tier = getTierName(purchaseCount);
			int nextTierNeed = getNextTierRequirement(purchaseCount);
    		
            double subtotal = 0;
    		cout << CYAN << "+======================= RECEIPT =======================+\n" << RESET;           
    		cout << "Customer Name: " << customerName << "\n";
    		cout << "Tier: " << tier << endl;
            cout << "Purchase Count: " << purchaseCount << endl;
            cout << "Next Tier in: " << nextTierNeed << " purchase(s)\n";
    		cout << "--------------------------------------------------------\n";
    		cout << left << setw(5) << "No." << setw(20) << "Item" << setw(8) << "Qty" << setw(10) << "Price" << "Total" << endl;
    		cout << "--------------------------------------------------------\n";

    		for (int i = 0; i < cartCount; i++) {
        		double totalItem = cartPrice[i] * cartQty[i];
        		cout << left << setw(5) << (i + 1) << setw(20) << cartName[i] << setw(8) << cartQty[i] << setw(10) << cartPrice[i] << totalItem << endl;
        		subtotal += totalItem;}
    		
    		cout << "--------------------------------------------------------\n";
    		cout << YELLOW << left << setw(33) << "SUBTOTAL:" << subtotal << endl << RESET;

    		cout << BLUE << "================== DISCOUNT ===========================\n" << RESET;
    		
			double tierDiscountPercent = getTierDiscount(purchaseCount);
    		
    		cout << GREEN << "Tier Discount (" << tier << "): " << tierDiscountPercent << "%\n" << RESET;
    		cout << "[10] Student Discount (10%)\n";
    		cout << "[20] Senior / PWD Discount (20%)\n";

			int extraDisc;
    			while (true) {
        		cout << "Choose ONE discount (Type 10/20/0 if none): "; cin >> extraDisc;
        		if (extraDisc == 0 || extraDisc == 10 || extraDisc == 20) break;
        	cout << RED << "Invalid discount option! Try again.\n" << RESET;}
			
			double totalDiscountPercent = tierDiscountPercent + extraDisc;
            double discountAmount = subtotal * (totalDiscountPercent / 100.0);
            double totalDue = subtotal - discountAmount;

    		cout << YELLOW << left << setw(33) << "DISCOUNT:" << "-" << discountAmount << endl;
            cout << GREEN << left << setw(33) << "TOTAL DUE:" << totalDue << RESET << endl;
            cout << "--------------------------------------------------------\n";

    		double amountPaid;
    		do {
        		cout << "Enter amount paid: "; cin >> amountPaid;
				if (amountPaid < totalDue)
            		cout << RED << "Invalid payment! Must be >= total due.\n" << RESET;
			} while (amountPaid < totalDue);

    		double change = amountPaid - totalDue;
    		cout << GREEN << left << setw(33) << "AMOUNT PAID:" << amountPaid << endl << RESET;
    		cout << left << setw(33) << "CHANGE DUE:" << change << endl;
    		cout << "+=======================================================+\n";

    		saveReceipt(cartID, cartName, cartPrice, cartQty, cartCount, totalDiscountPercent, amountPaid, customerName);
    		updatePurchaseCount(customerName);
    		cartCount = 0;
    		cout << GREEN << "Receipt saved. Items purchased. Cart cleared.\n" << RESET;
    		cout << YELLOW << "\tThank you for shopping!\n" << RESET;
	}
        else if (choice == 6) {
            if (!adminLogin()) continue;
            int adminChoice;
            do {
                cout << CYAN << "\n====== ADMIN MENU ======\n" << RESET;
                cout << "[1] View Stock\n[2] View Current Cart\n[3] View Last Receipt\n[4] Restock all items\n[5] View Customer Loyalty\n[6] Return to Main Menu\n";
                cout << "Enter choice: "; cin >> adminChoice;

                switch(adminChoice){
                    case 1: viewStockFile(productName, productID, stock, productCount);break;
                    case 2:
                        cout << MAGENTA << "\nCURRENT CART:\n" << RESET;
                        for (int i = 0; i < cartCount; i++)
                            cout << i + 1 << ". " << cartName[i] << " x" << cartQty[i] << " = Php " << cartPrice[i]*cartQty[i] << endl; break;
                    case 3: {
                        ifstream in("Gkiya_Receipt.txt");
                        if (!in) cout << RED << "No receipt found!\n" << RESET;
                        else { 
                            string line;
                            while (getline(in, line)) cout << line << endl;
                            in.close();
                        }
                        break;
                    }
					case 4: restockAll(stock, productCount); break;
					case 5: viewCustomerLoyalty();	break;
            	}      
    		}while (adminChoice != 6);   
    	}
        else if(choice==7){
            continueShopping = 'N';
            break;
        }
        else cout << RED << "Invalid choice!\n" << RESET;
		}
cout << CYAN << "Thank you for shopping here at Gunita't KuhanIyah!\n" << RESET;
return 0;	
	}
 