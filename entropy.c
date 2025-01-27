#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "stb_image/stb_image.h"


double calculateEntropyImage(char image_path[300]){
    int width , height , channels; 
    unsigned char* image_data = stbi_load(image_path, &width ,&height, &channels, 0);
    if (!image_data){
        printf("Failed to load image\n") ; 
    }
    int size = width* height ; 
    unsigned char* pixels= image_data ; 
    
    double entropy = 0 ; 
    int histogram[256] = {0} ; 
    for (int i=0; i<size; i++){
        histogram[pixels[i]]++ ; 
    }
    for (int i=0; i<256; i++){
        double p = (double) histogram[i] / size ;
        if (p> 0) {
            entropy -= p* log2(p) ; 
        } 
    } 
    stbi_image_free(image_data); 
    return entropy; 
}


int main() {
    //char image_path[300];
    //printf("Enter the path to your image : ") ; 
    // scanf("%s" , &image_path) ;  
    char path_image[300] ; //Enter here the path of the initial image before encryption
    char path_encrypted_image[300];  //and here paste the path of the image after ecryption ,  in this case i already have the two image im only gonna try to see how the entropy vary after we proceed the encryption , 
    double entropy_img = calculateEntropyImage(path_image); 
    double entropy_encr_img = calculateEntropyImage(path_encrypted_image); 
    printf("Entropy of the image before the encryption : %f\n", entropy_img) ;
    printf("Entropy of the image after the encryption : %f\n", entropy_encr_img) ;
}