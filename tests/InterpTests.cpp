/* 
 * File:   InterpTests.cpp
 * Author: ryan_h_smith
 *
 * Created on Apr 11, 2011, 1:01:04 PM
 */

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include "IOFunctions.h"
#include "InterpretationFunctions.h"

/*
 * Simple C++ Test Suite
 */
using namespace std;
void kshell_Test() {
    //NOTE!!!! DO NOT CHANGE THESE VALUES ELSE TEST DATA FILE WILL NOT WORK OR WILL
    //HAVE TO BE REGENERATED!!
    const int N = 20; //DO NOT CHANGE UNLESS TEST FILE REBUILT
    const int step = 15; //DO NOT CHANGE UNLESS TEST FILE REBUILT
    const double tol = 1E-15;
    std::cout << "kshell test" << std::endl;
    vector<double> x,y,z,p,bx,by,bz,results, correctData;
    readPositionData("testData/positions.txt",x,y,z);
    readHDFData("testData/testmhddata.txt",p,bx,by,bz);

    const int M = 1728; //calculated based on known test file size
    ifstream correctDataFile;
    correctDataFile.open("testData/kshelltestdata.txt", ios::in);
    correctData.resize(M);
    for(int i = 0; i < M; i++){
        correctDataFile >> correctData[i];
    }

    //NOTE, not sure that this properly tests KK == 64 shell...
    cout.precision(numeric_limits<double>::digits10 + 2);
    for(double x0=-N; x0<=N; x0 = x0+step){
        for(double y1=-N; y1<=N; y1  = y1+step){
            for(double z1=-N; z1<=N; z1= z1+step){
                for(int KK=0; KK<64; KK++){
                    results.push_back(kshell_tri_interp(x,y,z,p,x0,y1,z1,KK));
                    cout << x0 << " " << y1 << " " << z1 << " " << KK << " " << endl;
                }
            }
        }
    }

    for(int i = 0; i < M; i++){
        double diff = abs(results[i] - correctData[i]);
        if(diff > tol)
        {
            cout << "%TEST_FAILED% time=0 testname=kshell_Test (InterpTests) message=Test Value: "
                 << results[i] << " != " << correctData[i]
                 << ". Difference = " << diff << " Tol = " << tol << std::endl;
            break;
        }
    }

    //GENERATION CODE FOR ORIGINAL TEST DATA
//    for(double x0=-N; x0<=N; x0 = x0+step){
//        for(double y1=-N; y1<=N; y1  = y1+step){
//            for(double z1=-N; z1<=N; z1= z1+step){
//                for(int KK=0; KK<64; KK++){
//                    results.push_back(kshell_tri_interp(x,y,z,p,x0,y1,z1,KK));
//                    cout << x0 << " " << y1 << " " << z1 << " " << KK << " " << endl;
//                }
//            }
//        }
//    }
//
//    ofstream output;
//    output.open("testData/kshelltestdata.txt");
//    output.precision(numeric_limits<double>::digits10 + 1);
//    for(int i=0; i < results.size(); i++)
//    {
//        output << results[i] << endl;
//    }
//    output.close();
    

}

void LFM3D_Test() {
    std::cout << "LFM3D test " << std::endl;
    
    std::cout << "%TEST_FAILED% time=0 testname=LFM3D_Test (InterpTests) message=error message sample" << std::endl;
}

void EntropyCalc_Test() {
    std::cout << "EntropyCalc test " << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=EntropyCalc_Test (InterpTests) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% InterpTests" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% kshell_test (InterpTests)\n" << std::endl;
    kshell_Test();
    std::cout << "%TEST_FINISHED% time=0 kshell_Test (InterpTests)" << std::endl;

    std::cout << "%TEST_STARTED% LFM3D_Test (InterpTests)" << std::endl;
    LFM3D_Test();
    std::cout << "%TEST_FINISHED% time=0 LFM3D_Test (InterpTests)" << std::endl;

    std::cout << "%TEST_STARTED% EntropyCalc_Test (InterpTests)\n" << std::endl;
    EntropyCalc_Test();
    std::cout << "%TEST_FINISHED% time=0 EntropyCalc_Test (InterpTests)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

