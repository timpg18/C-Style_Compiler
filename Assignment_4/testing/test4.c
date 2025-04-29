
typedef int myint;

int main(){
    myint ww = 100;
    int x = 1;
    int l = 0;
    while(x != 10){
        x++;
        continue;
       l++;
    }
    printf("%d %d %d\n",x,l,ww);
    do{
        x--;
    }while(x>0);
    printf("%d \n",x);
    int t = 1;
    for(int i=0;i<10;i++){
        t++;
        break;
    }
    printf("%d \n",t);
}