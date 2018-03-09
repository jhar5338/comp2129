#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <float.h>

#define IDX(x, y) ((y) * g_width + (x))

#include "matrix.h"

static int g_seed = 0;

static ssize_t g_width = 0;
static ssize_t g_height = 0;
static ssize_t g_elements = 0;

static ssize_t g_nthreads = 1;

////////////////////////////////
///      FLOAT FUNCTIONS     ///
////////////////////////////////

/**
 * Sees if floats are equal to a certain precision
 */
 bool approx_equals(float a, float b) {
	return fabsf((a - b)) <= FLT_EPSILON;
}

/**
 * Sees if float a > float b to a certain precision
 */
 bool greater_than(float a, float b) {
    return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * FLT_EPSILON);
}
////////////////////////////////
///     UTILITY FUNCTIONS    ///
////////////////////////////////

/**
 * Returns pseudorandom number determined by the seed.
 */
int fast_rand(void) {

	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}

/**
 * Sets the seed used when generating pseudorandom numbers.
 */
void set_seed(int seed) {

	g_seed = seed;
}

/**
 * Sets the number of threads available.
 */
void set_g_nthreads(ssize_t count) {

	g_nthreads = count;
}

/**
 * Sets the dimensions of the matrix.
 */
void set_dimensions(ssize_t order) {

	g_width = order;
	g_height = order;

	g_elements = g_width * g_height;
}

/**
 * Displays given matrix.
 */
void display(const float* matrix) {

	for (ssize_t y = 0; y < g_height; y++) {
		for (ssize_t x = 0; x < g_width; x++) {
			if (x > 0) printf(" ");
			printf("%.2f", matrix[y * g_width + x]);
		}

		printf("\n");
	}
}

/**
 * Displays given matrix row.
 */
void display_row(const float* matrix, ssize_t row) {

	for (ssize_t x = 0; x < g_width; x++) {
		if (x > 0) printf(" ");
		printf("%.2f", matrix[row * g_width + x]);
	}

	printf("\n");
}

/**
 * Displays given matrix column.
 */
void display_column(const float* matrix, ssize_t column) {

	for (ssize_t i = 0; i < g_height; i++) {
		printf("%.2f\n", matrix[i * g_width + column]);
	}
}

/**
 * Displays the value stored at the given element index.
 */
void display_element(const float* matrix, ssize_t row, ssize_t column) {

	printf("%.2f\n", matrix[row * g_width + column]);
}

////////////////////////////////
///   MATRIX INITALISATIONS  ///
////////////////////////////////

/**
 * Returns new matrix with all elements set to zero.
 */
float* new_matrix(void) {

	return calloc(g_elements, sizeof(float));
}

/**
 * Returns new identity matrix.
 */
float* identity_matrix(void) {

	float* result = new_matrix();

	for (ssize_t i = g_width*g_height-1; i >= 0; i-=g_width+1) 
		result[i]=1;
	
	return result;
}

/**
 * Returns new matrix with elements generated at random using given seed.
 */
float* random_matrix(int seed) {

	float* result = new_matrix();

	set_seed(seed);

	for (ssize_t i = 0; i < g_elements; i++) {
		result[i] = fast_rand();
	}

	return result;
}

/**
 * Returns new matrix with all elements set to given value.
 */
float* uniform_matrix(float value) {

	float* result = new_matrix();

	for (ssize_t i = 0; i < g_width*g_height; i++) {
		result[i] = value;
	}

	return result;
}

/**
 * Returns new matrix with elements in sequence from given start and step
 */
float* sequence_matrix(float start, float step) {

	float* result = new_matrix();
	float value = start;
	result[0] = start;
	
	for (ssize_t i = 1; i < g_width*g_height; i++) {
		result[i] = value+=step;
	}

	return result;
}

////////////////////////////////
///     MATRIX OPERATIONS    ///
////////////////////////////////

/**
 * Returns new matrix with elements cloned from given matrix.
 */
