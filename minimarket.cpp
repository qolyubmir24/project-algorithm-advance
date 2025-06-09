#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <limits>
#include <sstream>

using namespace std;

ifstream ifs;
ofstream ofs;

struct Customer {
    string id;
    string name;
    string address;
};

struct Goods {
    int id;
    string name;
    int stock;
    float price;
};

struct Employee
{
    int id;
    string name;
    string password;
    string role;
};

Employee employees[100];
Goods goods[10000];
int totalGoods = 0;
int totalEmployees = 0;

int inputInteger(const string& prompt = "") {
    int value;
    while (true) {
        if (!prompt.empty()) cout << prompt;
        cin >> value;

        if (cin.fail()) {
            cin.clear(); // reset status error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // buang inputan yang salah
            cout << "Input tidak valid. Harap masukkan angka bulat.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // pastikan tidak ada karakter sisa
            return value;
        }
    }
}


void loadGoods() {
    ifs.open("goods.txt");
    totalGoods = 0;
    while (!ifs.eof()) {
        string temp;
        getline(ifs, temp);
        if (temp.empty()) break;

        goods[totalGoods].id = stoi(temp);
        getline(ifs, goods[totalGoods].name);
        ifs >> goods[totalGoods].stock;
        ifs >> goods[totalGoods].price;
        ifs.ignore(); // skip newline
        getline(ifs, temp); // skip delimiter ---
        totalGoods++;
    }
    ifs.close();
}

void saveGoods() {
    ofs.open("goods.txt");
    for (int i = 0; i < totalGoods; i++) {
        ofs << goods[i].id << endl;
        ofs << goods[i].name << endl;
        ofs << goods[i].stock << endl;
        ofs << goods[i].price << endl;
        ofs << "---" << endl;
    }
    ofs.close();
}

string generateCustomerID() {
    ifs.open("customer_data.txt");
    int count = 0;
    string line;
    while (getline(ifs, line)) {
        if (!line.empty()) count++;
    }
    ifs.close();
    return to_string(count);
}

void buatAkunCustomer() {
    Customer c;
    c.id = generateCustomerID();
    cin.ignore();
    cout << "Masukkan Nama: ";
    getline(cin, c.name);
    cout << "Masukkan Alamat: ";
    getline(cin, c.address);

    string password;
    cout << "Buat Password: ";
    getline(cin, password);

    ofs.open("customer_data.txt", ios::app);
    ofs << c.id << ";" << c.name << ";" << c.address << ";" << password << endl;
    ofs.close();

    ofs.open("onlinecustomer_data.txt", ios::app);
    ofs << c.id << ";" << c.name << ";" << c.address << ";" << password << endl;
    ofs.close();

    cout << "Akun berhasil dibuat. ID Anda: " << c.id << endl;
    system("pause");
}

bool loginCustomer(string& loggedInID, string& loggedInName) {
    system("cls");
    string id, password;
    cout << "Masukkan ID: ";
    cin >> id;
    cout << "Masukkan Password: ";
    cin >> password;

    ifs.open("customer_data.txt");
    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);
        size_t pos3 = line.find(';', pos2 + 1);
        string cid = line.substr(0, pos1);
        string cname = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string cpass = line.substr(pos3 + 1);
        if (cid == id && cpass == password) {
            loggedInID = cid;
            loggedInName = cname;
            ifs.close();
            return true;
        }
    }
    ifs.close();
    cout << "Login gagal.\n";
    system("pause");
    return false;
}


