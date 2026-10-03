#include <iostream>
#include <array>
#include <map>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include "PixelClassifier.h"

using namespace std;

//Constructor sets total templates to 0, as we have not added any templates yet
PixelClassifier::PixelClassifier() {
    numTemplates = 0; 
}

BoundingBox PixelClassifier::cropToBoundingBox(int grid[][max_grid_size], int rows, int cols){
    /* This creates a box and the box position
    MinRow = top edge
    MaxRow = bottom edge
    MinCol = left edge
    MaxCol = right edge
    The -1 value and the rows/cols we want the pixel find to replace these values
    */
    BoundingBox box;
    box.minRow = rows;
    box.maxRow = -1;    
    box.minCol = cols;
    box.maxCol = -1;
    /* Scan every outer pixel for outer loop
    Inner loop goes through every column*/
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            /* Check for symbol pixel
            Each of the each statement check and update each edges of the box*/
            if (grid[r][c] != 0) {
                if (r < box.minRow) box.minRow = r;
                if (r > box.maxRow) box.maxRow = r;
                if (c < box.minCol) box.minCol = c;
                if (c > box.maxCol) box.maxCol = c;
            }
        }
    }
    return box;
}
void PixelClassifier::resizeToTargetSize(int grid[][max_grid_size], BoundingBox box, double outputFeatures[feature_size]){
    
    //Target is 16x16
    for (int r = 0; r < target; r++){
        for (int c = 0; c < target; c++){

            /*Which row/col in the original grid corresponds to the target grid.
            box.maxRow-box.minRow calculates how tall the bounding box is
            (r * )/ target calculates how far down we are as a fraction
            + box.minRow is the offset from the top of the bounding box
            */
            int height = box.maxRow - box.minRow + 1;
            int width = box.maxCol - box.minCol + 1;

            int newRow = box.minRow + (r * height / target);
            int newCol = box.minCol + (c * width / target);

            //Prevents index from going out of bounds
            newRow = min(newRow, box.maxRow);
            newCol = min(newCol, box.maxCol);
            //Reads the item in original grid and sets the correct index in the 1D array
            //(r * target + c) How to turn 2D cords into 1D index
            outputFeatures[r * target + c] = grid[newRow][newCol];

        }
    }
}
/* this is to get extra info about the pixel like its density, symmetry and center
*/

StructuralFeatures PixelClassifier::extractStructuralFeatures(double features[feature_size]) {

    StructuralFeatures result = {};

    double total = 0;
    double rowSum = 0;
    double colSum = 0;

    int horizontal = 0;
    int vertical = 0;

    //goes through every row and column and converts the 2D pixel location into a 1D array
    for(int r = 0; r < target; r++) {
        for(int c = 0; c < target; c++) {
            double value = features[r * target + c];

            //this counts the total number of filled pixels
            total += value;

            //used to find where the center is,
            rowSum += r * value;
            colSum += c * value;
            
            //compares top with bottom
            if(value == features[(target - r - 1) * target + c]) {
                horizontal++;
            }
            
            //compares right side with left side
            if(value == features[r * target + (target - c - 1)]) {
                vertical++;
            }
        }
    }
    
    //calculates how much the grid is filled
    result.density = total / feature_size;

    if(total != 0) {
        //find row position avg 
        result.centerMassRow = rowSum / total;
        //find column position avg
        result.centerMassCol = colSum / total;
    }
    //finds symmetry percentage
    result.symmetryHorizontal = (double)horizontal / feature_size;
    result.symmetryVertical = (double)vertical / feature_size;

    return result;
}

double PixelClassifier::cosineSimilarity(double features1[feature_size], double features2[feature_size]){
    /*calculates how similar two pixels are,  measures how much two symbols overlap
    */

    //stores multiplied values between matching pixel positions
    double dot = 0;
    //stores magnitude of first and second vector
    double mag1 = 0;
    double mag2 = 0;

    //compares all 256 pixels
    for(int i = 0; i < feature_size; i++) {
        //dot product
        //
        dot += features1[i] * features2[i];
        mag1 += features1[i] * features1[i];
        mag2 += features2[i] * features2[i];

    }

    if(mag1 == 0 || mag2 == 0)
        return 0;

    return dot / (sqrt(mag1) * sqrt(mag2));
}

