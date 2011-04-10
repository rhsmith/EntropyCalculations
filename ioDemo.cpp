#include <iostream>
#include <map>
#include <string>

#include "Hdf4.h"

using namespace std;

int main(int argc, char **argv)
{
  // Open the file for reading
  Hdf4 io;
  io.open(string("z_mhd_0000000.hdf"), IO::READ);

  // Allocate & read a variable.  You can do this in two ways:
  // 1.  Use A++:
  //floatArray bx;
  //io.readVariable(string("bx_"), bx);
  // 2.  Use C++:
  int dims[3] = {bx.getLength(0), bx.getLength(1), bx.getLength(2)};
  float *by = new float[dims[0]*dims[1]*dims[2]];

  int inRank;
  int *inDims;
  io.readVariable(string("by_"), by, inRank, inDims);
  
  // If you want to get fancy, you can read attributes like so:
  map<string, double> inMetaDoubles;
  map<string, float> inMetaFloats;
  map<string, int> inMetaInts;
  map<string, string> inMetaStrings;
  io.readMetaData(inMetaDoubles, inMetaFloats, inMetaInts, inMetaStrings);
  
  // now you can access the attributes like so:
  cout << "Modified Julian Date is: " << inMetaFloats["mjd"] << endl;
  
  // Don't forget to close a file when you're done with it!
  io.close();

  return 0;
}

