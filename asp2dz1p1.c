#include <stdio.h>
#include <stdlib.h>

void menu();
int ternary_search(int *array, int key, int level);
int interpolation_search(int *array, int key, int level);
int factorial(int number);
int formula(int n, int k);
int calculate_element(int n, int k);
int *make_triangle(int n, int k, int *array);
int *make_level_of_triangle(int level);
int *make_sequence_of_keys();
int search(int *array, int *keys, int level, int ternary_steps, int interpolation_steps, int flag);
void compare_performance(int ternary_steps, int interpolation_steps);


void menu() {
    printf("****  MENU  ****\n");
    printf("1. Make a triangle\n");
    printf("2. Start search\n");
    printf("3. Print results and compare performance\n");
    printf("4. Start again\n");
    printf("0. Exit\n");
}

int factorial(int number) {
    int result = 1;
    for (int i = 1; i <= number; i++) {
        result *= i;
    }
    return result;
}

int formula(int n, int k) {
    int first, second, third;
    int total;
    if ((n == 0 && k == 0) || (n == 1 && k == 0) || (n == 1 && k == 1)) {
        first = 1;
        second = 1;
        third = 1;
        total = (first + (second * third)) / 2;
        return total;
    } else if (n == 0 && k == 1) {
        total = 0;
        return total;
    }else {
        first = factorial(n);
        second = factorial(k);
        third = factorial(n - k);
        total = (first / (second * third));
        return total;
    }
}

int calculate_element(int n, int k) {
    int element, first, second, third;
    first = formula(n, k);
    second = formula(n % 2, k % 2);
    third = formula(n / 2, k / 2);
    element = (first + (second * third)) / 2;
    return element;
}

int *make_triangle(int n, int k, int *array) {
    for (int i = 0; i < k; i++) {
        int value = calculate_element(n, i);
        array[i] = value;
    }
    return array;
}

int *make_level_of_triangle(int level) {
    int k = level + 1;
    int *array = malloc(k * sizeof(int));
    make_triangle(level, k, array);
    return array;
}

int *make_sequence_of_keys() {
    int value, size, counter = 0;
    printf("How many keys do you want: ");
    scanf("%d", &size);
    printf("Enter keys: ");
    int *keys = malloc((size + 1) * sizeof(int));
    while (counter < size) {
        scanf("%d", &value);
        keys[counter] = value;
        counter++;
    }
    keys[size] = -1;
    return keys;
}

int search(int *array, int *keys, int level, int ternary_steps, int interpolation_steps, int flag) {
    int steps_t, steps_i;
    int success_search = 0;
    for (int i = 0; keys[i] != -1; i++) {
        if (flag) {
            steps_t = ternary_search(array, keys[i], level);
            if (steps_t != -1) {
                success_search++;
                ternary_steps = ternary_steps + steps_t;
            }
        }else{
            steps_i = interpolation_search(array, keys[i], level);
            if (steps_i != -1) {
                success_search++;
                interpolation_steps = interpolation_steps + steps_i;
            }
        }
    }
    if (flag) {
        ternary_steps /= success_search;
        return ternary_steps;
    }else{
        interpolation_steps /= success_search;
        return interpolation_steps;
    }
}

int ternary_search(int *array, int key, int level) {
    int counter = 0, steps = 0;
    int left = 0, right = (level - 1) / 2;

    printf("->Searching for key %d\n", key);
    while (left <= right) {
        steps++;
        printf("Step %d:\n", steps);

        int left_mid = left + (right - left) / 3;
        int left_mid_elem = array[left_mid];
        counter++;
        printf(" Access to element at index %d.\n", left_mid);
        if (key == left_mid_elem) {
            printf(" Key found in array at index %d.\n", left_mid);
            return counter;
        }

        int right_mid = right - (right - left) / 3;
        int right_mid_elem = array[right_mid];
        counter++;
        printf(" Access to element at index %d.\n", right_mid);
        if (key == right_mid_elem) {
            printf(" Key found in array at index %d.\n", right_mid);
            return counter;
        }

        if (key < left_mid_elem) {
            right = left_mid - 1;
            printf(" New search area, left = %d, right = %d.\n", left, right);
            continue;
        }

        if (key > right_mid_elem) {
            left = right_mid + 1;
            printf(" New search area, left = %d, right = %d.\n", left, right);
        }
        else {
            left = left_mid + 1;
            right = right_mid - 1;
            printf(" New search area, left = %d, right = %d.\n", left, right);
        }
    }
    printf(" Key not found in array.\n");
    return -1;
}

int interpolation_search(int *array, int key, int level) {
    int counter = 0, steps = 0;
    int low = 0, high = (level - 1) / 2, position;
    printf("->Searching for key %d\n", key);

    while (low <= high && key >= array[low] && key <= array[high]) {
        steps++;
        printf("Step %d:\n", steps);
        position = low + (high - low) * (key - array[low]) / (array[high] - array[low]);

        counter++;
        printf(" Access to element on index %d.\n", position);
        if (key == array[position]) {
            printf(" Key found in array at index %d.\n", position);
            return counter;
        }

        if (key < array[position]) {
            printf(" New search area, low = %d, high = %d.\n", low, high);
            high = position - 1;
        } else {
            printf(" New search area, low = %d, high = %d.\n", low, high);
            low = position + 1;
        }
    }
    printf(" Key not found in array.\n");
    return -1;
}

void compare_performance(int ternary_steps, int interpolation_steps) {
    if (ternary_steps > interpolation_steps) {
        printf("Interpolation search have better performance!\n");
    }else if (ternary_steps < interpolation_steps){
        printf("Ternary search have better performance!\n");
    }else{
        printf("Both search have same performance!\n");
    }
}

int main() {
    int choice, ternary_steps, interpolation_steps;
    int *keys = NULL, *array = NULL;
    int first_choice = 1;
    while (1) {
        menu();
        scanf("%d", &choice);
        if (first_choice && choice == 4) {
            printf("You need to make a triangle first!!!\n");
            first_choice = 0;
        }
        switch (choice) {
            case 1:
                printf("Making triangle....\n");
                printf("Enter level of triangle: ");
                int level;
                scanf("%d", &level);
                array = make_level_of_triangle(level);
                keys = make_sequence_of_keys();
                break;
            case 2:
                printf("Start searching....\n");
                printf(" Ternary search:\n");
                int flag;
                ternary_steps = search(array, keys, level + 1, ternary_steps, 0, flag = 1);
                printf("\nInterpolation search:\n");
                interpolation_steps = search(array, keys, level + 1, 0, interpolation_steps, flag = 0);
                break;
            case 3:
                printf("Printing results....\n");
                printf("Ternary search = %d average steps.\n", ternary_steps);
                printf("Interpolation search = %d average steps.\n", interpolation_steps);
                putchar('\n');
                printf("Comparing performances....\n");
                compare_performance(ternary_steps, interpolation_steps);
                printf("If you want to start again enter 4 or enter 0 and exit.\n");
                break;
            case 4:
                printf("Deallocating memory....\n");
                free(keys);
                free(array);
                menu();
                break;
            case 0:
                printf("Exiting....\n");
                exit(0);
            default:
                printf("Wrong choice, try again!\n");
                menu();
                scanf("%d", &choice);
                break;
        }
    }
}