double PixelClassifier::distance(double features1[feature_size], double features2[feature_size]){
/* calculates and checks the actual distance between pixels
*/
    double sum = 0;

    for(int i = 0; i < feature_size; i++) {
        //find the difference between two matching pixels, difference = 0 if they match
        double difference = features1[i] - features2[i];
        //we square the difference because positive and negative differences should both increase distance
        sum += difference * difference;
    }
    //changes the squared distance back to normal
    return sqrt(sum);
}

double PixelClassifier::calculateConfidence(double cosineSim, double distance, StructuralFeatures& input, StructuralFeatures& templateFeatures){
    //Calculate similarity betweeen input and template's structural features
    double distancescore = 1.0 / (1.0 + distance);
    double symHscore = 1.0 - fabs(input.symmetryHorizontal - templateFeatures.symmetryHorizontal);
    double symVscore = 1.0 - fabs(input.symmetryVertical - templateFeatures.symmetryVertical);
    double densityScore = 1.0 - fabs(input.density - templateFeatures.density);

    /*Calculate Confidence
    Cosine similarity is given the most weight because it is the most direct comparison. 
    */
    double confidence = (cosineSim * 0.5) + (distancescore * 0.2) + (symHscore * 0.1) + (symVscore * 0.1) + (densityScore * 0.1);

    return confidence;
}

void PixelClassifier::addTemplate(const char* label, int grid[][max_grid_size], int rows, int cols){
    //Get edges of the known symbol
    BoundingBox box = cropToBoundingBox(grid, rows, cols);
    //Resize the image to 16x16
    resizeToTargetSize(grid, box, templates[numTemplates].features);
    //String copy the template 
    strcpy(templates[numTemplates].label, label);
    //Extract structural features for future comparison
    templates[numTemplates].structFeatures = extractStructuralFeatures(templates[numTemplates].features);
    //Add to the total # of templates stored
    numTemplates++;
}

ClassifyResult PixelClassifier::classify(int grid[][max_grid_size], int rows, int cols){
    //Crop and resize the input grid to 16x16
    BoundingBox box = cropToBoundingBox(grid, rows, cols);
    double inputFeatures[feature_size];
    resizeToTargetSize(grid, box, inputFeatures);
    
    //Extract structural features
    StructuralFeatures inputStructFeatures = extractStructuralFeatures(inputFeatures);

    //Classification result to be returned
    ClassifyResult result = {};
    double bestConfidence = -1.0;
    double secondBestConfidence = -1.0;

    //Compare input against all templates
    for (int i = 0; i < numTemplates; i++){
        //Calculate similarity
        double cosineSim = cosineSimilarity(inputFeatures, templates[i].features);
        double dist = distance(inputFeatures, templates[i].features);

        //Calculate confidence
        double score = calculateConfidence(cosineSim, dist, inputStructFeatures, templates[i].structFeatures);
        
        //Track the best match
        if (score > bestConfidence){
            //Current best becomes the runner up
            secondBestConfidence = bestConfidence;
            strcpy(result.runnerUpLabel, result.label);
            result.runnerUpConfidence = bestConfidence;

            //New Best Match
            bestConfidence = score;
            strcpy(result.label, templates[i].label);
            result.confidence = bestConfidence;

        }
        //Track second best match
        else if (score > secondBestConfidence){
            secondBestConfidence = score;
            strcpy(result.runnerUpLabel, templates[i].label);
            result.runnerUpConfidence = secondBestConfidence;
        }
    }
    //Confidence threshold to reject low confidence matches, this can be adjusted based on testing
    //UPDATE: I found 0.5 to be too low of a threshold. The classifier classified a random grid as a symbol with 63% confidence. 
    //0.70 is a much better threshold, confirming perfect scores and preventing noisy grids from being accepted.
    result.rejected = (bestConfidence < 0.70);
    return result; 

}

void PixelClassifier::printGrid(int grid[][max_grid_size],  int rows, int cols){
    //Just print the grid
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            cout << grid[r][c] << " ";
        }
        cout << '\n';
    }
}

void PixelClassifier::printResult(const ClassifyResult& result){
    cout << "Prediction: " << result.label << '\n';
    cout << "Confidence: " << result.confidence  * 100 << "%" << '\n';
    cout << "Runner Up: " << result.runnerUpLabel << '\n';
    cout << "Runner Up Confidence: " << result.runnerUpConfidence * 100 << "%" << '\n';
    if (result.rejected) {
        cout << "The confidence is too low" << '\n';
    }
    else {
        cout << "Result accepted" << '\n';
    }

}

int PixelClassifier::getTemplateCount(){
    return numTemplates;
}

