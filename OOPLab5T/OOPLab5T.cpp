#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// ======================================================
// TASK 1: Pair -> RightAngled
// ======================================================

class Pair {
protected:
    double first, second;

public:
    static int count;

    Pair(double a = 0, double b = 0) : first(a), second(b) {
        count++;
        cout << "Pair created\n";
    }

    virtual ~Pair() {
        count--;
        cout << "Pair destroyed\n";
    }

    void setFirst(double a) { first = a; }
    void setSecond(double b) { second = b; }

    double product() const { return first * second; }

    virtual void print() const {
        cout << "Pair: " << first << ", " << second << endl;
    }

    static int getCount() { return count; }
};

int Pair::count = 0;

class RightAngled : public Pair {
public:
    RightAngled(double a = 0, double b = 0) : Pair(a, b) {
        cout << "RightAngled created\n";
    }

    ~RightAngled() override {
        cout << "RightAngled destroyed\n";
    }

    double hypotenuse() const {
        return sqrt(first * first + second * second);
    }

    double area() const {
        return (first * second) / 2.0;
    }

    void print() const override {
        cout << "Right triangle: " << first << ", " << second << endl;
    }
};

// ======================================================
// TASK 2: Engine -> Car -> Truck (composition)
// ======================================================

class Engine {
private:
    int power;

public:
    Engine(int p = 0) : power(p) {
        cout << "Engine created\n";
    }

    ~Engine() {
        cout << "Engine destroyed\n";
    }

    int getPower() const { return power; }
};

class Car {
protected:
    Engine engine;
    char* brand;
    double price;

public:
    Car(const char* b = "NoName", int pwr = 0, double pr = 0)
        : engine(pwr), price(pr) {
        brand = new char[strlen(b) + 1];
        strcpy(brand, b);
        cout << "Car created\n";
    }

    virtual ~Car() {
        delete[] brand;
        cout << "Car destroyed\n";
    }

    virtual void print() const {
        cout << "Brand: " << brand
             << ", Power: " << engine.getPower()
             << ", Price: " << price << endl;
    }
};

class Truck : public Car {
private:
    double capacity;

public:
    Truck(const char* b = "Truck", int p = 0, double pr = 0, double c = 0)
        : Car(b, p, pr), capacity(c) {
        cout << "Truck created\n";
    }

    ~Truck() override {
        cout << "Truck destroyed\n";
    }

    void print() const override {
        cout << "Truck -> ";
        Car::print();
        cout << "Capacity: " << capacity << endl;
    }
};

// ======================================================
// TASK 3: Furniture -> Table (copy + move)
// ======================================================

class Furniture {
protected:
    char* material;

public:
    Furniture(const char* m = "wood") {
        material = new char[strlen(m) + 1];
        strcpy(material, m);
        cout << "Furniture created\n";
    }

    // COPY
    Furniture(const Furniture& other) {
        material = new char[strlen(other.material) + 1];
        strcpy(material, other.material);
        cout << "Furniture copied\n";
    }

    // MOVE
    Furniture(Furniture&& other) noexcept {
        material = other.material;
        other.material = nullptr;
        cout << "Furniture moved\n";
    }

    // COPY ASSIGN
    Furniture& operator=(const Furniture& other) {
        if (this != &other) {
            delete[] material;
            material = new char[strlen(other.material) + 1];
            strcpy(material, other.material);
        }
        cout << "Furniture assigned\n";
        return *this;
    }

    // MOVE ASSIGN
    Furniture& operator=(Furniture&& other) noexcept {
        if (this != &other) {
            delete[] material;
            material = other.material;
            other.material = nullptr;
        }
        cout << "Furniture move assigned\n";
        return *this;
    }

    virtual ~Furniture() {
        delete[] material;
        cout << "Furniture destroyed\n";
    }

    virtual void print(ostream& out) const {
        out << "Material: " << material;
    }

    virtual void read(istream& in) {
        char temp[100];
        in >> temp;
        delete[] material;
        material = new char[strlen(temp) + 1];
        strcpy(material, temp);
    }

    friend ostream& operator<<(ostream& out, const Furniture& f) {
        f.print(out);
        return out;
    }

    friend istream& operator>>(istream& in, Furniture& f) {
        f.read(in);
        return in;
    }
};

class Table : public Furniture {
private:
    int legs;

public:
    Table(const char* m = "wood", int l = 4)
        : Furniture(m), legs(l) {
        cout << "Table created\n";
    }

    // COPY
    Table(const Table& other)
        : Furniture(other), legs(other.legs) {
        cout << "Table copied\n";
    }

    // MOVE
    Table(Table&& other) noexcept
        : Furniture(move(other)), legs(other.legs) {
        cout << "Table moved\n";
    }

    // ASSIGN
    Table& operator=(const Table& other) {
        if (this != &other) {
            Furniture::operator=(other);
            legs = other.legs;
        }
        cout << "Table assigned\n";
        return *this;
    }

    // MOVE ASSIGN
    Table& operator=(Table&& other) noexcept {
        if (this != &other) {
            Furniture::operator=(move(other));
            legs = other.legs;
        }
        cout << "Table move assigned\n";
        return *this;
    }

    ~Table() override {
        cout << "Table destroyed\n";
    }

    void print(ostream& out) const override {
        out << "Material: " << material << ", Legs: " << legs;
    }

    void read(istream& in) override {
        Furniture::read(in);
        in >> legs;
    }
};

// ======================================================
// INPUT METHODS
// ======================================================

void inputKeyboard() {
    double a, b;
    cout << "\nKeyboard input: ";
    cin >> a >> b;

    RightAngled r(a, b);
    r.print();
    cout << "Hypotenuse: " << r.hypotenuse() << endl;
}

void inputFile() {
    ifstream fin("input.txt");

    if (!fin) {
        cout << "File not found\n";
        return;
    }

    double a, b;
    fin >> a >> b;

    RightAngled r(a, b);
    cout << "\nFile input:\n";
    r.print();

    fin.close();
}

void inputRandom() {
    double a = rand() % 10 + 1;
    double b = rand() % 10 + 1;

    RightAngled r(a, b);
    cout << "\nRandom input:\n";
    r.print();
}

// ======================================================
// TESTS
// ======================================================

void test1() {
    cout << "\nTEST 1\n";
    RightAngled r(3, 4);
    cout << "Expected: 5\nActual: " << r.hypotenuse() << endl;
}

void test2() {
    cout << "\nTEST 2\n";
    Truck t("DAF", 400, 50000, 10000);
    t.print();
}

void test3() {
    cout << "\nTEST 3\n";

    Table t1("metal", 4);
    Table t2 = t1;
    Table t3;
    t3 = move(t1);

    cout << t2 << endl;
    cout << t3 << endl;
}

// ======================================================
// MAIN
// ======================================================

int main() {
    srand(time(0));

    cout << "=== TASK 1 ===\n";
    RightAngled r(6, 8);
    r.print();

    inputKeyboard();
    inputFile();
    inputRandom();

    cout << "\n=== TASK 2 ===\n";
    Truck t("Volvo", 500, 80000, 15000);
    t.print();

    cout << "\n=== TASK 3 ===\n";
    Table table("wood", 4);
    cout << table << endl;

    cout << "Enter material and legs: ";
    cin >> table;
    cout << table << endl;

    test1();
    test2();
    test3();

    return 0;
}