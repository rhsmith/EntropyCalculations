

#include "InterpretationFunctions.h"



const double PI = M_PI;

double LFM3DInterp(vector<double> &x, vector<double> &y, vector<double> &z, vector<double> &bz, double x0, double y0, double z0)
{
  //cout << "Entering LFM3DInterp" << endl;
  int NI = 53;
  int NJ = 60;
  int NK = 65;
  double theta, theta1, theta2;
  int KK;

  for(int k=0; k<NK-1; k++)
  {
    theta1 = PI - sign(z[shellIndex(4,4,k)])*acos(-y[shellIndex(4,4,k)]/sqrt(pow(y[shellIndex(4,4,k)],2.) + pow(z[shellIndex(4,4,k)],2.)));
    theta2 = PI - sign(z[shellIndex(4,4,k+1)])*acos(-y[shellIndex(4,4,k+1)]/sqrt(pow(y[shellIndex(4,4,k+1)],2.) + pow(z[shellIndex(4,4,k+1)],2.)));

    if (z0==0){
      theta = PI/2 - PI/2*sign(y0);
    }else{
      theta = PI - sign(z0)*acos(-y0/sqrt(pow(y0,2)+pow(z0,2)));
    }

    KK=63;
    if((theta-theta1)*(theta-theta2)<0){
      KK = k;
      break;
    }
  }

  double rho0=sqrt(pow(y0,2)+pow(z0,2));
  double y1=rho0*cos(theta1);
  double z1=rho0*sin(theta1);
  double y2=rho0*cos(theta2);
  double z2=rho0*sin(theta2);
  double d1,d2,d;

  if(KK==64){
    d1=kshell_tri_interp(x,y,z,bz,x0,y1,z1,KK);
    d2=kshell_tri_interp(x,y,z,bz,x0,y2,z2,0);
  } else {
    d1=kshell_tri_interp(x,y,z,bz,x0,y1,z1,KK);
    d2=kshell_tri_interp(x,y,z,bz,x0,y2,z2,KK);
  }

  if (KK<63){
    d=(d2-d1)*(theta-theta1)/(theta2-theta1)+d1;
  }else{
    theta1=theta1-2*PI;
    d=(d2-d1)*(theta-theta1)/(theta2-theta1)+d1;
  }

  return d;

}

