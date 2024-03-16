/******************************************************************************
EE245 – Project 2  	Nathnael Minuta and Omar Elkenawy        03/15/2024

Task2
*******************************************************************************/
// Include STB Image and STB Image Write implementations
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
// Function to apply Sobel Edge Detection to an image
void applySobel(const unsigned char* inputImage, int width, int height, unsigned char* outputImage, int threshold) {
    // Sobel kernels for horizontal and vertical edge detection
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    // Iterate over the pixels of the input image
    for (int i = 0; i < height - 2; ++i) {
        for (int j = 0; j < width - 2; ++j) {
            double Gxsum = 0.0, Gysum = 0.0;
            // Apply the Sobel kernels
            for (int ky = 0; ky < 3; ++ky) {
                for (int kx = 0; kx < 3; ++kx) {
                    int pixelIndex = (i + ky) * width + (j + kx);
                    Gxsum += Gx[ky][kx] * inputImage[pixelIndex];
                    Gysum += Gy[ky][kx] * inputImage[pixelIndex];
                }
            }
            // Calculate the gradient magnitude and set it as the pixel value
            int gradientMagnitude = static_cast<int>(sqrt(Gxsum * Gxsum + Gysum * Gysum));
            outputImage[i * (width - 2) + j] = (gradientMagnitude > threshold) ? '*' : ' ';
        }
    }
}
int main() {
    // Declare variables to store image information
    int originalWidth, originalHeight, originalChannels;
    // Load the original image into memory using STB Image
    unsigned char *originalImage = stbi_load("OENM.JPG", &originalWidth, &originalHeight, &originalChannels, 1);
    if (originalImage == NULL) {
        cout << "Error loading image...\n";
        exit(1);
    }
    // Set the desired size for the resized image
    int desiredSize = 48;
    // Calculate the width and height ratio for resizing
    int widthRatio = originalWidth / desiredSize;
    int heightRatio = originalHeight / desiredSize;
    // Create a buffer for resized image
    vector<unsigned char> resizedImage(desiredSize * desiredSize);

    // Loop through the pixels of the resized image
    for (int i = 0; i < desiredSize; i++) {
        for (int j = 0; j < desiredSize; j++) {
            // Calculate the index of the corresponding pixel in the original image
            int originalIndex = originalWidth * (i * heightRatio) + j * widthRatio;
            // Copy pixel value to the resized image
            resizedImage[i * desiredSize + j] = originalImage[originalIndex];
        }
    }
    // Free the memory allocated for the original image
    stbi_image_free(originalImage);
    // Create a buffer for Sobel Edge Detection output
    vector<unsigned char> sobelOutput((desiredSize - 2) * (desiredSize - 2));
    // Apply Sobel Edge Detection with a higher threshold
    applySobel(resizedImage.data(), desiredSize, desiredSize, sobelOutput.data(), 200);
    // Print characters based on pixel intensity (Sobel Edge Detected)
    for (int i = 0; i < (desiredSize - 2); i++) {
        for (int j = 0; j < (desiredSize - 2); j++) {
            // Check if the pixel is an edge or has a significant intensity difference
            if (sobelOutput[i * (desiredSize - 2) + j] == '*') {
                cout << '*';
            } else {
                // Print characters based on pixel intensity
                cout << ' ';
            }
        }
        cout << '\n';  // Move to the next line after each row
    }
// Write Sobel Edge Detection output to an image file
stbi_write_png("sobel_output.png", desiredSize - 2, desiredSize - 2, 1, sobelOutput.data(), (desiredSize - 2) * sizeof(unsigned char));
    return 0;
}
