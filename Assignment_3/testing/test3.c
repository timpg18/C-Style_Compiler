// SWITCH CASE

int main() {
    int x = 2, y = 1;

    switch (x) {
        case 1:
        printf("Case 1 in outer switch\n");
        switch (y) {
            case 0:
            printf("Case 0 in inner switch\n");
            break;
            case 1:
            printf("Case 1 in inner switch\n");
            break;
            default:
            printf("Default in inner switch\n");
            break;
        }
        break;
        case 2:
            printf("Case 2 in outer switch\n");
            break;
        case 3:
            printf("Case 3 in outer switch\n");
            break;
        // default:
        //     printf("Default in outer switch\n");
    }

    return 0;
}
