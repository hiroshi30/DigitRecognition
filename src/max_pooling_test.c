#include <kazamori.h>
#include <print.h>


int main(int argc, char* argv[]) {
	MaxPooling* layer = MaxPooling_construct(
		2, // channels
		4, // height
		6, // width
		2, // matrix_h
		3  // matrix_w
	);
	MaxPooling_train_construct(layer);
	ImageSet* image_set = ImageSet_construct(
		1, // length
		2, // channels
		4, // height
		6, // width
		2  // output_length
	);

	ImageSet_add(image_set,
		(double[]){
			// input
			1, 2, 3, 4, 5, 7,
			6, 7, 8, 9, 10, 12,
			11, 12, 13, 14, 15, 2,
			16, 17, 18, 19, 20, 21,

			2, 4, 6, 8, 10, 9,
			12, 14, 16, 18, 20, 12,
			22, 24, 26, 28, 30, 0,
			32, 34, 36, 38, 40, 11
		}, (double[]){
			// output
			5, 17
		}
	);

	ImageSet_print_input(image_set);
	
	MaxPooling_forward(layer, image_set->input[0]);
	MaxPooling_print_output(layer);

	int channels = 2;
	int height = 2;
	int width = 2;
	double arr[] = {
		0, 1,
		1, 1,

		0, 0,
		2, -1
	};
	double*** ptr = (double***)malloc(sizeof(double**) * channels);
	for (int c = 0; c < channels; ++c) {
		ptr[c] = (double**)malloc(sizeof(double*) * height);
		for (int h = 0; h < height; ++h) {
			ptr[c][h] = (double*)malloc(sizeof(double) * width);
			for (int w = 0; w < width; ++w) {
				ptr[c][h][w] = arr[c * height * width + h * width + w];
			}
		}
	}

	MaxPooling_backward(layer, ptr);
	MaxPooling_print_err(layer);

	ImageSet_destruct(image_set);
	MaxPooling_train_destruct(layer);
	MaxPooling_destruct(layer);
	
	return 0;
}