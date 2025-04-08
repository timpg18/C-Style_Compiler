int factorial(int n) {
    if (n == 0 || n == 1)  // base case
        return 1;
    else
        return n  factorial(n - 1);  // recursive case
}

int main() {
    int number = 5;
    printf("Factorial of %d is %d\n", number, factorial(number));
    return 0;
}