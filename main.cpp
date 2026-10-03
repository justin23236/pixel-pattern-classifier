
#include <iostream>
#include "PixelClassifier.h"

using namespace std;

int main() {
    
    //Test
    PixelClassifier pc;

    //X Pattern Grid
    int xGrid[max_grid_size][max_grid_size] = {
        {1, 0, 0, 0, 1},
        {0, 1, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 0, 1, 0},
        {1, 0, 0, 0, 1}
    };

    //O Pattern Grid
    int oGrid[max_grid_size][max_grid_size] = {
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 0}
    };

    //T Pattern Grid
    int tGrid[max_grid_size][max_grid_size] = {
        {1,1,1,1,1},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0}
    };

    int largeX[max_grid_size][max_grid_size] = {
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
};

    //Add templates
    pc.addTemplate("X", xGrid, 5, 5);
    pc.addTemplate("O", oGrid, 5, 5);
    pc.addTemplate("T", tGrid, 5, 5);   

    //Display template count
    cout << "Templates loaded: " << pc.getTemplateCount() << endl;

    //Noisy X Pattern Grid
    int xtestGrid_1[max_grid_size][max_grid_size] = {
        {1, 0, 0, 1, 1},
        {0, 1, 0, 1, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 0, 1, 0},
        {1, 0, 0, 0, 0}
    };

    int xtestGrid_2[max_grid_size][max_grid_size] = {
        {1, 0, 0, 0, 1},
        {0, 1, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 0, 1, 0},
        {1, 0, 0, 0, 1}
    };

    //Clean O Test
    int otestGrid[max_grid_size][max_grid_size] = {
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 0}
    };
    //Noisy O Test
    int otestGridnoisy[max_grid_size][max_grid_size] = {
        {0, 1, 0, 1, 0},
        {1, 1, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 0, 0, 1},
        {1, 1, 1, 1, 0}
    };

    //Clean T Grid
    int ttestGrid[max_grid_size][max_grid_size] = {
        {1,1,1,1,1},
         {0,0,1,0,0},
         {0,0,1,0,0},
         {0,0,1,0,0},
         {0,0,1,0,0}
    };

    //Noisy T Test
    int ttestGridnoisy[max_grid_size][max_grid_size]= {
        {1,0,1,1,1},
         {0,0,1,0,0},
         {0,0,1,0,1},
         {1,0,1,0,0},
         {0,0,1,0,0}
    };

    int heavyNoiseX[max_grid_size][max_grid_size] = {
        {1, 1, 1, 0, 1},
        {0, 1, 1, 1, 0},
        {0, 1, 1, 0, 0},
         {0, 0, 0, 1, 0},
         {1, 0, 1, 0, 0}
    };

    //Should be rejected
    int randomGrid[max_grid_size][max_grid_size] = {
        {1, 1, 0, 1, 0},
        {1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0},
         {1, 0, 1, 0, 1},
         {0, 1, 0, 1, 1}
    };

    //Test classification
    cout << "Testing X pattern with noise:" << endl;
    pc.printGrid(xtestGrid_1, 5, 5);
    cout << "\n";

    ClassifyResult result1 = pc.classify(xtestGrid_1, 5, 5);
    pc.printResult(result1);

    cout << "\nTesting clean X pattern:" << endl;
    pc.printGrid(xtestGrid_2, 5, 5);
    cout << "\n";
    ClassifyResult result2 = pc.classify(xtestGrid_2, 5, 5);
    pc.printResult(result2);

    cout << "\nTesting clean O pattern:" << endl;
    pc.printGrid(otestGrid, 5, 5);
    cout << "\n";
    ClassifyResult result3 = pc.classify(otestGrid, 5, 5);
    pc.printResult(result3);

    cout << "\nTesting noisy O pattern:" << endl;
    pc.printGrid(otestGridnoisy, 5, 5);
    cout << "\n";
    ClassifyResult result4 = pc.classify(otestGridnoisy, 5, 5);
    pc.printResult(result4);

    cout << "\nTesting clean T pattern:" << endl;
    pc.printGrid(ttestGrid, 5, 5);
    cout << "\n";
    ClassifyResult result5 = pc.classify(ttestGrid, 5, 5);
    pc.printResult(result5);

    cout << "\nTesting noisy T pattern:" << endl;
    pc.printGrid(ttestGridnoisy, 5, 5);
    cout << "\n";
    ClassifyResult result6 = pc.classify(ttestGridnoisy, 5, 5);
    pc.printResult(result6);

    //Result turns out to be T, not X. But only a 1.6% difference in score. 
    //The heavily noisy X pattern is very off from the template, but it still has a strong X signal. 
    cout << "\nTesting heavily noisy X pattern:" << endl;
    pc.printGrid(heavyNoiseX, 5, 5);
    cout << "\n";
    ClassifyResult result7 = pc.classify(heavyNoiseX, 5, 5);
    pc.printResult(result7);

    cout << "\nTesting random pattern:" << endl;
    pc.printGrid(randomGrid, 5, 5);
    cout << "\n";
    ClassifyResult result8 = pc.classify(randomGrid, 5, 5);
    pc.printResult(result8);

    cout << "\nTesting 20x20 X pattern: " <<endl;
    pc.printGrid(largeX, 20, 20);
    cout << "\n";
    ClassifyResult result9 = pc.classify(largeX, 20,20);
    pc.printResult(result9);

    return 0;

}