float* cloned(const float* matrix) {

	float* result = new_matrix();

	for (ssize_t y = 0; y < g_height; y++) {
		for (ssize_t x = 0; x < g_width; x++) {
			result[y * g_width + x] = matrix[y * g_width + x];
		}
	}

	return result;
}

/**
 * Returns new matrix with elements in ascending order.
 */
float* sorted(const float* matrix) {

	float* result = cloned(matrix);
	
	float temp = 0;
	for(ssize_t i = 0; i < g_width*g_height; i++) {
		for(ssize_t j = 0; j < g_width*g_height-1; j++) {
			if(greater_than(result[j],result[j+1]))	{
				temp = result[j+1];
				result[j+1] = result[j];
				result[j] = temp;
			}
		}
	}
	return result;
}

/**
 * Returns new matrix with elements rotated 90 degrees clockwise.
 */
float* rotated(const float* matrix) {
	
	float* result = transposed(matrix);
	
	if (g_width==1)
		return result;
	float temp = 0;
	
	for (ssize_t i = 0; i < g_width/2; i++) {
		// iterate down each column
		for (ssize_t j = i; j < (g_width*g_width)+i; j+=g_width) {
			// swap element from this col with corresp element from col on opposite half
			temp = result[j];
			result[j] = result[(g_width+j)-(2*i+1)];
			result[(g_width+j)-(2*i+1)] = temp;
		}
	}
	return result;
}

/**
 * Returns new matrix with elements ordered in reverse.
 */
float* reversed(const float* matrix) {

	float* result = cloned(matrix);
	float temp = 0;
	ssize_t j = g_width*g_height-1;
	for (ssize_t i = 0;i < g_width*g_height/2; i++,j--) {
		temp = result[i];
		result[i] = result[j];
		result[j] = temp;
	}
	return result;
}

/**
 * Returns new transposed matrix.
 */
float* transposed(const float* matrix) {

	float* result = cloned(matrix);
	
	if (g_width==1)
		return result;
	
	float temp = 0;
	ssize_t j = 0;
	
	for (ssize_t i = 0; i < g_width*g_height; i++) {
		if (i%g_width==0) {
			// if i is first element of row and it's not first row then 
			if(j!=0) 
				i+=j;
			continue;
		}
		// swap i with corresponding element across the diagonal
		temp = result[i];
		result[i] = result[(i%g_width)*g_width+j];
		result[(i%g_width)*g_width+j] = temp;
		// if it is the last element of that row then increment j counter
		if ((i+1)%g_width==0&&i!=0) {
			j++;
			// last row elements do not have to be changed
			if (j == g_width) 
				break;
		}
	}
	return result;
}

/**
 * Returns new matrix with scalar added to each element.
 */
float* scalar_add(const float* matrix, float scalar) {

	float* result = new_matrix();

	for (ssize_t y = 0; y < g_height; y++) {
		for (ssize_t x = 0; x < g_width; x++) {
			result[y * g_width + x] = matrix[y * g_width + x] + scalar;
		}
	}
	
	return result;
}

/**
 * Returns new matrix with scalar multiplied to each element.
 */
float* scalar_mul(const float* matrix, float scalar) {

	float* result = new_matrix();

	for (ssize_t y = 0; y < g_height; y++) {
		for (ssize_t x = 0; x < g_width; x++) {
			result[y * g_width + x] = matrix[y * g_width + x] * scalar;
		}
	}

	return result;
}

/**
 * Returns new matrix that is the result of
 * adding the two given matrices together.
 */
float* matrix_add(const float* matrix_a, const float* matrix_b) {

	float* result = new_matrix();

	for (ssize_t y = 0; y < g_height; y++) {
		for (ssize_t x = 0; x < g_width; x++) {
			result[y * g_width + x] = matrix_a[y * g_width + x] + matrix_b[y * g_width + x];
		}
	}

	return result;
}
/*void* worker_multiply(void* args) {

	worker_args* wargs = (worker_args*) args;
	ssize_t CHUNK = g_width / g_nthreads;
	const size_t start = wargs->id * CHUNK;
	const size_t end = wargs->id == g_nthreads - 1 ? g_width : (wargs->id + 1) * CHUNK;

	for (size_t i = start; i < end; i++) {
		for (size_t j = 0; j < g_width; j++) {
			for (size_t k = 0; k < g_width; k++) {
				wargs->result[IDX(j, i)] += wargs->a[IDX(k, i)] * wargs->b[IDX(j, k)];
			}
		}
	}
	return NULL;
}*/

