struct A{
    int h;
};

int f(){
    return 0;
}

int main() {
    int * b;
    int const * a = b;
    const struct A d;
    int c;
    a = &c;
    (*a) = 1;
}