/*
 * File:   IOFunctions.hpp
 * Author: Ryan Hobson Smith
 *
 * Created on April 4, 2011, 8:29 PM
 */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "tnt/tnt_array2d.h"

#ifndef IOFUNCTIONS_H
#define	IOFUNCTIONS_H

/*
 * This funciton takes filename as an input and then reads in data found in that file into the supplied vector arrays Bx, By, Bz, P
 */
bool readHDFData(std::string filename, std::vector<double> &P, std::vector<double> &Bx, std::vector<double> &By, std::vector<double> &Bz);

/*
 * This function reads in position data from specified filename (txt file representation only)
 */
bool readPositionData(std::string filename, std::vector<double> &x, std::vector<double> &y, std::vector<double> &z);

/*
 * This function simply reads the file and returns the number of lines (used to preallocate vectors). Unsure if this is the fastest method to allocate and read data
 * But it works for now
 */
int lineCount(std::string filename);

/*
 * This function writes out entropy data into the filename specified
 */
bool writeDataOut(std::string filename, TNT::Array2D<double> data);


#endif	/* IOFUNCTIONS_H */

