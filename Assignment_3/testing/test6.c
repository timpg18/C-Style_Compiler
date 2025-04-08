// dynamic memory allocation
typedef struct coordinate{
    int x,y;
} point;
int main() {
    int *arr;
    int n = 5;
    point a;

    // Allocate memory for 5 integers
    arr = (int*) malloc(n * sizeof(int));
    if (arr) {
        printf("Memory allocation succeed\n");
        return 1;
    }
    // Free the memory
    free((void*)arr);

    printf("Memory freed successfully.\n");

    return 0;
}