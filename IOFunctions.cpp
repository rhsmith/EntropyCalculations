#include "IOFunctions.h"
using namespace std;

/*
 * Outline:
 *  - Determine length/size of supplied data given in filename
 *  - Resize vectors to proper length
 *  - read data into vectors
 */
bool readHDFData(string filename,vector<double> &P, vector<double> &Bx, vector<double> &By, vector<double> &Bz)
{
    cout << "reading in data from " << filename << endl;

    int numlines = lineCount(filename);
    P.resize(numlines);
    Bx.resize(numlines);
    By.resize(numlines);
    Bz.resize(numlines);

    ifstream hdffile;
    hdffile.open(filename.c_str(),ios::in);
    for(int i=0; i < numlines; i++)
    {
        hdffile >> P[i];
        hdffile >> Bx[i];
        hdffile >> By[i];
        hdffile >> Bz[i];
    }
    return true;
}

bool readPositionData(string filename, vector<double> &x, vector<double> &y, vector<double> &z)
{
    int numlines = lineCount(filename); //little wasteful since file is opened and closed twice... but can fix that later
    x.resize(numlines);
    y.resize(numlines);
    z.resize(numlines);
    ifstream positionsFile;
    positionsFile.open(filename.c_str(),ios::in);
    //Don't need to error check because lineCount already does this
    for(int i = 0; i < numlines; i++)
    {
        positionsFile >> x[i];
        positionsFile >> y[i];
        positionsFile >> z[i];
    }
    return true;
}

int lineCount(string filename)
{
    int numlines = 0;
    char c;
    ifstream file;
    file.open(filename.c_str(),ios::in);
    if(!file.is_open())
    {
        //Error handle for bad filename
        cerr << "Error, cannot open file. Filename = " << filename;
    }
    else
    {
        while(file.good())
        {
            c = file.get();
            if(c == '\n')
            {
                numlines++;
            }
        }
    }
    file.close();
    return numlines;

}


bool writeDataOut(string filename, TNT::Array2D<double> data)
{
    cout << "writing data out to " << filename << endl;
    ofstream outputfile;
    outputfile.open(filename.c_str(),ios::out);
    for(int i= 0; i < data.dim1(); i++)
    {
        for(int j = 0; j < data.dim2(); j++)
        {
            outputfile << data[i][j] << " ";
        }
        outputfile << endl;
    }
    
    return false;
}