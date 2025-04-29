int main(){
    int x = 2;
    if(x > 0){
        x++;
        
    }
    else{
        x--;
    }
    printf("%d \n",x); 
    
    for(int i = 0;i<=10;i++)x += i;
   
    printf("%d\n",x); //exp is 385
    switch(x){
        case 1:
        x++;
        break;
        case 58:
        x = 20;
        break;
    }
    
    if(x < 0){
        x++;
    }
    else{
        x--;
    }
    printf("%d \n",x);
    int t = 0;
    for(int i=0;i<=10;i++){
        for(int j=0;j>-5;j--){
            t +=  i-j;
        }
    }
    printf("%d %d\n",x,t); //exp is 385
}