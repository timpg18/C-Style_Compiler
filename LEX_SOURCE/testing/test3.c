/*
===================================================================
                               TEST 3
                      CLASS, STRUCT, ENUM, UNION
===================================================================
*/

class Car {
private:
    int id;
protected:
    int n_id;
public:
    string brand;
    string model;
    int year;

    void displayInfo() {
        printf("created");
    }
};

struct Book {
    char title[50];
    char author[50];
    int year;
};

enum Day { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };

union Data {
    int i;
    float f;
    char str[20];
};