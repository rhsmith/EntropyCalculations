/* 
 * File:   InterpretationFunctions.h
 * Author: ryan_h_smith
 *
 * Created on April 5, 2011, 2:28 PM
 */

#ifndef INTERPRETATIONFUNCTIONS_H
#define	INTERPRETATIONFUNCTIONS_H
#include <vector>
#include "math.h"
#include "tnt/tnt.h"
#include "tnt/jama_lu.h"

double LFM3DInterp(std::vector<double> &x, std::vector<double> &y, std::vector<double> &z, std::vector<double> &bz, double x0, double y0, double z0);
double kshell_tri_interp(std::vector<double> &x, std::vector<double> &y, std::vector<double> &z, std::vector<double> &data, double x0, double y1, double z1, int KK);
int shellIndex(int i, int j, int k);
int kindex(int linearIndex);
int jindex(int linearIndex);
int iindex(int linearIndex);
double sign(double x);
double dist(double x, double y, double z);




#endif	/* INTERPRETATIONFUNCTIONS_H */

