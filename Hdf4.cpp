#include "Hdf4.h"

using namespace std;

Hdf4::Hdf4()
{
  fileId = -999;
}


Hdf4::~Hdf4()
{
  close();
}


void Hdf4::open(const string &filename, const int &ioType)
{
  int32 accessMode;
  if (ioType == IO::READ){
    accessMode = DFACC_RDONLY;
  }
  else if (ioType == IO::WRITE){
    accessMode = DFACC_CREATE;
  }
  else{
    cerr << "ERROR:  " <<__FILE__ << "::" << __FUNCTION__ << "(" << __LINE__ << ") Did not understand ioType" << endl;
  }
  
  ERRORCHECK( fileId = SDstart(filename.c_str(), accessMode) );
}


void Hdf4::close(void)
{
  if (fileId >= 0){
    ERRORCHECK( SDend(fileId) );
    fileId = -999;
  }
}

void Hdf4::errorCheck(const int &status, const char *file, const int &line, const char *func, const char *command)
{
  if (status < 0) {
    cerr << "*** Error in " << file << "(L " << line << "): " << func << "(...)" << endl;
    cerr << "\"" << command << "\": "
	 << "HDF4 error code: " << status << endl;

#if defined(APLUSPLUS) || defined(PPLUSPLUS)
    Optimization_Manager::Exit_Virtual_Machine();
#endif

    exit(1); 
  }
}

void Hdf4::readMetaData(std::map<std::string, double> &metaDoubles,
			std::map<std::string, float> &metaFloats,
			std::map<std::string, int> &metaInts,
			std::map<std::string, std::string> &metaStrings)
{
  int32 nDatasets;
  int32 nAttributes;

  ERRORCHECK( SDfileinfo(fileId,&nDatasets ,&nAttributes) );

  char attrName[MAX_NC_NAME];
  int32 dataType;
  int32 nCharacters;
  for (int32 i=0; i < nAttributes; i++){
    // Get attribute name
    ERRORCHECK( SDattrinfo( fileId, i, attrName, &dataType, &nCharacters) );

      // Read the attribute.
    if (dataType == DFNT_FLOAT64){
      double attrValue;
      ERRORCHECK( SDreadattr( fileId, i, &attrValue) );
      metaDoubles.insert ( pair<string,double>( attrName, attrValue) );
    }
    else if (dataType == DFNT_FLOAT32){
      float attrValue;
      ERRORCHECK( SDreadattr( fileId, i, &attrValue) );
      metaFloats.insert ( pair<string,float>( attrName, attrValue) );
    }
    else if (dataType == DFNT_INT32){
      int attrValue;
      ERRORCHECK( SDreadattr( fileId, i, &attrValue) );
      metaInts.insert ( pair<string,int>( attrName, attrValue) );
    }
    else if (dataType == DFNT_CHAR8){
      char *attrValue = new char[nCharacters+1];
      // Silly trick to use Hdf4 error handling on bad allocations:
      ERRORCHECK( (attrValue != NULL) -1 );
      
      ERRORCHECK( SDreadattr( fileId, i, attrValue) );    
      attrValue[nCharacters] = '\0';
      
      metaStrings.insert( pair<string, string>( attrName, attrValue) );
      delete [] attrValue;
      attrValue = NULL;
    }
    else{
      cerr << "*** Error in " << __FILE__ << "(L " << __LINE__ << "): " << __FUNCTION__ << "(...)" << endl;
      cerr << "\tDid not understand HDF dataType \"" << dataType << "\"." << endl;
#if defined(APLUSPLUS) || defined(PPLUSPLUS)
      Optimization_Manager::Exit_Virtual_Machine();
#endif
      exit(1);
    }
  }
}

