//ADVANCE features

class myclass{
    public:{
        int x,y;
        int func1(int xt){
            return 20;
        }
    }
    private:{
        int d2;
    }
    };
    
    class par2{
    public:{
        int t1,t2;
    }
    };
    
    class derieve : public myclass , protected par2{
    public:{
        int yy;
        int gg(int xx){
            return 7;
        }
    }
    private:{
        int f(int zz){
    
        }
    }
    };
    
    struct st1{
        int i;
        struct st2{
            int j;
            struct st3{
                int k;
                double x;
            };
            struct st3 t3;
        };
        struct st2 t2;
    };
    
    struct st5{
        int k4;
        union k3{
            double x3;
            int x4;
            struct sg1{
                int x5;
                int x6;
            };
        };
    };
    union un1{
        int x;
        double g;
        int xrr;
    };
    
    
    
    enum Color {
        RED = 1,
        GREEN = 2,
        BLUE
    };
    int main(){
    class myclass arr[20];
    class derieve arr2[5];
    
    for(int i=0;i<20;i++){
        arr[i].x = i;
        arr[i].y = i;
    }
    class myclass a;
    
    struct st1 s1;
    s1.t2.t3.x = 18 + a.x  + arr[8].x;
    struct st1 ar3[10];
    ar3[5].t2.j = 9;
    
    
    //now multi dim pointers AND ARRAYS ALL IN ONE!!
    int ***ark[20][1][10]; 
    //obv it gives error if you mismatch dimnesions while assigning
    int **brr[20][1][10];
    //ark[2][2][3][4] = brr[1][2][3];  error!
    *ark[20][1][9] = brr[1][2][3];
    
    union un1 st;
    enum Color c1;
    c1 = RED;
    enum Color c3;
    c1 = c3;
    
    }