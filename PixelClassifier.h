#pragma once
#ifndef PIXELCLASSIFIER_H
#define PIXELCLASSIFIER_H

#include <string>

using namespace std;

//Constants
const int max_grid_size = 32;
const int target = 16;
const int max_templates = 20;
const int feature_size = target * target; //256

//To crop the resulting image to the symbol. The bounding box of the symbol.
struct BoundingBox{
    int minRow, maxRow, minCol, maxCol;
};


//Noise features. The features that are used to classify the symbol. The symmetry, density, and center of mass.
struct StructuralFeatures{
    double symmetryHorizontal;
    double symmetryVertical;
    double density;
    double centerMassRow;
    double centerMassCol;

};

//Bound truth. The 'perfect' version of each symbol.
struct Template{
    char label[32];
    double features[feature_size];
    StructuralFeatures structFeatures;
};

//Classification Result. The output of the classifier. The label and the bounding box.
struct ClassifyResult{
    char label[32];
    double confidence;
    char runnerUpLabel[32];
    double runnerUpConfidence;
    bool rejected;

};



class PixelClassifier{

private:
    //Template storage, up to 20 symbols. Each symbol has a label and a 16x16 grid of pixels.
    Template templates[max_templates];
    int numTemplates;

    //Internal helper functions
    //Crop to bounding box, returns four edges of the symbol
    BoundingBox cropToBoundingBox(int grid[][max_grid_size], int rows, int cols);

    //Resize to target size, sets a 16x16 grid of pixels (flattened to a 256-length array) representing the symbol.
    void resizeToTargetSize(int grid[][max_grid_size], BoundingBox box, double outputFeatures[feature_size]);

    //Structural Featueres
    StructuralFeatures extractStructuralFeatures(double features[feature_size]);

    //Symmetry, density, and center of mass calculations
    double cosineSimilarity(double features1[feature_size], double features2[feature_size]);
    double distance(double features1[feature_size], double features2[feature_size]);

    //Confidence calculation based on similarity scores, distance, and structural features. Returns a confidence score between 0 and 1.
    double calculateConfidence(double cosineSim, double distance, StructuralFeatures& input, StructuralFeatures& templateFeatures);

public:
    //Initialize template count to 0.
    PixelClassifier();
    
    //Register known symbols.
    void addTemplate(const char* label, int grid[][max_grid_size], int rows, int cols);

    //Actually classify an input grid. Returns the label, confidence, and runner-up information.
    ClassifyResult classify(int grid[][max_grid_size], int rows, int cols);

    void printGrid(int grid[][max_grid_size], int rows, int cols);
    void printResult(const ClassifyResult& result);
    int getTemplateCount();

};

#endif
