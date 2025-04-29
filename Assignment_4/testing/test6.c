int f(int a, ...){
   printf("we are passing a lot of param boiii \n");
   return 1;
}

int main(){
    int b = f(1,2,4,9.0,9.8,4);
    int ***p;
    int **q;
    p = &q;
   
    int *a = &b;
    int x = *a;
    //we get 1 by reference!!
     
    
    //multilevel pointers and references
    int t = 2;
    int *t1 = &t;
    int **t2 = &t1;
    p = &t2;
    int xx = ***p;
    printf("%d %d\n",x ,xx);

    int *ar[4];
    ar[3] = t1;
    
    int soln = *t1;
    printf("%d \n",soln);

    int brr[4][5][6];
    for(int i=0;i<4;i++){
        for(int j=0;j<5;j++){
            for(int k=0;k<6;k++){
                int x = i + j + k;
                
                printf("%d ",x);
            }
        }
    }
   
}