void kasirMenu() {
    bool running = true;
    int pilihan;

    while (running) {
        system("cls");
        cout << "\n=== MENU KASIR ===\n";
        cout << "1. Tambah Pelanggan\n";
        cout << "2. Kembali ke Menu Utama\n";
        cout << "Pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: {
                system("cls");
                string tambahBarang;
                loadGoods();
                Customer c;
                int idBarang, qty, i = 0;
                float total = 0;

                c.id = generateCustomerID();
                cin.ignore();
                cout << "Masukkan Nama Pelanggan: ";
                getline(cin, c.name);
                cout << "Masukkan Alamat: ";
                getline(cin, c.address);

                ofs.open("customer_data.txt", ios::app);
                ofs << c.id << ";" << c.name << ";" << c.address << endl;
                ofs.close();

                string filename = "order/id_" + c.id + ".txt";
                ofs.open(filename.c_str());
                ofs << "Nama   : " << c.name << endl;
                ofs << "Alamat : " << c.address << endl;
                ofs << "ID     : " << c.id << endl << endl;
                ofs << "-------------------------------------------------" << endl;

                while (true) {
                    cout << "Ingin menambahkan barang (y/n): ";
                    cin >> tambahBarang;
                    if (tambahBarang == "n" || tambahBarang == "N") break;
                    else if(tambahBarang == "y" || tambahBarang == "Y") {
                        cout << "ID Barang ke-" << i + 1 << ": ";
                        cin >> idBarang;
                        cout << "Jumlah: ";
                        cin >> qty;
                        i++;
                        for (int j = 0; j < totalGoods; j++) {
                            if (goods[j].id == idBarang && goods[j].stock >= qty) {
                                goods[j].stock -= qty;
                                float subtotal = qty * goods[j].price;
                                total += subtotal;
                                ofs << setw(35) << left << goods[j].name << " x" 
                                    << setw(3) << qty << " = " << setw(10) << subtotal << endl;
                            }
                        }
                    } else {
                        cout << "angka tidak valid\n";
                    }
                }

                ofs << "-------------------------------------------------" << endl;
                ofs << setw(42) << left << "Total: " << total << endl;
                ofs.close();

                system("cls");
                ifs.open("order/id_" + c.id + ".txt");
                string line;
                while (getline(ifs, line)) {
                    cout << line << endl;
                }
                ifs.close();
                saveGoods();
                cout << "\n         - Terimakasih Telah Berbelanja Di MAMAT GUNSHOP - \n";
                system("pause");
                break;
            }

            case 2:
                running = false;
                break;

            default:
                cout << "Pilihan tidak valid.\n";
                system("pause");
        }
    }
}

void sortByNameASC() {
    for (int i = 0; i < totalGoods - 1; i++) {
        for (int j = i + 1; j < totalGoods; j++) {
            if (goods[i].name > goods[j].name) {
                Goods temp = goods[i];
                goods[i] = goods[j];
                goods[j] = temp;
            }
        }
    }
}

void sortByStockASC() {
    for (int i = 0; i < totalGoods - 1; i++) {
        for (int j = i + 1; j < totalGoods; j++) {
            if (goods[i].stock > goods[j].stock) {
                Goods temp = goods[i];
                goods[i] = goods[j];
                goods[j] = temp;
            }
        }
    }
}

