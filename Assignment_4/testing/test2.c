// Recursive Function Call - Factorial Funciton

int fac(int n){
    if(n==1){
        return 1;
    }

    return n * fac(n-1);
}

int main(){
    int num = 5;
    int sum = fac(num);
    printf("factorial of %d is %d \n",num,sum);
    num = 9;
    sum = fac(num);
    printf("factorial of %d is %d \n",num,sum);
    return 0;
}