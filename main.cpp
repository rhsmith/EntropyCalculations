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
int main (int argc, char** argv)
{
    cout << "Hello World!" << endl;
    return 0;
}
