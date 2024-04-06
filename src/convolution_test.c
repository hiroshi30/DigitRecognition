#include <kazamori.h>
#include <print.h>


int main() {
	Convolution* layer = Convolution_construct(
		3, // channels
		5, // height
		7, // width
		
		2, // matrix_c
		2, // matrix_h
		2, // matrix_w
		
		1, // padding
		
		1, // stride_h
		1  // stride_w
	);
	Convolution_train_construct(layer);

	ImageSet* image_set = ImageSet_construct(1, 3, 5, 7, 2);
	ImageSet_add(image_set,
	(double[]){
		-10, -6, -4, -8, -10, -9, -2,
		1, 8, -5, -1, -2, 0, 6,
		3, 2, -6, -3, 10, -2, 4,
		5, -3, -8, 9, -6, -3, 6,
		-6, 2, 0, -9, -1, 0, -6,

		-6, -9, 10, 3, -1, 9, 4,
		1, 8, 8, -2, 4, 3, 1,
		0, 10, -3, -7, -7, 9, -5,
		9, 1, 8, 6, 1, -4, 5,
		-10, -4, -1, 3, -7, 8, 6,

		3, -2, -3, 8, 0, -4, -2,
		-4, -2, 5, 1, 6, -10, 6,
		7, 3, -1, -10, 1, 2, -2,
		-4, 0, 6, -8, -3, 3, -1,
		9, -6, -10, -9, 5, -10, 2

	},
	(double[]){
		1, 0
	});

	Convolution_forward(layer, image_set->input[0]);
	Convolution_print_output(layer);

	ImageSet_destruct(image_set);
	Convolution_train_destruct(layer);
	Convolution_destruct(layer);
	
	return 0;
}