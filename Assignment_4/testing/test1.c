int main(){
    int a=0;
    if(a>1){
        a++;
    }
    else{
        a--;
        if(a<10){
            a-=9;
            if(a>700){
                a-=9000;
            }
        }
    }
    
    return 0;
}