void searchGoods() {
    loadGoods();
    cin.ignore();
    string keyword;
    cout << "Masukkan nama barang yang dicari: ";
    getline(cin, keyword);

    bool found = false;
    for (int i = 0; i < totalGoods; i++) {
        if (goods[i].name.find(keyword) != string::npos) {
            cout << setw(5) << goods[i].id << " | " << setw(20) << left << goods[i].name
                 << " | Stok: " << setw(4) << goods[i].stock << " | Harga: " << goods[i].price << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Barang tidak ditemukan.\n";
    }
    system("pause");
}

void SearchCustomer(){
    cout << "Masukkan nama customer: ";
    string keyword;
    getline(cin, keyword);

    ifs.open("customer_data.txt");
    string line;
    while (getline(ifs, line)) {
        if (line.find(keyword) != string::npos) {
        string id = line.substr(0, line.find(';'));
        ifstream orderFile("order/id_" + id + ".txt");
        if (!orderFile.is_open()) continue;
        string orderLine;
        while (getline(orderFile, orderLine)) {
            cout << orderLine << endl;
            }
        cout << string(50, '=') << endl;
        orderFile.close();
            }
        }
    ifs.close();
    system("pause");
}

void displayAllGoods() {
    //loadGoods();
    cout << left << setw(5) << "ID" << setw(35) << "Nama Barang" << setw(6) << "Stok" << "Harga" << endl;
    cout << string(52, '-') << endl;
    for (int i = 0; i < totalGoods; i++) {
        cout << left << setw(5) << goods[i].id << setw(35) << goods[i].name
             << setw(7) << goods[i].stock << goods[i].price << endl;
    }
    system("pause");
}

void displayGoodsSortedByName() {
    sortByNameASC();
    cout << "\nBarang diurutkan berdasarkan nama (A-Z):\n";
    displayAllGoods();
}

void displayGoodsSortedByStock() {
    sortByStockASC();
    cout << "\nBarang diurutkan berdasarkan stok (terkecil - terbesar):\n";
    displayAllGoods();
}

void tambahBarangBaru() {
    int inputID;
    cout << "Masukkan ID barang: ";
    cin >> inputID;

    for (int i = 0; i < totalGoods; i++) {
        if (goods[i].id == inputID) {
            int tambahStok;
            cout << "Barang dengan ID tersebut sudah ada: " << goods[i].name << endl;
            cout << "Stok saat ini: " << goods[i].stock << endl;
            cout << "Masukkan jumlah stok yang ingin ditambahkan: ";
            cin >> tambahStok;
            goods[i].stock += tambahStok;
            saveGoods();
            cout << "Stok berhasil diperbarui. Stok sekarang: " << goods[i].stock << endl;
            system("pause");
            return;
        }
    }

    cin.ignore();
    Goods newGoods;
    newGoods.id = inputID;
    cout << "ID belum terdaftar. Tambah barang baru.\n";
    cout << "Masukkan nama barang: ";
    getline(cin, newGoods.name);
    cout << "Masukkan jumlah stok: ";
    cin >> newGoods.stock;
    cout << "Masukkan harga barang: ";
    cin >> newGoods.price;

    goods[totalGoods] = newGoods;
    totalGoods++;
    saveGoods();
    cout << "Barang baru berhasil ditambahkan.\n";
    system("pause");
}

void gudangMenu() {
    loadGoods();
    bool loop = true;
    int pilihan;
    while (loop) {
        system("cls");
        cout << "\n--- Menu Petugas Gudang ---\n";
        cout << "1. Tampilkan Semua Barang\n";
        cout << "2. Urutkan Berdasarkan Nama (ASC)\n";
        cout << "3. Urutkan Berdasarkan Stok (ASC)\n";
        cout << "4. Cari Barang\n";
        cout << "5. Tambah Barang Baru\n";
        cout << "6. Kembali\n";
        cout << "Pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                displayAllGoods();
                break;
            case 2:
                displayGoodsSortedByName();
                break;
            case 3:
                displayGoodsSortedByStock();
                break;
            case 4:
                searchGoods();
                break;
            case 5:
                tambahBarangBaru();
                break;
            case 6:
                loop = false;
                break;
            default:
                cout << "Pilihan tidak valid.\n";
                system("pause");
        }
    }
}

void displayAllTransactions() {
    system("cls");
    int TransactionTotal = 0;
    string line;
    ifs.open("customer_data.txt");
    if (!ifs.is_open()) {
        cout << "Tidak ada transaksi yang ditemukan.\n";
        return;
    }
    while (getline(ifs, line)) {
        if (line != "") {
            TransactionTotal++;
        }
    }
    ifs.close();

    for (int i = 0; i < TransactionTotal; i++) {
        string str = to_string(i);
        ifs.open("order/id_" + str + ".txt");
        if (!ifs.is_open()) continue;
        while (getline(ifs, line)) {
            cout << line << endl;
        }
        ifs.close();
        cout << endl << string(50, '=') << endl;
    }
    system("pause");
}