void Hdf4::readString(const std::string&name, std::string &data)
{
  int32 SDselectId;                    // HDF selection ID
  int32 varId;                         // HDF variable ID
  int32 dataType;                      // Internal HDF datatype of variable
  int32 nAttributes;                   // number of attributes for the data set
  int32 rank;                          // rank of the variable.
  int32 dimensions_i32[MAX_VAR_DIMS];  // used to compute indexEnd
  int32 indexStart[MAX_VAR_DIMS];      // [x,y,z] starting location from where data is read
  int32 indexEnd[MAX_VAR_DIMS];        // [x,y,z] number of values to be read along each dimension
  char dataName[MAX_NC_NAME];          // 
    
  int i;
    
  for(i=0;i<MAX_VAR_DIMS;i++) {
    dimensions_i32[i] = 0;
    indexStart[i] = 0;
    indexEnd[i] = 0;
  }
  /*Get the info about the dataset */
  ERRORCHECK( varId = SDnametoindex(fileId,name.c_str()) );

  SDselectId = SDselect(fileId,varId);
  ERRORCHECK( SDgetinfo(SDselectId,dataName,&rank,dimensions_i32,&dataType,&nAttributes) );
  
  /* Create reterival information arrays and allocate space */
  for(i=0;i<rank;i++) {
    indexStart[i]=0;
    indexEnd[i]=dimensions_i32[rank-i-1];
    indexEnd[i]=dimensions_i32[i];
  }
  char *inData = new char [indexEnd[0]+1];
  // Silly trick to use Hdf4 error handling on bad allocations:
  ERRORCHECK( (inData != NULL) -1 );
    
  /* Get dataset */
  ERRORCHECK( SDreaddata(SDselectId,indexStart,NULL,indexEnd,inData) );

  data.reserve(indexEnd[0]+1);
  data = string(inData);
  delete []inData;
      
  ERRORCHECK( SDendaccess(SDselectId) ); 
}


/*******************************************************************************
 * Read @variable into @data.
 *
 * Notes: 
 *   - Allocates data as 1-d array.
 *   - Allocates dims
 ******************************************************************************/
void Hdf4::readVariable(const std::string &variable, float *&data, int &rank, int *&dims)
{
  int32 SDselectId;                    // HDF selection ID
  int32 varId;                         // HDF variable ID
  int32 dataType;                      // Internal HDF datatype of variable
  int32 nAttributes;                   // number of attributes for the data set
  int32 rank_i32;                      // rank of the variable.
  int32 dimensions_i32[MAX_VAR_DIMS];  // used to compute indexEnd
  int32 indexStart[MAX_VAR_DIMS];      // [x,y,z] starting location from where data is read
  int32 indexEnd[MAX_VAR_DIMS];        // [x,y,z] number of values to be read along each dimension
  char dataName[MAX_NC_NAME];          // 
    
  int i;
    
  for(i=0;i<MAX_VAR_DIMS;i++) {
    dimensions_i32[i] = 0;
    indexStart[i] = 0;
    indexEnd[i] = 0;
  }
  /*Get the info about the dataset */
  ERRORCHECK( varId = SDnametoindex(fileId,variable.c_str()) );

  SDselectId = SDselect(fileId,varId);
  ERRORCHECK( SDgetinfo(SDselectId,dataName,&rank_i32,dimensions_i32,&dataType,&nAttributes) );
  
  /* Create reterival information arrays */
  int nPoints = 1;
  rank = rank_i32;
  dims = new int[rank];
  // Silly trick to use Hdf4 error handling on bad allocations:
  ERRORCHECK( (dims != NULL) -1 );
  for(i=0;i<rank;i++) {
    indexStart[i]=0;
    indexEnd[i]=dimensions_i32[rank-i-1];
    indexEnd[i]=dimensions_i32[i];
   
    nPoints *= dimensions_i32[i];
    dims[i] = dimensions_i32[i];
  }

  data = new float[nPoints];

  // Silly trick to use Hdf4 error handling on bad allocations:
  ERRORCHECK( (data != NULL) -1 );

  /* Get dataset */
  ERRORCHECK( SDreaddata(SDselectId,indexStart,NULL,indexEnd,data) );
      
  ERRORCHECK( SDendaccess(SDselectId) );
}


#if defined(APLUSPLUS) || defined(PPLUSPLUS)
/*******************************************************************************
 * Redimensions data & reads in data.  
 *
 * Note: This function only works for variables of up to 6 dimensions!
 * Search HDF4::readVariable(...) implementation for "data.redim(" for
 * more info.
 ******************************************************************************/
