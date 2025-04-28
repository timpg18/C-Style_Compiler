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


int main(){
  
    int x[5];

    x[0] = 10;
    char ch[20];
    ch[5] = 'k';
    int y = x[0];
    char g = ch[5];
    printf("%d %c k", y, g);

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
}