void displayOnlineTransactions() {
    system("cls");
    string line;

    ifstream ifs("customer_data.txt");
    if (!ifs.is_open()) {
        cout << "Tidak ada transaksi yang ditemukan.\n";
        return;
    }

    while (getline(ifs, line)) {
        // Pastikan format "id;nama;alamat;password"
        int semicolonCount = 0;
        for (char c : line) {
            if (c == ';') semicolonCount++;
        }
        if (semicolonCount == 3){
            stringstream ss(line);
            string id;
            getline(ss, id, ';'); // Ambil ID sebelum titik koma

            if (!id.empty()) {
                ifstream orderFile("order/id_" + id + ".txt");
                if (!orderFile.is_open()) continue;

                string orderLine;
                while (getline(orderFile, orderLine)) {
                    cout << orderLine << endl;
                }
                orderFile.close();

                cout << endl << string(50, '=') << endl;
            }
        }
    }
    ifs.close();
    system("pause");
}

void managerMenu() {
    bool managermenu = true;
    while (managermenu) {
        system("cls");
        cout << "\n=== MENU MANAJER ===\n";
        cout << "1. Lihat semua barang di gudang\n";
        cout << "2. Lihat barang (Sort nama ASC)\n";
        cout << "3. Lihat barang (Sort stock ASC)\n";
        cout << "4. Lihat semua transaksi\n";
        cout << "5. Kembali\n";
        cout << "Pilih: ";
        int choice;
        cin >> choice;
        cin.ignore();
        switch (choice) {
        case 1:
            displayAllGoods();
            break;
        case 2:
            displayGoodsSortedByName();
            break;
        case 3:
            displayGoodsSortedByStock();
            break;
        case 4:
            displayAllTransactions();
            break;
        case 5:
            managermenu = false;
            break;
        default:
            cout << "Pilihan tidak valid. Coba lagi.\n";
            system("pause");
            break;
        }
    }
}

void loadEmployees() {
    ifstream file("employee.txt");
    totalEmployees = 0;
    if (!file.is_open()) {
        cout << "Gagal membuka file employee.txt\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        employees[totalEmployees].id = stoi(line);
        getline(file, employees[totalEmployees].name);
        getline(file, employees[totalEmployees].password);
        getline(file, employees[totalEmployees].role);
        getline(file, line); // skip delimiter ---
        totalEmployees++;
    }
    file.close();
}

bool login(const string& role, int attempt = 1) {
    system("cls");
    int inputID;
    string inputPassword;
    cout << "\nLOGIN SEBAGAI " << role << " - Percobaan ke-" << attempt << " dari 3\n";
    cout << "ID       : ";
    inputID = inputInteger(); // pakai fungsi validasi angka
    cout << "Password : ";
    cin >> inputPassword;

    for (int i = 0; i < totalEmployees; i++) {
        if (employees[i].id == inputID && employees[i].password == inputPassword && employees[i].role == role) {
            cout << "Login berhasil sebagai " << role << " (" << employees[i].name << ")\n";
            system("pause");
            return true;
        }
    }

    cout << "Login gagal. ID atau password salah.\n";
    if (attempt >= 3) {
        cout << "Kesempatan habis. Kembali ke menu utama.\n";
        system("pause");
        return false;
    } else {
        system("pause");
        return login(role, attempt + 1);
    }
}

void kurirMenu() {
    bool loop = true;
    while (loop) {
        system("cls");
        cout << "--- Menu Kurir ---\n";
        cout << "1. Lihat Semua Pesanan\n";
        cout << "2. Cari Pesanan Berdasarkan Nama\n";
        cout << "3. Kembali\n";
        cout << "Pilihan: ";
        int pilih;
        cin >> pilih;
        cin.ignore();
        switch (pilih) {
            case 1:
                displayOnlineTransactions();
                break;
            case 2: {
                SearchCustomer();
                break;
            }
            case 3:
                loop = false;
                break;
            default:
                cout << "Pilihan tidak valid.\n";
                system("pause");
        }
    }
}

