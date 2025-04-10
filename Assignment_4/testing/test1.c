int main() {
    int a=0;

    if(a<1){
        a=10000;
        if(a>1){
            a=2;
            if(a>1){
                a++;
            }
            else {
                a--;
            }
            a=9;
        }
        else{
            a+=2;
        }
        a++;
    }
    else{
        a-=2;
    }
    if(a<9){
        a+=4;
    }

    

    return 0;
}
