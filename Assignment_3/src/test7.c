union G{
    int k;
    struct s{
        int p;
        double f;
    };
    struct s t;
};

int main(){
    union G h;
    h.t.f = 0;
}