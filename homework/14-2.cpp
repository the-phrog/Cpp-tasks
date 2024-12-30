#include <iostream>
#include <vector>
#include <memory>

class Time{
private:
    int hours, minutes, seconds;
    static unsigned int count;

public:
    struct WrongHoursValue {
        int hours;
        WrongHoursValue(int h): hours(h){};
    };
    struct WrongMinutesValue {
        int minutes;
        WrongMinutesValue(int m): minutes(m){};
    };
    struct WrongSecondsValue {
        int seconds;
        WrongSecondsValue(int s): seconds(s){};
    };

    Time(){
        // std::cout << "  An object of class Time has been created by default."  << std::endl;
        // std::cout << "  In total: " << ++count << std::endl;
    };
    Time(int h, int m, int s): hours(h), minutes(m), seconds(s) {
        if(h < 0)
            throw WrongHoursValue(h);
        if(m < 0)
            throw WrongMinutesValue(m);
        if(s < 0)
            throw WrongSecondsValue(s);
        this->Normalize();
        // std::cout << "  An object of class Time has been created."  << std::endl;
        // std::cout << "  In total: " << ++count << std::endl;
    };
    Time(const Time& t): Time(t.hours, t.minutes, t.seconds){
        // std::cout << "  An object of class Time has been copied." << std::endl;
        // // std::cout << "  In total: " << ++count << std::endl;
    };
    Time(Time&& t): Time(t.hours, t.minutes, t.seconds){
        t.hours = 0;
        t.minutes = 0;
        t.seconds = 0;
        // std::cout << "  An object of class Time has been copied." << std::endl;
        // // std::cout << "  In total: " << ++count << std::endl;
    }
    ~Time() {
        // std::cout << "  An object of class Time has been destroyed." << std::endl;
        // std::cout << "  In total: " << --count << std::endl;
    };

    void SetHours(int hours)     { noexcept(true); this->hours   = hours;   this->Normalize(); };
    void SetMinutes(int minutes) { noexcept(true); this->minutes = minutes; this->Normalize(); };
    void SetSeconds(int seconds) { noexcept(true); this->seconds = seconds; this->Normalize(); };
    int GetHours()   const { noexcept(true); return hours;   };
    int GetMinutes() const { noexcept(true); return minutes; };
    int GetSeconds() const { noexcept(true); return seconds; };
    void PrintTime() { noexcept(true); std::cout << this->hours << ":" << this->minutes << ":" << this->seconds << std::endl; };

    void Normalize(){
        noexcept(true);
        int m = minutes + (seconds / 60);
        int h = hours + (m / 60);
        seconds = seconds % 60;
        if(seconds < 0) seconds += 60, minutes -= 1;
        minutes = m % 60;
        if(minutes < 0) minutes += 60, hours -= 1;
        hours = h % 24;
        if(hours < 0) hours += 24;
    }

    int ToSeconds() const { noexcept(true); return seconds + minutes * 60 + hours * 3600; }

    Time& operator = (const Time& other){
        hours   = other.hours;
        minutes = other.minutes;
        seconds = other.seconds;
        return *this;
    }
    Time& operator = (Time&& other){
        hours   = other.hours;
        minutes = other.minutes;
        seconds = other.seconds;
        other.hours = 0;
        other.minutes = 0;
        other.seconds = 0;
        return *this;
    }
    Time operator +  (int s)  { Time t = Time(hours, minutes, seconds + s); t.Normalize(); return t; };
    Time operator -  (int s)  { Time t = Time(hours, minutes, seconds - s); t.Normalize(); return t; };
    Time operator += (int s)  { Time t = Time(hours, minutes, seconds + s); t.Normalize(); return t; };
    Time operator -= (int s)  { Time t = Time(hours, minutes, seconds - s); t.Normalize(); return t; };
    bool operator == (Time t) const { return ((hours == t.hours) && (minutes == t.minutes) && (seconds == t.seconds)); };
};

unsigned Time::count = 0;

using namespace std;

int main(){
    Time t1(1, 2, 3);
    Time t2 = move(t1); // перемещающий конструктор
    t2.PrintTime();

    Time t3(4, 5, 6);
    t3 = move(t2); // перемещающий оператор присваивания
    t3.PrintTime();

    vector<Time> clocks;

    for(int i=0; i<3; i++)
        clocks.push_back(Time(2+i, i*5, i*10)); // добавление rvalues

    for(int i=0; i<3; i++){
        Time t(2+i, i*5, i*10);
        clocks.push_back(t); // определение и добавление lvalues
    }

    for(auto i : clocks)
        i.PrintTime();
}
