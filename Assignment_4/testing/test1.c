int f(int a, ...){
    return 0;
}

struct A{
    int a;
    char c;
    int* b;
};

union B{
    int a;
    char c;
};

enum color{
    red=1, g = 2, bl = 3
};

struct str{
    int x;
    float b;
    struct gcr{
        char c;
        int d;
    };
    struct gcr ss;
    int y;
};

int main(){
    int x[5];

    x[0] = 10;
    char ch[20];
    ch[5] = 'k';
    int y = x[0];
    x[1]  = 1;
    x[2]  = 2;
    char g = ch[5];
    printf("%d %c k \n", y, g);
    
    struct A c;
    c.a = 1;
    c.c = 'k';
    int xx = c.a;
    char ck = c.c;
    printf("%d %c \n",xx,ck);
    
    union B obj1;
    obj1.a = 5;
    obj1.c = 'k';
    xx = obj1.a;
    ck = obj1.c;
    printf("%d %c \n",xx,ck);
    enum color gg = red;
    xx = red;
    printf("%d %c \n",xx,ck);

    struct str ob;
    ob.ss.d = 5;
    ob.ss.c = 'b';
    int ww = ob.ss.d;
    char cp = 'b';
    printf("%d %c \n",ww,cp);
    int a[10];
    a[0]=9;
    a[2]=10;
    int ff = a[1] + a[2];
    double cc = 9.0;
    double d = 7.0;
    double b = cc+d;
    printf("%f %f",cc,d);
    printf("%d %f\n",ff,b);
    
}


