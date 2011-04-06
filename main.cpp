/*
 * File:   main.cpp
 * Author: Ryan Hobson Smith
 *
 * This is the main file which when executed will read in a batch of specified .hdf files, extract pressure
 * and magnetic field variables, then trace out the field lines and compute the entropy, utilizing parallel
 * processing where useful.
 *
 * Created on April 4, 2011, 7:44 PM
 */

#include <iostream>
#include <vector>
#include "InterpretationFunctions.h"
#include "IOFunctions.h"

using namespace std;

/*
 * Main program entry point
 */
int main (int argc, char* argv[])
{
    vector<double> xpos,ypos,zpos,bx,by,bz,p;
    string hdfFileName;

    readPositionData("testData/positions.txt",xpos,ypos,zpos);

    if(argc < 2)
    {
        cerr << "Error: input file not specified" << endl;
        return 1;
    }

    char* filelistlocation = argv[1];

    ifstream filelist;
    filelist.open(filelistlocation, ios::in);
    //Main Loop over all files specified in input file
    while(getline(filelist,hdfFileName))
    {
        cout << hdfFileName << endl;
        readHDFData(hdfFileName, p, bx, by, bz);
        cout << "Read in data from " << hdfFileName << endl;
        
    }



    return 0;
}
