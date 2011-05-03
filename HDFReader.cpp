/* 
 * File:   HDFReader.cpp
 * Author: ryan_h_smith
 *
 * Wrapper class for Io/Hdf4 to access MHD HDF files in an easy way and return
 * the data types that I desire.
 * 
 * Created on May 3, 2011, 4:04 PM
 */

#include "HDFReader.h"
using namespace std;

HDFReader::HDFReader(string filename, int dim1, int dim2, int dim3)
{
    this->filename = filename;
    dims[0] = dim3;
    dims[1] = dim2;
    dims[2] = dim1;
    this->scale = 6.38e8;
    this->hdfFileOpen = false;
}

HDFReader::~HDFReader()
{
    if(hdfFileOpen)
        closeFile();
}

void HDFReader::setHDFFile(string filename, int dim1, int dim2, int dim3)
{
    if(hdfFileOpen)
    {
        closeFile();
    }
    this->filename = filename;
    hdfFileOpen = openFile();
    dims[0] = dim3;
    dims[1] = dim2;
    dims[2] = dim1;

}

void HDFReader::setHDFFile(string filename)
{
    if(hdfFileOpen)
    {
        closeFile();
    }
    this->filename = filename;
    hdfFileOpen = openFile();

}

bool HDFReader::openFile()
{
    io.open(string("filename"), IO::READ);
    //TODO add check to make sure file really opens!
    return true;
}

void HDFReader::closeFile()
{
    io.close();
}

vector< float* > HDFReader::readPositionData()
{
    float* xposData = new float[dims[0]*dims[1]*dims[2]];
    float* yposData = new float[dims[0]*dims[1]*dims[2]];
    float* zposData = new float[dims[0]*dims[1]*dims[2]];

    io.readVariable(string("X_grid"), xposData, inRank, inDims);
    io.readVariable(string("Y_grid"), yposData, inRank, inDims);
    io.readVariable(string("Z_grid"), zposData, inRank, inDims);

    vector< float*> data;
    data.resize(3);
    data[0] = xposData;
    data[1] = yposData;
    data[2] = zposData;

    return data;

}

vector<float * > HDFReader::readBData()
{

    float* bxData = new float[dims[0]*dims[1]*dims[2]];
    float* byData = new float[dims[0]*dims[1]*dims[2]];
    float* bzData = new float[dims[0]*dims[1]*dims[2]];

    io.readVariable(string("bx_"), bxData, inRank, inDims);
    io.readVariable(string("by_"), byData, inRank, inDims);
    io.readVariable(string("bz_"), bzData, inRank, inDims);

    vector< float*> data;
    
    return data;
}

vector<float * > HDFReader::readVData()
{

    vector< float*> data;

    return data;
}

vector<float> HDFReader::readPData()
{

    vector< float> data;

    return data;
}

