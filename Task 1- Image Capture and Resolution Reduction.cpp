/******************************************************************************
EE245 – Project 2  	Nathnael Minuta and Omar Elkenawy        03/15/2024

Task1
*******************************************************************************/
// Include STB Image and STB Image Write implementations
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <vector>
using namespace std;
// Function to convert pixel intensity to extended ASCII character
char intensityToChar(int intensity, int threshold) {
    // Define an array of extended ASCII characters
    const char asciiChars[] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '8', '@'};
    // Calculate the index based on the intensity and threshold
    int index = (intensity > threshold) ? (intensity - threshold) / ((255 - threshold) / (sizeof(asciiChars) - 1)) : 0;
    // Return the corresponding extended ASCII character
    return asciiChars[index];
}


int main() {
    // Declare variables to store image information
    int width, height, chan;
    // Create a vector to store the averaged pixel values of the resized image
    vector<int> pixelAverages;
    // Load the original image into memory using STB Image
    unsigned char *my_image = stbi_load("OENM.JPG", &width, &height, &chan, 1);
    if (my_image == NULL) {
        cout << "Error loading image...\n";
        exit(1);
    }
    // Set the desired size for the resized image
    int desiredSize = 48;
    // Calculate the width and height ratio for averaging neighboring pixels
    int widthRatio = width / desiredSize;
    int heightRatio = height / desiredSize;
    int pixelSum = 0;
    // Loop through the pixels of the resized image
    // Loop through the rows of the resized image
    for (int i = 1; i <= desiredSize; i++) {
        // Loop through the columns of the resized image
        for (int a = 1; a <= desiredSize; a++) {
            // Nested loop: Loop through the horizontal pixels in the region
            for (int j = 0; j < widthRatio; j++) {
                // Nested loop: Loop through the vertical pixels in the region
                for (int c = 0; c < heightRatio; c++) {
                    // Calculate the index of the corresponding pixel in the original image
                    int originalIndex = 720 * (c + heightRatio * (i - 1)) + j + widthRatio * (a - 1);
                    // Sum the pixel values of the corresponding region in the original image
                    pixelSum += static_cast<int>(my_image[originalIndex]);
                }
            }
            // Calculate the average pixel value for the region and store it in the vector
            pixelAverages.push_back(pixelSum / (widthRatio * heightRatio));
            // Reset pixelSum for the next region
            pixelSum = 0;
        }
    }
    // Free the memory allocated for the original image
    stbi_image_free(my_image);
    // Set a threshold value to determine black or white
    int threshold = 128;
    // Print characters based on pixel intensity
    for (vector<int>::size_type i = 0; i < pixelAverages.size(); i++) {
        cout << intensityToChar(pixelAverages[i], threshold);
        // Print a newline character after every row
        if ((i + 1) % desiredSize == 0) {
            cout << endl;
        }
    }
    return 0;
}