void offlineStore(){
    int role;
    do {
        system("cls");
        cout << "\nPilih Role:\n1. Kasir\n2. Petugas Gudang\n3. Manajer\n4. Kurir\n5. Keluar\nPilihan: ";
        cin >> role;
        switch (role) {
            case 1:
                if (login("kasir")) kasirMenu();
                break;
            case 2:
                if (login("gudang")) gudangMenu();
                break;
            case 3:
                if (login("manajer")) managerMenu();
                break;
            case 4:
                if (login("kurir")) kurirMenu();
                break;
            case 5 : cout << "bye!\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
                system("pause");
                break;
        }
    } while (role != 5);
}

void onlineCustomerMenu(const string& id, const string& name) {
    bool loop = true;
    while (loop) {
        system("cls");
        cout << "Halo, " << name << " (ID: " << id << ")\n";
        cout << "1. Lihat Semua Barang\n";
        cout << "2. Cari Barang\n";
        cout << "3. Beli Barang\n";
        cout << "4. Keluar\n";
        cout << "Pilihan: ";
        int pilihan;
        cin >> pilihan;
        switch (pilihan) {
            case 1:
                displayAllGoods();
                break;
            case 2:
                searchGoods();
                break;
            case 3: {
                loadGoods();
                string filename = "order/id_" + id + ".txt";
                ofs.open(filename.c_str(), ios::app);
                ofs << "Nama   : " << name << endl;
                ofs << "ID     : " << id << endl << endl;
                ofs << "-------------------------------------------------" << endl;

                float total = 0;
                int qty, idBarang;
                string tambahBarang;
                while (true) {
                    cout << "Ingin menambahkan barang (y/n): ";
                    cin >> tambahBarang;
                    if (tambahBarang == "n" || tambahBarang == "N") break;
                    else if (tambahBarang == "y" || tambahBarang == "Y") {
                        cout << "Masukkan ID Barang: ";
                        cin >> idBarang;
                        cout << "Jumlah: ";
                        cin >> qty;

                        for (int j = 0; j < totalGoods; j++) {
                            if (goods[j].id == idBarang && goods[j].stock >= qty) {
                                goods[j].stock -= qty;
                                float subtotal = qty * goods[j].price;
                                total += subtotal;
                                ofs << setw(35) << left << goods[j].name << " x" 
                                    << setw(3) << qty << " = " << setw(10) << subtotal << endl;
                            }
                        }
                    }
                }

                ofs << "-------------------------------------------------" << endl;
                ofs << setw(42) << left << "Total: " << total << endl;
                ofs.close();
                saveGoods();

                cout << "Pembelian berhasil!\n";
                system("pause");
                break;
            }
            case 4:
                loop = false;
                break;
            default:
                cout << "Pilihan tidak valid\n";
                system("pause");
                break;
        }
    }
}

void onlineStore(){
    bool onlinestore = true;
    int pilihan;
    while(onlinestore){
        system("cls");
        cout << "\n            - Online Menu - \n1. Buat Akun\n2. Login\n3. Exit\nPilihan: ";
        cin >> pilihan;
        switch (pilihan) {
            case 1:
                buatAkunCustomer();
                break;
            case 2: {
                string id, name;
                if (loginCustomer(id, name)) {
                    onlineCustomerMenu(id, name);
                }
                break;
            }
            case 3:
                onlinestore = false;
                break;
            default:
                cout << "Pilihan tidak valid.\n";
                system("pause");
                break;
        }
    }
}

int main() {
    loadEmployees();
    loadGoods(); 
    bool main = true;
    int pilihan;
    while(main){
        system("cls");
        cout << "                      - Welcome To MAMAT GUNSHOP - \n1. Online Store\n2. Offline Store\n3. exit program\npilihan : ";
        cin >> pilihan;
        switch (pilihan)
        {
        case 1:
            onlineStore();
            break;
        case 2:
            offlineStore();
            break;
        case 3:
            main = false;
            break;
        default:
            cout << "Pilihan tidak valid";
            break;
        }
    }
    return 0;
}
