#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// A structure to hold statistics results.
typedef struct {
    float *values;
    int num_values;
    int capacity;
} DynamicArray;

// Function to initialize the dynamic array.
DynamicArray* init_dynamic_array() {
    DynamicArray *arr = malloc(sizeof(DynamicArray));
    if (!arr) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    arr->values = malloc(20 * sizeof(float));  // Initial capacity of 20
    if (!arr->values) {
        free(arr);
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    arr->num_values = 0;
    arr->capacity = 20;
    return arr;
}

// Function to add an element to the dynamic array.
void add_value(DynamicArray *arr, float value) {
    // Check if the array needs to be resized.
    if (arr->num_values == arr->capacity) {
        int new_capacity = arr->capacity * 2;
        float *new_values = realloc(arr->values, new_capacity * sizeof(float));
        if (!new_values) {
            free(arr->values);
            free(arr);
            fprintf(stderr, "Memory reallocation failed!\n");
            exit(EXIT_FAILURE);
        }
        arr->values = new_values;
        arr->capacity = new_capacity;
    }
    arr->values[arr->num_values++] = value;
}

// Function to free the dynamic array.
void free_dynamic_array(DynamicArray *arr) {
    free(arr->values);
    free(arr);
}

// Function to compare floats for qsort.
int compare_floats(const void *a, const void *b) {
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb);
}

// Function to calculate the mean of the dynamic array.
float calculate_mean(DynamicArray *arr) {
    float sum = 0.0;
    int i;
    for (i = 0; i < arr->num_values; i++) {
        sum += arr->values[i];
    }
    return arr->num_values ? (sum / arr->num_values) : 0.0;
}

// Function to calculate the median of the dynamic array.
float calculate_median(DynamicArray *arr) {
    qsort(arr->values, arr->num_values, sizeof(float), compare_floats);
    int mid = arr->num_values / 2;
    if (arr->num_values % 2 == 0) {
        // Average of middle two values if even number of values
        return (arr->values[mid - 1] + arr->values[mid]) / 2.0;
    } else {
        // Middle value if odd number of values
        return arr->values[mid];
    }
}

// Function to calculate the standard deviation of the dynamic array.
float calculate_stddev(DynamicArray *arr, float mean) {
    float sum_sq_diff = 0.0;
    int i;
    for (i = 0; i < arr->num_values; i++) {
        sum_sq_diff += (arr->values[i] - mean) * (arr->values[i] - mean);
    }
    return sqrt(sum_sq_diff / arr->num_values);
}

// Function to calculate the mode of the dynamic array.
float calculate_mode(DynamicArray *arr) {
    qsort(arr->values, arr->num_values, sizeof(float), compare_floats);
    
    float mode = arr->values[0];
    int current_count = 1;
    int max_count = 1;
    int i;

    for (i = 1; i < arr->num_values; i++) {
        if (arr->values[i] == arr->values[i - 1]) {
            current_count++;
        } else {
            if (current_count > max_count) {
                max_count = current_count;
                mode = arr->values[i - 1];
            }
            current_count = 1; // reset count for the new number
        }
    }

    // Check if the last number is the most frequent
    if (current_count > max_count) {
    	max_count = current_count;
        mode = arr->values[arr->num_values - 1];
    }

    return mode;
}


// Function to calculate the N-th root of a using Newton's method
float nth_root(float a, int N) {
    float x = 1; // Initial guess
    int i, j;
    for (i = 0; i < 100; i++) { // Iterate to get better approximations
        float xN_minus_1 = x;
        for (j = 1; j < N-1; j++)
            xN_minus_1 *= x;
        x = (1.0 / N) * ((N - 1) * x + a / xN_minus_1);
    }
    return x;
}

//Function tO calculate the natural logarithm of a given number x, ln(x)
float my_ln(float x) {
    if (x <= 0) {
        return -1;  // Logarithm not defined for non-positive values
    }

    int n = 0;
    while (x > 2) {
        x /= 2.718281828459045;  // Using a precise value of 'e'
        n++;
    }
    x -= 1;

    float term = x;
    float result = 0;
    int i;  // Declare loop variable outside the 'for' loop
    for (i = 1; i <= 20; ++i) {  // Increased iterations for accuracy
        result += term / i;
        term *= -x;
    }

    // Adjust the result after range adjustment, using 'ln(e) = 1'
    result += n * 1.0;
    return result;
}

//Function to calculate an approximation of the exponential function for a given number x
float my_exp(float x) {
    float result = 1;
    float term = 1;
    int i;  // Declare loop variable outside the 'for' loop
    for (i = 1; i <= 20; ++i) {
        term *= x / i;
        result += term;
    }
    return result;
}

// Function to calculate the geometric mean
float calculate_geometric_mean(float *values, int num_values) {
    if (num_values == 0) return 0;  // Handle empty array case

    float log_sum = 0.0;
    int i;  // Declare loop variable outside the 'for' loop
    for (i = 0; i < num_values; ++i) {
        float log_value = my_ln(values[i]);
        if (log_value == -1) {
            return -1;  // Return error code for invalid input
        }
        log_sum += log_value;
    }
    float log_mean = log_sum / num_values;
    return my_exp(log_mean);
}

// Function to calculate the harmonic mean of the dynamic array
float calculate_harmonic_mean(float *values, int num_values) {
    if (num_values == 0) return 0; // Handle empty array case

    float sum_reciprocals = 0.0;
    int i;
    for (i = 0; i < num_values; i++) {
        if (values[i] == 0) {
            fprintf(stderr, "Harmonic mean is not defined for zero values.\n");
            return -1;
        }
        sum_reciprocals += 1.0 / values[i];
    }
    return num_values / sum_reciprocals;
}


// Function to read the values from a file into the dynamic array.
DynamicArray* read_values_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    DynamicArray *arr = init_dynamic_array();
    float value;
    while (fscanf(file, "%f", &value) == 1) {
        add_value(arr, value);
    }

    fclose(file);
    return arr;
}

int main(int argc, char *argv[]) {
    // Ensure proper usage
    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename.txt\n", argv[0]);
        return 1;
    }

    // Read values from file
    DynamicArray *arr = read_values_from_file(argv[1]);

    // Calculate statistics
    float mean = calculate_mean(arr);
    float median = calculate_median(arr);
    float stddev = calculate_stddev(arr, mean);
    float mode = calculate_mode(arr);
    float geometric_mean = calculate_geometric_mean(arr->values, arr->num_values);
    float harmonic_mean = calculate_harmonic_mean(arr->values, arr->num_values);    

    // Print statistics
    printf("Results:\n");
    printf("Num values: %d\n", arr->num_values);
    printf("mean: %.3f\n", mean);
    printf("median: %.3f\n", median);
    printf("stddev: %.3f\n", stddev);
    printf("mode: %.3f\n", mode);
    printf("Geometric Mean: %.3f\n", geometric_mean);
    printf("Harmonic Mean: %.3f\n", harmonic_mean);
    printf("Unused array capacity: %d\n", arr->capacity - arr->num_values);

    // Free the dynamic array
    free_dynamic_array(arr);

    return 0;
}

