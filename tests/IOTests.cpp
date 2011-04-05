/*
 * File:   IOTests.cpp
 * Author: Ryan Hobson Smith
 *
 * Created on Apr 4, 2011, 9:03:55 PM
 */

#include <stdlib.h>
#include <iostream>
#include "IOFunctions.h"

void LineCountTest() {
    std::cout << "Testing Line Count Function" << std::endl;
    int numlines = lineCount("testData/linecounttestfile.txt");
    if(numlines != 15)
    {
        std::cout << "%TEST_FAILED% time=0 testname=LineCountTest (IOTests) message=Line Count Function failed, returned " << numlines << std::endl;
    }
}

void ReadHDFTest(){
    std::cout << "Testing HDF Reading Function" << std::endl;

    double P1,Bx1,By1,Bz1;
    std::vector<double> P,Bx,By,Bz;
    std::string datFile = "testData/0";

    if (!(readHDFData(datFile,P,Bx,By,Bz)))
    {
        std::cout << "%TEST_FAILED% time=0 testname=ReadHDFTest (IOTests) message=readHDFData returned false " << std::endl;
    }

    int numlines = lineCount(datFile.c_str());
    std::ifstream testfile;
    testfile.open(datFile.c_str(),std::ios::in);
    for(int i=0; i < numlines; i++)
    {
        testfile >> P1;
        testfile >> Bx1;
        testfile >> By1;
        testfile >> Bz1;

        if(i < 10)
        {
            std::cout << P[i] << " " << Bx[i] << " " << By[i] << " " << Bz[i] << " " << std::endl;
        }

        if(!(P1 == P[i] && Bx1 == Bx[i] && By1 == By[i] && Bz1 == Bz[i]))
        {
           std::cout << "%TEST_FAILED% time=0 testname=ReadHDFTest (IOTests) message=Read in Values did not match on line " << i << std::endl;
        }
    }

}

void WriteHDFTest(){
    std::cout << "Testing HDF Write Function" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=WriteHDFTest (IOTests) message=Write Function failed (not implemented)" << std::endl;
}


int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% IOTests" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% LineCountTest (IOTests)" << std::endl;
    LineCountTest();
    std::cout << "%TEST_FINISHED% time=0 LineCountTest (IOTests)" << std::endl;

    std::cout << "%TEST_STARTED% ReadHDFTest (IOTests)" << std::endl;
    ReadHDFTest();
    std::cout << "%TEST_FINISHED% time=0 ReadHDFTest (IOTests)" << std::endl;

    std::cout << "%TEST_STARTED% WriteHDFTest (IOTests)" << std::endl;
    WriteHDFTest();
    std::cout << "%TEST_FINISHED% time=0 WriteHDFTest (IOTests)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}
