#include <stdint.h>

// Run length decode

int32_t* run_length_decode( int32_t* input, unsigned long input_length, unsigned long* output_length ) {
	(*output_length) = 0;

	if( input_length % 2 != 0 ) {
		fprintf( stderr, "Error in calling run-length decode: you are calling run-length decode with a odd number of values\n" );
		return NULL;
	}

	int i;
	int32_t value, number;
	for(i = 0; i < input_length; i += 2) {
		value = input[i];
		number = input[i+1];

		(*output_length) += number;
	}

	int32_t* output = malloc(sizeof(int32_t)*(*output_length)); // The output needs to be freed by the calling process

	int j = 0;
	int k;
	for(i = 0; i < input_length; i += 2) {
		value = input[i];
		number = input[i+1];

		for(k = 0; k < number; ++k) {
			output[j] = value;
			++j;
		}
	}

	return output;
}

// Delta decode

int32_t* delta_decode( int32_t* input, unsigned long input_length, unsigned long* output_length ) {
	(*output_length) = input_length;
	int32_t* output = malloc(sizeof(int32_t)*(*output_length)); // The output needs to be freed by the calling process

	output[0] = input[0];
	int i;
	for(i = 1; i < input_length; ++i) {
		output[i] = output[i-1] + input[i];
	}

	return output;
}

// Recursive indexing decode

typedef struct list_int32_t list_int32_t;

struct list_int32_t {
	list_int32_t* previous;
	uint32_t value;
	list_int32_t* next;
};

int32_t* recursive_indexing_decode( int16_t* input, unsigned long input_length, unsigned long* output_length ) {
	list_int32_t* start = malloc( sizeof(list_int32_t) );

	list_int32_t* current = start;
	(*current).previous = NULL;
	(*current).value = 0;
	(*current).next = NULL;

	(*output_length) = 1;

	int i;
	bool adding_next = true;
	for(i = 0; i < input_length; ++i) {
		if( ! adding_next ) {
			list_int32_t* next = malloc( sizeof(list_int32_t) );
			(*current).next = next;
			(*next).previous = current;
			(*next).value = 0;
			(*next).next = NULL;
			current = next;
			++(*output_length);
		}

		(*current).value += input[i];

		adding_next = ( input[i] == INT16_MAX || input[i] == INT16_MIN );
	}

	int32_t* output = malloc(sizeof(int32_t)*(*output_length)); // The output needs to be freed by the calling process

	int j = 0;

	current = start;
	while( current != NULL ) {
		output[j] = (*current).value;
		list_int32_t* next = (*current).next;
		free(current);
		current = next;
		++j;
	}

	return output;
}

// Integer decoding

float* integer_decode( int32_t* input, unsigned long input_length, int32_t parameter, unsigned long* output_length ) {
	(*output_length) = input_length;
	float* output = malloc( sizeof(float) * (*output_length) );

	float parameter_float = (float) parameter;
	int i;
	for(i = 0; i < input_length; ++i ) {
		output[i] = ((float) input[i])/parameter_float;
	}

	return output;
}
