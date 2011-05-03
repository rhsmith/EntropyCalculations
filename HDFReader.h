/* 
 * File:   HDFReader.h
 * Author: ryan_h_smith
 *
 * Created on May 3, 2011, 4:04 PM
 */

#ifndef HDFREADER_H
#define	HDFREADER_H
#include<iostream>
#include<string>
#include <vector>
#include "Hdf4.h"

class HDFReader{
public:
    HDFReader(std::string filename, int dim1, int dim2, int dim3);
    std::vector<float*> readPositionData();
    std::vector<float*> readBData();
    std::vector<float*> readVData();
    std::vector<float> readPData();
    void setHDFFile(std::string filename, int dim1, int dim2, int dim3);
    void setHDFFile(std::string filename);
    virtual ~HDFReader();
private:
    void closeFile();
    bool openFile();

    Hdf4 io;
    std::string filename;
    bool hdfFileOpen;
    int dims[3];
    int inRank;
    int *inDims;
    float scale;


};

#endif	/* HDFREADER_H */

