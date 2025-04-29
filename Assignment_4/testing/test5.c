class A{
public:{
    int x;
    int w;
    char c;
}
private:{
    int g;
}
};

class B: public A{
public:{
    int xe;
    char xw;
    int xa;
}
private:{
    int wkk;
    int wkke;
}
};

int main(){
class A obj;
obj.w = 1;
int ww = obj.w;
printf("%d \n",ww);
class B obj1;
obj1.c = 'k'; //parent ka 

printf("%c \n",obj1.c);

int x = 0;
do{
    x++;
}until(x > 10);
printf("%d\n",x);


}