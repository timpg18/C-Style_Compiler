union data{
    int i;
    float f;
    char st[20];
};


int main(){
    int x = 0;
    while(x < 100){
        x++;
    }
    union data d;
    int g = d.i;
    char gg[21];
    gg = d.st;
    d.i = 5;
    gg = "hello";
    
    // (a==b)&&(a>b);
    //return 0;
}