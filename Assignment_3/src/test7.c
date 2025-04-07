//advance features

int func_ellipses(int a, int b , ...){
    return a +b;
}
int** add(int x){
   
}

int main(){
    //int a = func_ellipses(7,2); ERROR!
    
    int** (*func)(int x) = &add;
    int** (*fu)(int y) = &add;
    int** (**fuk)(int z) = &fu;
   

}