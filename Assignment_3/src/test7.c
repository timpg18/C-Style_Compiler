//advance features

int func_ellipses(int a, int b , ...){
    return a +b;
}
int** add(int x){
    int a = 10;
    return &(&a);
}

int main(){
    //int a = func_ellipses(7,2); ERROR!
    int a = func_ellipses(7,2,3,5,6,5,4,3,3,8,3); 
    a = func_ellipses(8,2,5);
    int** (*func)(int x) = &add;

    int *p =  &a;
    (*p) = 100;

}