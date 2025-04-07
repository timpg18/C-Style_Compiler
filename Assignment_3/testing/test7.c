//advance features

int func_ellipses(int a, int b , ...){
    return a +b;
}

int** add(int *k, int l){
    
    return &(k);
}

int main(){
    //int a = func_ellipses(7,2); ERROR!
    int a = func_ellipses(7,2,3,5,6,5,4,3,3,8,3); 
    a = func_ellipses(8,2,5);
    int** (*func)(int *z, int p) = &add;
    int** (**f2)(int *z1,int p1) = &func;

    int *p2 =  &a;
    (*p2) = 100;
    
}