void Hdf4::readVariable(const string &variable, floatArray &data)
{
  // Read the data
  int inRank = 0;
  int *inDims = NULL;
  float *inData = NULL;
  readVariable(variable, inData, inRank, inDims);

  // Stored as A++/P++ floatArray
  /* Allocate space. Note that we transpose the dimensions here since
   *  HDF stores in column-ordered, but A++/P++ floatArray uses
   *  Fortran ordered.  See Hdf4::writeVaraible(...) comments for more info.
   *
   *  Also note: Trial & error shows that
   *       data.redim(dimensions_i32[0], 0)
   *  does not work for 1-d arrays. 
   * 
   * Finally, note this works only for dimensioned up to 6-d.
   */
  if (inRank == 1){
    data.redim(inDims[0]);
    for (int i = 0; i < inDims[0]; i++){
      data(i) = inData[i];
    }      
  }
  else if (inRank == 2){
    data.redim(inDims[1], inDims[0]);
    for (int i = 0; i < inDims[1]; i++){
      for (int j = 0; j < inDims[0]; j++){
	data(i,j) = inData[ i + j*inDims[1] ];
      }
    }
  }
  else if (inRank == 3){
    data.redim(inDims[2], inDims[1], inDims[0]);
    for (int i = 0; i < inDims[2]; i++){
      for (int j = 0; j < inDims[1]; j++){
	for (int k = 0; k < inDims[0]; k++){
	  data(i,j,k) = inData[ i + j*inDims[2] + k*inDims[2]*inDims[1] ];
	}
      }
    }
  }
  else if (inRank == 4){
    data.redim(inDims[3], inDims[2], inDims[1], inDims[0]);
    for (int i = 0; i < inDims[3]; i++){
      for (int j = 0; j < inDims[2]; j++){
	for (int k = 0; k < inDims[1]; k++){
	  for (int m = 0; m < inDims[0]; m++){
	    data(i,j,k,m) = inData[ i + j*inDims[3] + k*inDims[3]*inDims[2] + m*inDims[3]*inDims[2]*inDims[1] ];
	  }
	}
      }
    }
  }
  else if (inRank == 5){
    data.redim(inDims[4], inDims[3], inDims[2], inDims[1], inDims[0]);
    for (int i = 0; i < inDims[4]; i++){
      for (int j = 0; j < inDims[3]; j++){
	for (int k = 0; k < inDims[2]; k++){
	  for (int m = 0; m < inDims[1]; m++){
	    for (int n = 0; n < inDims[0]; n++){
	      data(i,j,k,m,n) = inData[ i + j*inDims[4] + k*inDims[4]*inDims[3] + 
					m*inDims[4]*inDims[3]*inDims[2] +
					n*inDims[4]*inDims[3]*inDims[2]*inDims[1] ];
	    }
	  }
	}
      }
    }
  }
  else if (inRank == 6){
    data.redim(inDims[5], inDims[4], inDims[3], inDims[2], inDims[1], inDims[0]);
    for (int i = 0; i < inDims[5]; i++){
      for (int j = 0; j < inDims[4]; j++){
	for (int k = 0; k < inDims[3]; k++){
	  for (int m = 0; m < inDims[2]; m++){
	    for (int n = 0; n < inDims[1]; n++){
	      for (int p = 0; p < inDims[0]; p++){	      
		data(i,j,k,m,n,p) = inData[ i + j*inDims[5] + k*inDims[5]*inDims[4] + 
					    m*inDims[5]*inDims[4]*inDims[3] +
					    n*inDims[5]*inDims[4]*inDims[3]*inDims[2] +
					    p*inDims[5]*inDims[4]*inDims[3]*inDims[2]*inDims[1] ];
	      }
	    }
	  }
	}
      }
    }
  }
  else {
    cerr << __FILE__ << " (L" << __LINE__ << "): " << __FUNCTION__ << "(...):" << endl
	 << "Implementation supports arrays up to 6-dimensions, but you requested " << inRank << endl
	 << "This is a silly hard-coded bug.  The fix is to add more cases to \"else if (inRank == n)\"." << endl;
    ERRORCHECK (-1);
  }
  
  delete []inDims;
  inDims = NULL;
  delete []inData;
  inData = NULL;  
}
#endif
// defined(APLUSPLUS) || defined(PPLUSPLUS)

