#ifndef __IO_H__
#define __IO_H__

#if defined(APLUSPLUS) || defined(PPLUSPLUS)
#include <A++.h>
#endif

#include <map>
#include <string>


namespace IO
{
  static const int HDF4=0;
  static const int HDF5=1;
  static const int PHDF5=2;

  static const int READ=0;
  static const int WRITE=1;
}

/**
 * Abstract base class that defines an I/O interface
 *
 * Warning: typeArray stores data internally in Fortran column
 * ordering (ie. inner index loops fastest); C++ expects data in row
 * ordering (ie. outer index loops fastest).  This has strong
 * implications for Io::readVariable(...) and
 * Io::writeVariable(...)!  Make sure indices are oprdered properly!
 *
 * FIXME: Should we append attribute describing where the data is
 *        located (vertices, cell edges, center, etc.)?  ... Might
 *        alleviate confusion for new users & aide in automating
 *        visualization/post-processing I/O.
 *
 */
class Io
{
public:
  Io() {};
  virtual ~Io() {};
  virtual void open(const std::string &filename, const int &ioType) = 0;
  /// Routines for Reading:
  //@{
  virtual void readMetaData(std::map<std::string, double> &metaDoubles,
			    std::map<std::string, float> &metaFloats,
			    std::map<std::string, int> &metaInts,
			    std::map<std::string, std::string> &metaStrings) = 0;
  virtual void readString(const std::string&name, std::string &data) = 0;
#if defined(APLUSPLUS) || defined(PPLUSPLUS)
  virtual void readVariable(const std::string &variable, floatArray &data) = 0;
#endif
  virtual void readVariable(const std::string &variable, float *&data, int &rank, int *&dims) = 0;
  //@}

  ///Routines for Writing:
  //@{
  virtual void writeMetaData(const std::map<std::string, double> &metaDoubles,
			     const std::map<std::string, float> &metaFloats,
			     const std::map<std::string, int> &metaInts,
			     const std::map<std::string, std::string> &metaStrings) = 0;
  virtual void writeMetaData(const std::map<std::string, double> &metaDoubles) = 0;
  virtual void writeMetaData(const std::map<std::string, float> &metaFloats) = 0;
  virtual void writeMetaData(const std::map<std::string, int> &metaInts) = 0;
  virtual void writeMetaData(const std::map<std::string, std::string> &metaStrings) = 0;
  virtual void writeString(const std::string &name, const std::string &data) = 0;
#if defined(APLUSPLUS) || defined(PPLUSPLUS)
  virtual void writeVariable(const std::string &variable, const floatArray &data, const string &units) = 0;
#endif
  virtual void writeVariable(const std::string &variable, const float *data, const int &rank, const int *dims, const std::string &units) = 0;
  //@}

  virtual void close() = 0;
};

#endif
