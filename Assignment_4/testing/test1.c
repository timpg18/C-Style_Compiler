int main() {
    int a=0;

    // Using break and continue in for loop
    for (int i = 0; i < 5; i++) {
        if (i == 2) {continue;}
        // if (i == 4) {break;}
        printf("For with break/continue: i = %d\n", i);
    }

    return 0;
}
