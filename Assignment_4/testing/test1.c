int fac(int n){
    if(n==1){return 1;}
    else{
        return fac(n-1) * n;
    }
}

int main(){
    int ans = fac(3);
    return ans;
}