void Hdf4::writeMetaData(const std::map<std::string, double> &metaDoubles,
			 const std::map<std::string, float> &metaFloats,
			 const std::map<std::string, int> &metaInts,
			 const std::map<std::string, std::string> &metaStrings)
{
  writeMetaData(metaDoubles);
  writeMetaData(metaFloats);
  writeMetaData(metaInts);
  writeMetaData(metaStrings);
}

void Hdf4::writeMetaData(const map<string, double> &metaDoubles)
{
  typedef map<string, double> mapType;
  for (mapType::const_iterator it = metaDoubles.begin(); it != metaDoubles.end(); ++it){
    string attributeName = it->first; //aka the "key"
    double attributeData = it->second; // aka the "value"
    
    ERRORCHECK( SDsetattr( fileId, attributeName.c_str(), DFNT_FLOAT64, 1, &attributeData) );
  }
}

void Hdf4::writeMetaData(const map<string, float> &metaFloats)
{
  typedef map<string, float> mapType;
  for (mapType::const_iterator it = metaFloats.begin(); it != metaFloats.end(); ++it){
    string attributeName = it->first; //aka the "key"
    float attributeData = it->second; // aka the "value"
    
    ERRORCHECK( SDsetattr( fileId, attributeName.c_str(), DFNT_FLOAT32, 1, &attributeData) );
  }
}

void Hdf4::writeMetaData(const map<string, int> &metaInts)
{
  typedef map<string, int> mapType;
  for (mapType::const_iterator it = metaInts.begin(); it != metaInts.end(); ++it){
    string attributeName = it->first; //aka the "key"
    int attributeData = it->second; // aka the "value"
    
    ERRORCHECK( SDsetattr( fileId, attributeName.c_str(), DFNT_INT32, 1, &attributeData) );
  }
}

void Hdf4::writeMetaData(const map<string, string> &metaStrings)
{
  typedef map<string, string> mapType;
  for (mapType::const_iterator it = metaStrings.begin(); it != metaStrings.end(); ++it){
    string attributeName = it->first; //aka the "key"
    string attributeData = it->second; // aka the "value"
    
    // Hdf4 is unpredictable when writing 0-length data!
    assert(attributeName.length() > 0);    
    if (attributeData.length() > 0){
      ERRORCHECK( SDsetattr( fileId, attributeName.c_str(), 
			     DFNT_CHAR8, attributeData.length(), attributeData.c_str() ) );
    }
    else{
      // Empty strings should be output as null character.
      static const char nullString = '\0';
      ERRORCHECK( SDsetattr( fileId, attributeName.c_str(), 
			     DFNT_CHAR8, 1, nullString ) );      
    }
  }   
}

void Hdf4::writeString(const string &name, const string &data)
{
  int32 varId;
  int32 rank1 = 1;
  int32 dimensions[MAX_VAR_DIMS];
  dimensions[0] = data.length()+1; //we will append a NULL ('\0') character
  ERRORCHECK( varId = SDcreate(fileId, name.c_str(), DFNT_CHAR8, rank1, dimensions) );
  
  int32 indexStart[MAX_VAR_DIMS];
  indexStart[0] = 0;
  int32 indexEnd[MAX_VAR_DIMS];
  indexEnd[0] = data.length()+1;

  // Hdf4 is unpredictable when writing 0-length data!  Append a  NULL '\0' character.

  // HDF4 SDwritedata doesn't know how to handle data.c_str() since
  // it returns a const pointer.  So we must do this funkieness with
  // C-strings.
  char *outputStr = new char[data.length()+1];
  // Silly trick to use Hdf4 error handling on bad allocations:
  ERRORCHECK( (outputStr != NULL) -1 );

  strncpy(outputStr, data.c_str(), data.length());
  outputStr[data.length()] = '\0';
  ERRORCHECK( SDwritedata(varId, indexStart, NULL, indexEnd, outputStr) );
  delete []outputStr;
}