/**
 * Returns new matrix that is the result of
 * multiplying the two matrices together.
 */
float* matrix_mul(const float* matrix_a, const float* matrix_b) {

	/*float* result = calloc(g_width * g_height, sizeof(float));

	worker_args args[g_nthreads];
	pthread_t thread_ids[g_nthreads];

	for (size_t i = 0; i < g_nthreads; i++) {
		args[i] = (worker_args) {
			.a = matrix_a,
			.b = matrix_b,
			.id = i,
			.result = result,
		};
	}

	// Launch threads
	for (size_t i = 0; i < g_nthreads; i++) {
		pthread_create(thread_ids + i, NULL, worker_multiply, args + i);
	}

	// Wait for threads to finish
	for (size_t i = 0; i < g_nthreads; i++) {
		pthread_join(thread_ids[i], NULL);
	}

	return result;*/return 0;
}

/**
 * Returns new matrix that is the result of
 * powering the given matrix to the exponent.
 */
float* matrix_pow(const float* matrix, int exponent) {

	float* result = new_matrix();

	for (ssize_t y = 0; y < g_height; y++) {
		for (ssize_t x = 0; x < g_width; x++) {
			result[y * g_width + x] = pow(matrix[y * g_width + x],exponent);
		}
	}

	return result;
}

/**
 * Returns new matrix that is the result of
 * convolving given matrix with a 3x3 kernel matrix.
 */
float* matrix_conv(const float* matrix, const float* kernel) {

	float* result = new_matrix();

	/*
		TODO

		Convolution is the process in which the values of a matrix are
		computed according to the weighted sum of each value and it's
		neighbours, where the weights are given by the kernel matrix.
	*/

	return result;
}

////////////////////////////////
///       COMPUTATIONS       ///
////////////////////////////////

/**
 * Returns the sum of all elements.
 */
float get_sum(const float* matrix) {

	float sum = 0;
	
	for (ssize_t y = 0; y < g_height; y++) {
		for (ssize_t x = 0; x < g_width; x++) {
			sum+=matrix[y * g_width + x];
		}
	}
	return sum;
}

/**
 * Returns the trace of the matrix.
 */
float get_trace(const float* matrix) {

	if (g_width == 1) 
		return matrix[0];
	
	float sum  = 0;
	
	for (ssize_t i = 0; i < g_width; i++) 
		sum += matrix[(i*g_width)+i];
	
	return sum;
}

/**
 * Returns the smallest value in the matrix.
 */
float get_minimum(const float* matrix) {

	if (g_width == 1) 
		return matrix[0];
	
	float min = matrix[0];
	
	for (ssize_t i = 0; i < g_width*g_height; i++) {
		if (greater_than(min,matrix[i]))
			min = matrix[i];
	}
	return min;
}

/**
 * Returns the largest value in the matrix.
 */
float get_maximum(const float* matrix) {

	if (g_width == 1) 
		return matrix[0];
	
	float max = matrix[0];
	
	for (ssize_t i = 0; i < g_width*g_height; i++) {
		if (greater_than(matrix[i],max))
			max = matrix[i];
	}
	return max;
}

/**
 * Returns the determinant of the matrix.
 */
float get_determinant(const float* matrix) {

	

	return 0;
}

/**
 * Returns the frequency of the given value in the matrix.
 */
ssize_t get_frequency(const float* matrix, float value) {
	
	if (g_width == 1) {
		if (approx_equals(matrix[0],value)) 
			return 1;
	}
	ssize_t freq = 0;
	
	for (ssize_t i = 0; i < g_width*g_height; i++) {
		if (approx_equals(matrix[i],value))
			freq++;
	}
	return freq;
}