//BASIC FEATURES

int func(){
    static int x;
}


int main(){
    //ARITHMETIC 
    int a = 1;
    int b = 12;
    int c = a&&b;
    c = a&b; c = a+b;
    c += a;
    c -= b;
    c++;
    --a;
    c = a>>b;
    c = a<<b;
    c = sizeof(a);
    int k = c*b;
    c ^= b;
    c %= k;

    lab:
    //if + loops (all kind)
    //also goto + break + continue added
    if(c > b){
        for(int c1 = 0; c1 <100;c1++){
            c1++;
            continue;
            int gg = 10;
        }
    }
    else{
        while(c < 1000){
            c++;
            a++;
            break;
        }
        do{
            c--;
            b--;
        
        }while(c > 0);
        goto lab;
    }
    //switch case
    switch(c){
        //repeated labels, or duplicate default will give error!!
        case 1 :
        c++;
        break;
        case 2 :
        c -= 10;
        break;
        default:
        c++;
        break;
    }

    //arrays
    int arr[200][100][10][12];
    int brr[1][2][3][4];
    arr[0][1][2][1] = 3;
    brr[10][12][12][32] = 10;

    //static
    static int x = 10;
    x++;
    static int g = 122;
    g--;
    static int* kk = &x;
    
}