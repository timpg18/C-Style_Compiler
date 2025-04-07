int main() {
    int i = 0, j = 0;

    // // Simple for loop
    // for (i = 0; i < 3; i++) {
    //     printf("For loop i = %d\n", i);
    // }

    // // Nested for loop
    // for (i = 0; i < 2; i++) {
    //     for (j = 0; j < 2; j++) {
    //         printf("Nested For: i = %d, j = %d\n", i, j);
    //     }
    // }

    // // Simple while loop
    // i = 0;
    // while (i < 3) {
    //     printf("While loop i = %d\n", i);
    //     i++;
    // }

    // // Nested while inside for
    // for (i = 0; i < 2; i++) {
    //     j = 0;
    //     while (j < 2) {
    //         printf("Nested While: i = %d, j = %d\n", i, j);
    //         j++;
    //     }
    // }

    // // Simple do-until loop
    // i = 0;
    // do {
    //     printf("Do-While loop i = %d\n", i);
    //     i++;
    // } until (i < 3);

    // // do-while inside while
    // i = 0;
    // while (i < 2) {
    //     j = 0;
    //     do {
    //         printf("Nested Do-While: i = %d, j = %d\n", i, j);
    //         j++;
    //     } while (j < 2);
    //     i++;
    // }

    // Using break and continue in for loop
    for (i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (i == 4) break;
        printf("For with break/continue: i = %d\n", i);
    }

    return 0;
}