double kshell_tri_interp(vector<double> &x, vector<double> &y, vector<double> &z, vector<double> &data, double x0, double y1, double z1, int KK)
{
  //cout << "Entering kshel" << endl;
  int NI = 53;
  int NJ = 60;
  int NK = 65;
  double p[NI][NJ];
  double q[NI][NJ];
  double data1[NI][NJ];
  double d = 0.0;
  double p1,q1;

  for(int i = 0; i < NI; i++)
  {
    for(int j = 0; j < NJ; j++)
    {
      p[i][j] = x[shellIndex(i,j,KK)];
      q[i][j] = sqrt(pow(y[shellIndex(i,j,KK)],2.0)+pow(z[shellIndex(i,j,KK)],2.0));
      p1=x0;
      q1=sqrt(pow(y1,2.0)+pow(z1,2.0));
      data1[i][j] = data[shellIndex(i,j,KK)];
    }
  }

  //Find out which triangle is (p1.q1) in
  //search through (i,j) pairs, each cell is divided into two triangles
  // 1 (i,j) (i+1,j),(i,j+1)
  // 2 (i+1,j+1) (i+1,j), (i,j+1)

  double s1[2];
  double s2[2];
  double s3[2];
  double s4[2];
  double xx1,yy1,ff1,xx2,yy2,ff2,xx3,yy3,ff3;

  for(int i = 0; i < NI-1; i++)
  {
    for(int j = 0; j < NJ-1; j++)
    {
      s1[0] = p[i][j]-p1;
      s1[1] = q[i][j]-q1;
      s2[0] = p[i+1][j]-p1;
      s2[1] = q[i+1][j]-q1;
      s3[0] = p[i+1][j+1]-p1;
      s3[1] = q[i+1][j+1]-q1;
      s4[0] = p[i][j+1]-p1;
      s4[1] = q[i][j+1]-q1;

      //Triangle 1, ANG(12)+ANG(24)+ANG(41)=2*pi
      double theta12, theta24, theta41;
      theta12=acos((s1[0]*s2[0]+s1[1]*s2[1])/sqrt((pow(s1[0],2)+pow(s1[1],2))*(pow(s2[0],2)+pow(s2[1],2))));
      theta24=acos((s2[0]*s4[0]+s2[1]*s4[1])/sqrt((pow(s2[0],2)+pow(s2[1],2))*(pow(s4[0],2)+pow(s4[1],2))));
      theta41=acos((s4[0]*s1[0]+s4[1]*s1[1])/sqrt((pow(s4[0],2)+pow(s4[1],2))*(pow(s1[0],2)+pow(s1[1],2))));

      if(abs(theta12+theta24+theta41-2.0*PI) < 0.001)
      {
        xx1=p[i][j];
        yy1=q[i][j];
        ff1=data1[i][j];
        xx2=p[i+1][j];
        yy2=q[i+1][j];
        ff2=data1[i+1][j];
        xx3=p[i][j+1];
        yy3=q[i][j+1];
        ff3=data1[i][j+1];
        break;
      }

      //Triangle 2, ANG(23)+ANG(34)+ANG(42)=2*pi
      double theta23, theta34, theta42;
      theta23=acos((s2[0]*s3[0]+s2[1]*s3[1])/sqrt((pow(s2[0],2)+pow(s2[1],2))*(pow(s3[0],2)+pow(s3[1],2))));
      theta34=acos((s3[0]*s4[0]+s3[1]*s4[1])/sqrt((pow(s3[0],2)+pow(s3[1],2))*(pow(s4[0],2)+pow(s4[1],2))));
      theta42=acos((s4[0]*s2[0]+s4[1]*s2[1])/sqrt((pow(s4[0],2)+pow(s4[1],2))*(pow(s2[0],2)+pow(s2[1],2))));


      if(abs(theta23+theta34+theta42-2.0*PI) < 0.001)
      {
        xx1=p[i+1][j+1];
        yy1=q[i+1][j+1];
        ff1=data1[i+1][j+1];
        xx2=p[i+1][j];
        yy2=q[i+1][j];
        ff2=data1[i+1][j];
        xx3=p[i][j+1];
        yy3=q[i][j+1];
        ff3=data1[i][j+1];
        break;
      }

    }
  }

  Array2D< double > arr1(3,3);
  Array2D< double > arr2(3,3);
  Array2D< double > arr3(3,3);
  Array2D< double > arr(3,3);

  double temp[3][3] = { {xx1, yy1, 1},
                       {xx2, yy2, 1},
                       {xx3, yy3, 1}};
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      arr[i][j] = temp[i][j];
    }
  }

  double temp1[3][3] = { {p1, q1, 1},
                        {xx2, yy2, 1},
                        {xx3, yy3, 1}};
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      arr1[i][j] = temp1[i][j];
    }
  }

  double temp2[3][3] = { {p1, q1, 1},
                        {xx1, yy1, 1},
                        {xx3, yy3, 1}};
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      arr2[i][j] = temp2[i][j];
    }
  }

  double temp3[3][3] = { {p1, q1, 1},
                        {xx1, yy1, 1},
                        {xx2, yy2, 1}};
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      arr3[i][j] = temp3[i][j];
    }
  }

  JAMA::LU< double > compute(arr);
  JAMA::LU< double > compute1(arr1);
  JAMA::LU< double > compute2(arr2);
  JAMA::LU< double > compute3(arr3);

  d = (ff1*compute1.det() - ff2*compute2.det() + ff3*compute3.det())/compute.det();


  return d;


}



double dist(double x, double y, double z)
{
  return sqrt(pow(x,2)+pow(y,2)+pow(z,2));
}


//// FUNCTIONS FOR SIMULATING RESHAPING OF DATA ///////////////////////////////////////////////////////////////////

int shellIndex(int i, int j, int k)
{
  //gets single index value for the point specified by i,j,k shell numbering
  //use this instead of "reshaping" the array for effeciency.
  int index = k*50*53+j*53+i;
  return index;
}

int kindex(int linearIndex)
{
  //gets the k shell index given an index in the linear array
  return (linearIndex/(50*53));
}


int jindex(int linearIndex)
{
  //gets the j shell index given an index in the linear array
  return (linearIndex%(50*53))/50;
}

int iindex(int linearIndex)
{
  //gets the i shell index given an index in the linear array
  return ((linearIndex%(50*53))%53);
}

double sign(double x)
{
  int sign = signbit(x);
  if(sign) return -1.0;
  else return 1.0;
}