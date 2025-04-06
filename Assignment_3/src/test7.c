int f(int a,int b){
    return 0;
}

int main(){
    int x = 2, y = 1;

    switch (x) {
        case 1:
            printf("Outer case 1\n");
            break;
        case 2:
            printf("Outer case 2\n");

            switch (y) {
                case 0:
                    printf("  Inner case 0\n");
                    break;
                case 1:
                    printf("  Inner case 1\n");
                    break;
                default:
                    printf("  Inner default\n");
            }

            break;
        default:
            printf("Outer default\n");
    }

    return 0;
    

}