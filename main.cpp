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
    int Nx = 81;
    int Ny = 61;
    const double PI = M_PI;
    const double dl = .3;
    TNT::Array2D<double> volumetotal(Nx,Ny);
    TNT::Array2D<double> pressuretotal(Nx,Ny);
    TNT::Array2D<double> entrop(Nx,Ny);
    TNT::Array2D<double> X(Nx,Ny);
    TNT::Array2D<double> Y(Nx,Ny);
    vector<double> xpos,ypos,zpos,bx,by,bz,pressure;
    string hdfFileName;
    //string baseoutpath = "/thayerfs/research/lfm/rsmith/data/entropy/";
    string baseoutpath = "testData/testOutput/";

    //Make sure input file was specified
    if(argc < 2)
    {
        cerr << "Error: input file not specified" << endl;
        return 1;
    }

    //Read in position data from text file
    readPositionData("testData/positions.txt",xpos,ypos,zpos);


    //Create regular cartesian interpolation grid which we will use to trace field lines
    for(int i=0; i < Nx; i++)
    {
        for(int j=0; j < Ny; j++)
        {
            X[i][j] = -.25*j-5;
            Y[i][j] = .25*i-10;
        }
    }


    //Begining of Main Loop over all files specified in input file
    char* filelistlocation = argv[1];
    ifstream filelist;
    filelist.open(filelistlocation, ios::in);
    int filenum = 0;
    string filenumstr;
    stringstream ss;
    while(getline(filelist,hdfFileName))
    {
        //Read data from specified file in the list and convert filenum to a string value
        readHDFData(hdfFileName, pressure, bx, by, bz);
        cout << "Read in data from " << hdfFileName << endl;
        ss << filenum;
        filenumstr = ss.str();


        //Now loop over each point in the grid, trace the fieldline that passes through that grid point
        //and compute the entropy. Since each grid point is independent of each other grid point, run
        //this is parallel
        #pragma omp parallel for schedule(dynamic) collapse(2) num_threads(16)
        for (int j = 0; j < Nx; j++)
        {
            for (int k = 0; k < Ny; k++)
            {
                //Print current location of the program
                cout << hdfFileName << ":  " << j << ", " << k << endl;

                //Variable Declarations for use in main loop
                double xstart = X[j][k];
                double ystart = Y[j][k];
                double zstart = 0.1;
                double xtrack[1000];
                double ytrack[1000];
                double ztrack[1000];
                double xtrack2[1000];
                double ytrack2[1000];
                double ztrack2[1000];

                xtrack[0] = xstart;
                ytrack[0] = ystart;
                ztrack[0] = zstart;
                xtrack2[0] = xstart;
                ytrack2[0] = ystart;
                ztrack2[0] = zstart;

                for (int ii = 1; ii < 1000; ii++)
                {
                    xtrack[ii] = 0;
                    ytrack[ii] = 0;
                    ztrack[ii] = 0;
                    xtrack2[ii] = 0;
                    ytrack2[ii] = 0;
                    ztrack2[ii] = 0;
                }


                double bztest = LFM3DInterp(xpos, ypos, zpos, bz, xstart, ystart, zstart);

                if (bztest > 0)
                {
                    double volume = 0;
                    double press = LFM3DInterp(xpos, ypos, zpos, pressure, xstart, ystart, zstart);
                    double bxtrack, bytrack, bztrack, btrack;
                    double radi;
                    for (int i = 0; i < 1000; i++)
                    {
                        bxtrack = LFM3DInterp(xpos, ypos, zpos, bx, xtrack[i], ytrack[i], ztrack[i]);
                        bytrack = LFM3DInterp(xpos, ypos, zpos, by, xtrack[i], ytrack[i], ztrack[i]);
                        bztrack = LFM3DInterp(xpos, ypos, zpos, bz, xtrack[i], ytrack[i], ztrack[i]);
                        btrack = sqrt(pow(bxtrack, 2) + pow(bytrack, 2) + pow(bztrack, 2));

                        xtrack[i + 1] = bxtrack / btrack * dl + xtrack[i];
                        ytrack[i + 1] = bytrack / btrack * dl + ytrack[i];
                        ztrack[i + 1] = bztrack / btrack * dl + ztrack[i];

                        volume = dl / btrack + volume;

                        radi = sqrt(pow(xtrack[i + 1], 2) + pow(ytrack[i + 1], 2) + pow(ztrack[i + 1], 2));

                        if (radi < 3.5) break;

                        if (radi > 2 * sqrt(pow(xstart, 2) + pow(ystart, 2) + pow(zstart, 2)) || sqrt(pow(ytrack[i], 2)) > 45)
                        {
                            volume = 0;
                            break;
                        }

                    }

                    for (int i = 0; i < 1000; i++)
                    {
                        bxtrack = LFM3DInterp(xpos, ypos, zpos, bx, xtrack2[i], ytrack2[i], ztrack2[i]);
                        bytrack = LFM3DInterp(xpos, ypos, zpos, by, xtrack2[i], ytrack2[i], ztrack2[i]);
                        bztrack = LFM3DInterp(xpos, ypos, zpos, bz, xtrack2[i], ytrack2[i], ztrack2[i]);

                        btrack = sqrt(pow(bxtrack, 2) + pow(bytrack, 2) + pow(bztrack, 2));

                        xtrack2[i + 1] = bxtrack / btrack * (-dl) + xtrack2[i];
                        ytrack2[i + 1] = bytrack / btrack * (-dl) + ytrack2[i];
                        ztrack2[i + 1] = bztrack / btrack * (-dl) + ztrack2[i];

                        volume = dl / btrack + volume;

                        radi = sqrt(pow(xtrack2[i + 1], 2) + pow(ytrack2[i + 1], 2) + pow(ztrack2[i + 1], 2));

                        if (radi < 3.5) break;

                        if (radi > 2 * sqrt(pow(xstart, 2) + pow(ystart, 2) + pow(zstart, 2)) || sqrt(pow(ytrack2[i], 2)) > 45)
                        {
                            volume = 0;
                            break;
                        }

                    }

                    volumetotal[j][k] = volume;
                    pressuretotal[j][k] = press;
                    entrop[j][k] = pow(volume, 1.6666) * press;
                }
                else
                {
                    volumetotal[j][k] = 0;
                    pressuretotal[j][k] = 0;
                    entrop[j][k] = 0;
                }
            }
        }//end omp parallel loop

        //Now write out results to a txt file and increment the output filenumber
        string outputpath = baseoutpath + filenumstr + ".txt";
        writeDataOut(outputpath, entrop);
        filenum++;
    }//End main loop over all files

    return 0;
}
