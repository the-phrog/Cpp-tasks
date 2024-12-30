#include <iostream>
#include <memory>

using namespace std;

class Gear {
public:
    virtual ~Gear() = default;
    virtual double calculate_speed(double rpm) = 0;
};

class Gear_R: public Gear {
public:
    double calculate_speed(double rpm) { return rpm*(-0.1); }
};

class Gear_N: public Gear {
public:
    double calculate_speed(double rpm) { return 0; }
};

class Gear_1: public Gear {
public:
    double calculate_speed(double rpm) { return rpm*0.1; }
};

class Gear_2: public Gear {
public:
    double calculate_speed(double rpm) { return rpm*0.2; }
};

class Gear_3: public Gear {
public:
    double calculate_speed(double rpm) { return rpm*0.3; }
};

class Gear_4: public Gear {
public:
    double calculate_speed(double rpm) { return rpm*0.4; }
};

class Gear_5: public Gear {
public:
    double calculate_speed(double rpm) { return rpm*0.5; }
};

class Car {
private:
    unique_ptr<Gear> gear_;
    double rpm_ = 0;
    double speed_ = 0;
public:
    Car(unique_ptr<Gear> &&gear): gear_(move(gear)) {}
    void set_rpm(double rpm) { rpm_ = rpm; }
    void set_gear(unique_ptr<Gear> &&gear) { gear_ = move(gear); }
    void set_speed() { speed_ = gear_->calculate_speed(rpm_); }
    void speed() { cout << "Current speed - " << speed_ << endl; }
};

int main(){
    Car car(make_unique<Gear_1>());
    car.set_rpm(1000);
    car.set_speed();
    car.speed();

    car.set_gear(make_unique<Gear_2>());
    car.set_speed();
    car.speed();

    car.set_gear(make_unique<Gear_3>());
    car.set_speed();
    car.speed();

    car.set_gear(make_unique<Gear_4>());
    car.set_speed();
    car.speed();

    car.set_rpm(2000);
    car.set_speed();
    car.speed();

    car.set_gear(make_unique<Gear_5>());
    car.set_speed();
    car.speed();

    car.set_gear(make_unique<Gear_N>());
    car.set_speed();
    car.speed();

    car.set_gear(make_unique<Gear_R>());
    car.set_speed();
    car.speed();
}