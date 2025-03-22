union ggg{
    int w;
    double g;
    long ww;
};


int main(){
    int a;
    int b;
    int c;

    Label:
    for(int i=0;i<n;i++){
        a++;
        goto Label;
    }
    for(int i=0;i<10;i++){
        break;
    }
    for(int j=0;j<12;j++){
        continue;
    }
    static int g  = 5;
    register w = 4;
    volatile register ee = 44;
    volatile register*** ww = 121;
    union ggg x;
}