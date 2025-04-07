struct s1{
    int x;
    struct s2{
        int y;
        struct s3{
            int z;


        };
        struct s3 t;
    };
    struct s2 s;
};

int main(){
    struct s1 st;
    int x = st.s.t.z;
    return 0;
}