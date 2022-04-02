#include <iostream>
#include <cstring>
#define DFLT_BUS_SPEED 60
#define DFLT_TRAIN_SPEED 100
#define DFLT_PLANE_SPEED 750

using namespace std;

class Transport {
public :
    virtual void travel(char * src, char * dst, unsigned dist) const = 0;
};

class Bus : public Transport {
    char * brand_name;
    unsigned speed;
public :
    Bus(char * brand_name_ = 0, unsigned speed_ = DFLT_BUS_SPEED) {
        brand_name = new char[strlen(brand_name_)];
        strcpy(brand_name, brand_name_);
        speed = speed_;
    }
    Bus(const Bus& obj) {
        brand_name = new char[strlen(obj.brand_name)];
        strcpy(brand_name, obj.brand_name);
        speed = obj.speed;
    }
    ~Bus() {
        if (brand_name) { delete[] brand_name; }
    }
    Bus& operator = (const Bus& obj) {
        if (brand_name) { delete[] brand_name; }
        brand_name = new char[strlen(obj.brand_name)];
        strcpy(brand_name, obj.brand_name);
        speed = obj.speed;
        return *this;
    }
    void travel(char * src, char * dst, unsigned dist) const {
        cout << "virtual function travel() : " << endl;
        cout << "Travel from " << src << " to " << dst << ' ' << dist << "km long by the " << brand_name <<  " bus with speed " << speed << "km/h will take " << (double)dist / speed << " hours" << endl; 
    }
    void operator() () {
        cout << "operator() :" << endl;
        cout << "vroom - vroom" << endl;
    }
    friend ostream& operator << (ostream& os, const Bus& bus);
};

ostream& operator << (ostream& os, const Bus& bus) {
    cout << "operator<< :" << endl;
    cout << "This is a " << bus.brand_name << " bus with speed " << bus.speed << "km/h" << endl;
    return os;
}

class Train : public Transport {
    char * train_type;
    unsigned speed;
public :
    Train(char * train_type_ = 0, unsigned speed_ = DFLT_TRAIN_SPEED) {
        train_type = new char[strlen(train_type_)];
        strcpy(train_type, train_type_);
        speed = speed_;
    }
    Train(const Train& obj) {
        train_type = new char[strlen(obj.train_type)];
        strcpy(train_type, obj.train_type);
        speed = obj.speed;
    }
    ~Train() {
        if (train_type) { delete[] train_type; }
    }
    Train& operator = (const Train& obj) {
        if (train_type) { delete[] train_type; }
        train_type = new char[strlen(obj.train_type)];
        strcpy(train_type, obj.train_type);
        speed = obj.speed;
        return *this;
    }
    void travel(char * src, char * dst, unsigned dist) const {
        cout << "virtual function travel() : " << endl;
        cout << "Travel from " << src << " to " << dst << ' ' << dist << "km long by the " << train_type <<  " train with speed " << speed << "km/h will take " << (double)dist / speed << " hours" << endl; 
    }
    void operator() () {
        cout << "operator() :" << endl;
        cout << "choo - choo" << endl;
    }
    friend ostream& operator << (ostream& os, const Train& type);
};

ostream& operator << (ostream& os, const Train& train) {
    cout << "operator<< :" << endl;
    cout << "This is a " << train.train_type << " train with speed " << train.speed << "km/h" << endl;
    return os;
}

class Plane : public Transport {
    unsigned speed;
    char * plane_type;
public :
    Plane(char * plane_type_ = 0, unsigned speed_ = DFLT_PLANE_SPEED) {
        plane_type = new char[strlen(plane_type_)];
        strcpy(plane_type, plane_type_);
        speed = speed_;
    }
    Plane(const Plane& obj) {
        plane_type = new char[strlen(obj.plane_type)];
        strcpy(plane_type, obj.plane_type);
        speed = obj.speed;
    }
    ~Plane() {
        if (plane_type) { delete[] plane_type; }
    }
    Plane& operator = (const Plane& obj) {
        if (plane_type) { delete[] plane_type; }
        plane_type = new char[strlen(obj.plane_type)];
        strcpy(plane_type, obj.plane_type);
        speed = obj.speed;
        return *this;
    }
    void travel(char * src, char * dst, unsigned dist) const {
        cout << "virtual function travel() : " << endl;
        cout << "Travel from " << src << " to " << dst  << ' ' << dist << "km long by the " << plane_type <<  " airplane with speed " << speed << "km/h will take " << (double)dist / speed << " hours" << endl; 
    }
    void operator() () {
        cout << "operator() :" << endl;
        cout << "poof" << endl;
    }
    friend ostream& operator << (ostream& os, const Plane& plane);
};

ostream& operator << (ostream& os, const Plane& plane) {
    cout << "operator<< :" << endl;
    cout << "This is a " << plane.plane_type << " airplane with speed " << plane.speed << "km/h" << endl;
    return os;
}

int main() {
    cout.precision(3);
    char src[128], dst[128], transp[128], type[128];
    unsigned dist, speed;
    Transport * base_class_ptr;
    for (int i = 0; i < 5; ++i) {
        base_class_ptr = 0;
        cin >> transp >> type  >> speed >> src >> dst >> dist;
        if (!strcmp(transp, "Bus")) {
            Bus b(type, speed);
            cout << b;
            b();
            base_class_ptr = &b;
            base_class_ptr->travel(src, dst, dist);
        }
        else if (!strcmp(transp, "Train")) {
            Train t(type, speed);
            cout << t;
            t();
            base_class_ptr = &t;
            base_class_ptr->travel(src, dst, dist);
        }
        else if (!strcmp(transp, "Plane")) {
            Plane p(type, speed);
            cout << p;
            p();
            base_class_ptr = &p;
            base_class_ptr->travel(src, dst, dist);
        }
        cin.ignore(INT_MAX, '\n');
        cout << endl;
    }
    return 0;
}