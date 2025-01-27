#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FILTER_SIZE 3

float convolution(float *image, int i, int j, int width, float *filter)
{
    float sum = 0;
    int r, c;
    for (r = -FILTER_SIZE/2; r <= FILTER_SIZE/2; r++) {
        for (c = -FILTER_SIZE/2; c <= FILTER_SIZE/2; c++) {
            int row = i + r;
            int col = j + c;
            if (row < 0 || row >= width || col < 0 || col >= width) {
                continue;
            }
            sum += image[row*width + col] * filter[(r+FILTER_SIZE/2)*FILTER_SIZE + (c+FILTER_SIZE/2)];
        }
    }
    return sum;
}

int main()
{
    int width, height, channels;
    float *image, *filter, *correlation_map;
    int i, j;

    // Load the image file
    image = stbi_loadf("index.jpg", &width, &height, &channels, 0);
    if (image == NULL) {
        printf("Error: could not load image file.\n");
        return 1;
    }

    // Allocate memory for the filter and correlation map
    filter = (float *)malloc(FILTER_SIZE*FILTER_SIZE*sizeof(float));
    correlation_map = (float *)malloc(width*height*sizeof(float));
    if (filter == NULL || correlation_map == NULL) {
        printf("Error: could not allocate memory.\n");
        return 1;
    }

    // Initialize the filter to a simple averaging kernel
    for (i = 0; i < FILTER_SIZE*FILTER_SIZE; i++) {
        filter[i] = 1.0 / (float)(FILTER_SIZE*FILTER_SIZE);
    }

    // Calculate the correlation map
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            correlation_map[i*width + j] = convolution(image, i, j, width, filter);
        }
    }

    // Write the correlation map to file
    FILE *fp = fopen("correlation.dat", "wb");
    fwrite(correlation_map, sizeof(float), width*height, fp);
    fclose(fp);

    // Display the correlation map
    unsigned char *correlation_map_uc = (unsigned char *)malloc(width*height*sizeof(unsigned char));
    for (i = 0; i < width*height; i++) {
        correlation_map_uc[i] = (unsigned char)(255.0 * fabs(correlation_map[i]));
    }
    stbi_write_png("correlation.png", width, height, 1, correlation_map_uc, width);
    free(correlation_map_uc);

    // Free memory
    stbi_image_free(image);
    free(filter);
    free(correlation_map);

    return 0;
}
