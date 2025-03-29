class animal{
    private:{
        int a,b;
    }
};

int main(){
    struct animal lion;
    class animal * ptr = &lion;
    (*ptr).a;
}