/*******************************************************************************
 *
 * Note that changing the "data" type from float requires
 * changing the "dataType" variable from DFNT_FLOAT32.
 *
 * Note: "dims" array is transposed from what you would typically
 * expect.  See comments in writeVariable(...) implementation for
 * details.
 ******************************************************************************/
void Hdf4::writeVariable(const std::string &variable, const float *data, const int &rank, const int *dims, const string &units)
{
  int32 varId;                    // HDF variable ID
  int32 dataType = DFNT_FLOAT32;  // Internal HDF datatype of variable
  int32 dimensions[MAX_VAR_DIMS]; 
  int32 indexStart[MAX_VAR_DIMS]; // [i,j,k] starting location (typically 0 everywhere)
  int32 indexEnd[MAX_VAR_DIMS];   // [i,j,k] number of values to be read along each dimension

  for (int i=0; i < rank; i++){
    dimensions[i] = dims[i];
    indexStart[i] = 0;
    indexEnd[i] = dims[i];
  }

  // Now do the I/O on processor 0.
  ERRORCHECK( varId = SDcreate(fileId, variable.c_str(), dataType, rank, dimensions) );

  // Set attributes
  ERRORCHECK( SDsetattr( varId, "units", DFNT_CHAR8, units.length(), units.c_str() ) );
	
  /*
   * The original LFM HDF format includes dimensions as SD
   * attributes. This is should be unecessary, since the attribute
   * data should already be saved as attribute data via the
   * "dimensions" array.  However, we must duplicate the dimensions
   * here!
   * 
   * The "dimensions" array is transposed from the way the data
   * is actually stored.  That is:
   * 
   *     dimensions[0] = nk, 
   *     dimensions[1] = nj, 
   *     dimensions[2] = ni, 
   * 
   * Here's why: A++/P++ stores arrays in column-major ordering.  The
   * LFM-para I/O routines are written in Fortran and account for
   * this.  However, the HDF4 C interface reads/writes in row-major
   * ordering.  Old post-processing routines (such as CISM_DX) use the
   * C interface and query dimensions from "ni", "nj", "nk"
   * HDF4 SD attributes.
   */

  ERRORCHECK( SDsetattr( varId, "ni", DFNT_INT32, 1, &dims[2] ) );
  ERRORCHECK( SDsetattr( varId, "nj", DFNT_INT32, 1, &dims[1] ) );
  ERRORCHECK( SDsetattr( varId, "nk", DFNT_INT32, 1, &dims[0] ) );

  // write the data
  ERRORCHECK( SDwritedata(varId, indexStart, NULL, indexEnd, const_cast<float *>(data)) );
  
  ERRORCHECK( SDendaccess(varId) );
}

#if defined(APLUSPLUS) || defined(PPLUSPLUS)
/*******************************************************************************
 * Overloaded method to handle floatArray data types.
 * 
 * Note that changing the "data" type from floatArray requires
 * changing the "dataType" variable from DFNT_FLOAT32.
 ******************************************************************************/
void Hdf4::writeVariable(const string &variable, const floatArray &data, const string &units)
{
  const int rank = data.numberOfDimensions();
  int dims[MAX_VAR_DIMS];
  for (int i=0; i < rank; i++){
    /* Note the funkiness with the index=rank-1-i
     * 
     * A++ stores data in Fortran column ordering (ie. data(i,j) has
     * i the fastest index to loop over.
     *
     * C++ stores data in row ordering (ie. data[i][j] would have j be
     * the fastest index to loop over).
     * 
     * Because of this, we do some trickery with inverting the dimension index!
     */
    dims[rank-1-i] = data.getLength(i);
  }

  writeVariable(variable, 
		data.getDataPointer(), rank, dims,
		units);		
}
#endif
// defined(APLUSPLUS) || defined(PPLUSPLUS)
