//                                               -*- C++ -*-
/**
 *  @file  NumericalSampleImplementation.cxx
 *  @brief The class NumericalSampleImplementation implements blank free samples
 *
 *  Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author schueller
 *  @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)
 */
#include <limits>        // std::numeric_limits
#include <map>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <cstdio>        // std::fopen, std::errno
#include <cstring>       // std::strerror

#include "OTconfig.hxx"
#include "NumericalSampleImplementation.hxx"
#include "StorageManager.hxx"
#include "PersistentObjectFactory.hxx"
#include "Log.hxx"
#include "Exception.hxx"
#include "Path.hxx"
#include "Os.hxx"
#include "TBB.hxx"
#include "kendall.h"

#include "csv_parser_state.hxx"
#include "csv_parser.hh"
#include "csv_lexer.h"

int csvparse (OT::CSVParserState & theState, yyscan_t yyscanner, FILE * theFile, OT::NumericalSampleImplementation &impl, OT::UnsignedLong & theDimension, const char * separator);


BEGIN_NAMESPACE_OPENTURNS


TEMPLATE_CLASSNAMEINIT(PersistentCollection<NumericalPoint>);

static Factory<PersistentCollection<NumericalPoint> > RegisteredFactory_PC_NP("PersistentCollection<NumericalPoint>");

NSI_point::NSI_point(NumericalSampleImplementation * p_nsi, const UnsignedLong index)
  : p_nsi_(p_nsi), index_(index), dimension_(p_nsi->dimension_) {}

NSI_point & NSI_point::operator = (const NSI_point & rhs)
{
  if ( (this != &rhs) && (getDimension() == rhs.getDimension()) )
    std::copy( rhs.begin(), rhs.end(), begin() );
  return *this;
}

// NSI_point & NSI_point::operator = (const NSI_const_point & rhs)
// {
//   if ( getDimension() == rhs.getDimension() )
//     std::copy( rhs.begin(), rhs.end(), begin() );
//   return *this;
// }

// NSI_point & NSI_point::operator = (const NumericalPoint & rhs)
// {
//   if ( getDimension() == rhs.getDimension() )
//     std::copy( rhs.begin(), rhs.end(), begin() );
//   return *this;
// }

NumericalScalar & NSI_point::operator [] (UnsignedLong i)
{
  return p_nsi_->data_[index_ * dimension_ + i];
}

const NumericalScalar & NSI_point::operator [] (const UnsignedLong i) const
{
  return p_nsi_->data_[index_ * dimension_ + i];
}

NumericalScalar & NSI_point::at (UnsignedLong i)
{
  return p_nsi_->data_.at(index_ * dimension_ + i);
}

const NumericalScalar & NSI_point::at (const UnsignedLong i) const
{
  return p_nsi_->data_.at(index_ * dimension_ + i);
}

NSI_point & NSI_point::operator += (const NSI_point & other)
{
  if (getDimension() != other.getDimension())
    throw InvalidArgumentException(HERE)
      << "NumericalPoints of different dimensions cannot be added (LHS dimension = "
      << getDimension()
      << "; RHS dimension = "
      << other.getDimension();

  for (UnsignedLong i = 0; i < getDimension(); ++i) (*this)[i] += other[i];
  return *this;
}

NSI_point & NSI_point::operator -= (const NSI_point & other)
{
  if (getDimension() != other.getDimension())
    throw InvalidArgumentException(HERE)
      << "NumericalPoints of different dimensions cannot be substracted (LHS dimension = "
      << getDimension()
      << "; RHS dimension = "
      << other.getDimension();

  for (UnsignedLong i = 0; i < getDimension(); ++i) (*this)[i] -= other[i];
  return *this;
}

NSI_point & NSI_point::operator += (const NumericalPoint & other)
{
  if (getDimension() != other.getDimension())
    throw InvalidArgumentException(HERE)
      << "NumericalPoints of different dimensions cannot be added (LHS dimension = "
      << getDimension()
      << "; RHS dimension = "
      << other.getDimension();

  for (UnsignedLong i = 0; i < getDimension(); ++i) (*this)[i] += other[i];
  return *this;
}

NSI_point & NSI_point::operator -= (const NumericalPoint & other)
{
  if (getDimension() != other.getDimension())
    throw InvalidArgumentException(HERE)
      << "NumericalPoints of different dimensions cannot be substracted (LHS dimension = "
      << getDimension()
      << "; RHS dimension = "
      << other.getDimension();

  for (UnsignedLong i = 0; i < getDimension(); ++i) (*this)[i] -= other[i];
  return *this;
}

NSI_point & NSI_point::operator *= (const NumericalScalar val)
{
  for(UnsignedLong i = 0; i < getDimension(); ++i) (*this)[i] *= val;
  return *this;
}

NSI_point & NSI_point::operator /= (const NumericalScalar val)
{
  for(UnsignedLong i = 0; i < getDimension(); ++i) (*this)[i] /= val;
  return *this;
}


bool operator == (const NSI_point & lhs, const NSI_point & rhs)
{
  return (lhs.getDimension() == rhs.getDimension()) &&
    std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

bool operator != (const NSI_point & lhs, const NSI_point & rhs)
{
  return ! (lhs == rhs);
}

bool operator < (const NSI_point & lhs, const NSI_point & rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                      rhs.begin(), rhs.end(),
                                      std::less<NumericalScalar>());
}

bool operator > (const NSI_point & lhs, const NSI_point & rhs)
{
  return !( lhs <= rhs );
}

bool operator <= (const NSI_point & lhs, const NSI_point & rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                      rhs.begin(), rhs.end(),
                                      std::less_equal<NumericalScalar>());
}

bool operator >= (const NSI_point & lhs, const NSI_point & rhs)
{
  return !( lhs < rhs );
}





NSI_const_point::NSI_const_point(const NumericalSampleImplementation * p_nsi, const UnsignedLong index)
  : p_nsi_(p_nsi), index_(index), dimension_(p_nsi->dimension_) {}

NSI_const_point::NSI_const_point(const NSI_point & point)
  : p_nsi_(point.p_nsi_), index_(point.index_), dimension_(point.dimension_) {}

bool operator == (const NSI_const_point & lhs, const NSI_const_point & rhs)
{
  return (lhs.getDimension() == rhs.getDimension()) &&
    std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

const NumericalScalar & NSI_const_point::operator [] (const UnsignedLong i) const
{
  return p_nsi_->data_[index_ * dimension_ + i];
}

const NumericalScalar & NSI_const_point::at (const UnsignedLong i) const
{
  return p_nsi_->data_.at(index_ * dimension_ + i);
}

bool operator != (const NSI_const_point & lhs, const NSI_const_point & rhs)
{
  return ! (lhs == rhs);
}

bool operator < (const NSI_const_point & lhs, const NSI_const_point & rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                      rhs.begin(), rhs.end(),
                                      std::less<NumericalScalar>());
}

bool operator > (const NSI_const_point & lhs, const NSI_const_point & rhs)
{
  return !( lhs <= rhs );
}

bool operator <= (const NSI_const_point & lhs, const NSI_const_point & rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                      rhs.begin(), rhs.end(),
                                      std::less_equal<NumericalScalar>());
}

bool operator >= (const NSI_const_point & lhs, const NSI_const_point & rhs)
{
  return !( lhs < rhs );
}


typedef NumericalSampleImplementation (*BuildMethod) (const FileName & fileName, const String & parameters);

static Factory<NumericalSampleImplementation> RegisteredFactory_NSI("NumericalSampleImplementation");


/*
 * This class implements a map that stores a function pointer to a factory that builds
 * a NumericalSampleImplementation according to a file format
 */
class BuildMethodMap : public std::map<NumericalSampleImplementation::ExternalFileFormat, BuildMethod>
{
  typedef std::map<NumericalSampleImplementation::ExternalFileFormat, BuildMethod> ParentType;
  ParentType & table_;

public:
  BuildMethodMap()
    : std::map<NumericalSampleImplementation::ExternalFileFormat, BuildMethod>(),
      table_(*this)
  {
    table_[NumericalSampleImplementation::CSV] = NumericalSampleImplementation::BuildFromCSVFile;
  }

  const BuildMethod & operator[] (const NumericalSampleImplementation::ExternalFileFormat & format) const
  {
    return table_[format];
  }

}; /* end class BuildMethodMap */


static const BuildMethodMap SampleImportationFactoryMap;



CLASSNAMEINIT(NumericalSampleImplementation);


/* Constructor from file */
NumericalSampleImplementation NumericalSampleImplementation::GetFromFile(const FileName & fileName,
                                                                         const String & parameters,
                                                                         const ExternalFileFormat format)
{
  return SampleImportationFactoryMap[format](fileName, parameters);
}

/* Factory of NumericalSampleImplementation from CSV file */
NumericalSampleImplementation NumericalSampleImplementation::BuildFromCSVFile(const FileName & fileName,
                                                                              const String & csvSeparator)
{
  yyscan_t scanner = 0;
  NumericalSampleImplementation impl(0, 0);
  impl.setName(fileName);

  FILE * theFile = std::fopen(fileName.c_str(), "r");
  if (!theFile)
    {
      // theFile can not be found. Errno is set
      throw FileNotFoundException(HERE) << "Can NOT open file '" << fileName
                                        << "'. Reason: " << std::strerror(errno);
    }
  CSVParserState state;
  state.theFileName = fileName;

  csvlex_init(&scanner);
  csvparse(state, scanner, theFile, impl, impl.dimension_, csvSeparator.c_str());
  csvlex_destroy(scanner);
  std::fclose(theFile);
  // Check the description
  if (impl.p_description_.isNull() || (impl.p_description_->getSize() != impl.getDimension()))
    {
      const UnsignedLong dimension(impl.getDimension());
      Description defaultDescription(dimension);
      for (UnsignedLong i = 0; i < dimension; ++i)
        defaultDescription[i] = String(OSS() << "data_" << i);
      impl.setDescription(defaultDescription);
    }
  return impl;
}

/* Store a sample in a temporary text file, one realization by line. Returns the file name. */
String NumericalSampleImplementation::storeToTemporaryFile() const
{
  const String dataFileName(Path::BuildTemporaryFileName("RData.txt.XXXXXX"));
  std::ofstream dataFile(dataFileName.c_str());
  // Fill-in the data file
  for (UnsignedLong i = 0; i < size_; ++i)
    {
      String separator = "";
      for (UnsignedLong j = 0; j < dimension_; ++j, separator = " ")
        dataFile << separator << std::setprecision(16) << operator[](i)[j];
      dataFile << Os::GetEndOfLine();
    }
  dataFile.close();
  return dataFileName;
}

/* Export a sample as a matrix, one row by realization, in a format suitable to exchange with R */
String NumericalSampleImplementation::streamToRFormat() const
{
  OSS oss;
  oss.setPrecision(16);
  oss << "matrix(c(";
  String separator("");
  for (UnsignedLong j = 0; j < dimension_; ++j)
    for (UnsignedLong i = 0; i < size_; ++i, separator = ",")
      oss << separator << operator[](i)[j];
  oss << "), nrow=" << size_ << ", ncol=" << dimension_ << ")";
  return oss;
}

/* Default constructor is private */
NumericalSampleImplementation::NumericalSampleImplementation()
  : PersistentObject(),
    size_(0),
    dimension_(0),
    data_(size_ * dimension_, 0.0),
    p_description_()
{
  // Nothing to do
}

/* Standard constructor */
NumericalSampleImplementation::NumericalSampleImplementation(const UnsignedLong size,
                                                             const UnsignedLong dim)
  : PersistentObject(),
    size_(size),
    dimension_(dim),
    data_(size_ * dimension_, 0.0),
    p_description_()
{
  // Nothing to do
}

/* Constructor from a NumericalPoint */
NumericalSampleImplementation::NumericalSampleImplementation(const UnsignedLong size,
                                                             const NumericalPoint & point)
  : PersistentObject(),
    size_(size),
    dimension_(point.getDimension()),
    data_(size_ * dimension_, 0.0),
    p_description_()
{
  for (UnsignedLong i = 0; i < size_; ++i)
    for (UnsignedLong j = 0; j < dimension_; ++j)
      data_[i * dimension_ + j] = point[j];
}


/* Constructor from a collection of NumericalPoint */
NumericalSampleImplementation::NumericalSampleImplementation(const Collection<NumericalPoint> & coll)
  : PersistentObject(),
    size_(coll.getSize()),
    dimension_((coll.getSize() > 0) ? coll[0].getDimension() : 0),
    data_(size_ * dimension_, 0.0),
    p_description_()
{
  for (UnsignedLong i = 0; i < size_; ++i)
    for (UnsignedLong j = 0; j < dimension_; ++j)
      data_[i * dimension_ + j] = coll[i][j];
}

/* Partial copy constructor */
NumericalSampleImplementation::NumericalSampleImplementation(const NumericalSampleImplementation & other, iterator first, iterator last)
  : PersistentObject(),
    size_(last - first),
    dimension_(other.getDimension()),
    data_(size_ * dimension_, 0.0),
    p_description_(other.p_description_)
{
  std::copy( first, last, begin() );
}


/* Virtual constructor */
NumericalSampleImplementation * NumericalSampleImplementation::clone() const
{
  return new NumericalSampleImplementation(*this);
}



void NumericalSampleImplementation::swap_points(const UnsignedLong a, const UnsignedLong b)
{
  std::swap_ranges( &data_[ a * dimension_ ], &data_[ (a + 1) * dimension_ ], &data_[ b * dimension_ ] );
}

void NumericalSampleImplementation::swap_range_points(const UnsignedLong fa, const UnsignedLong ta, const UnsignedLong fb)
{
  for (UnsignedLong i = 0; i < ta - fa; ++i) swap_points( fa + i, fb + i );
}



/* Description Accessor */
void NumericalSampleImplementation::setDescription(const Description & description)
{
  if (description.getSize() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given description does not match the sample dimension.";
  p_description_ = description.getImplementation();
}


/* Description Accessor */
Description NumericalSampleImplementation::getDescription() const
{
  return p_description_.isNull() ? Description(getDimension()) : *p_description_;
}

/* Comparison function */
Bool operator ==(const NumericalSampleImplementation & lhs, const NumericalSampleImplementation & rhs)
{
  return (lhs.size_ == rhs.size_) && (lhs.dimension_ == rhs.dimension_) &&
    std::equal( lhs.begin(), lhs.end(), rhs.begin() );
}

void NumericalSampleImplementation::erase(iterator first, iterator last)
{
  PersistentCollection<NumericalScalar>::iterator f = data_.begin() + (first - begin()) * dimension_;
  PersistentCollection<NumericalScalar>::iterator l = data_.begin() + (last - begin()) * dimension_;
  data_.erase( f, l );
  size_ -= last - first;
}

void NumericalSampleImplementation::erase(const UnsignedLong first, const UnsignedLong last)
{
  PersistentCollection<NumericalScalar>::iterator f = data_.begin() + first * dimension_;
  PersistentCollection<NumericalScalar>::iterator l = data_.begin() + last * dimension_;
  data_.erase( f, l );
  size_ -= last - first;
}

void NumericalSampleImplementation::clear()
{
  data_.clear();
  size_ = 0;
}


/* Raw internal format accessor */
NumericalPoint NumericalSampleImplementation::getData() const
{
  return data_;
}

void NumericalSampleImplementation::setData(const Collection<NumericalScalar> & data)
{
  if (data.getSize() != dimension_ * size_) throw InvalidArgumentException(HERE) << "Error: the given raw data are not compatible with the dimension and size of the sample.";
  data_ = data;
}

/* Method __contains__() is for Python */
Bool NumericalSampleImplementation::contains(const NumericalPoint & val) const
{
  for (UnsignedLong i = 0; i < size_; ++i) if ( (*this)[i] == val ) return true;
  return false;
}

/* String converter */
String NumericalSampleImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << NumericalSampleImplementation::GetClassName()
      << " name=" << getName()
      << " size=" << size_
      << " dimension=" << dimension_
      << " data=[";
  const char * sep = "";
  for(const_iterator it = begin(); it != end(); ++it, sep = ",") oss << sep << *it;
  oss << "]";
  return oss;
}

String NumericalSampleImplementation::__str__(const String & offset) const
{
  // First, print the description if it is not empty.
  // If you use the getDescription() method you get a default value
  // for the description that is not stored in the sample, producing a spurious output
  const Bool printDescription = !p_description_.isNull() && (p_description_->getSize() == dimension_);

  size_t twidth = 0; // column title max width
  size_t lwidth = 0; // LHS number max width
  size_t rwidth = 0; // RHS number max width
  size_t iwidth = 0; // index max width

  if (printDescription)
    {
      for( UnsignedLong j = 0; j < dimension_; ++j )
        twidth = std::max( twidth, (*p_description_)[j].size() );
    }

  for( UnsignedLong i = 0; i < size_; ++i )
    for( UnsignedLong j = 0; j < dimension_; ++j )
      {
        String st = OSS() << data_[i * dimension_ + j];
        size_t dotpos = st.find( '.' );
        lwidth = std::max( lwidth, (dotpos != String::npos) ? dotpos             : st.size() );
        rwidth = std::max( rwidth, (dotpos != String::npos) ? st.size() - dotpos : 0         );
      }

  if (twidth > lwidth + rwidth)
    rwidth = twidth - lwidth;
  else
    twidth = lwidth + rwidth;

  {
    // Computing the size of the last index (max width of the indexes)
    String sti = OSS() << size_ - 1;
    iwidth = sti.size();
  }

  OSS oss;
  // Print the column title
  if (printDescription)
    {
      oss << offset << String( iwidth , ' ' ) << "   [ ";
      const char * sep = "";
      for( UnsignedLong j = 0; j < dimension_; ++j, sep = " " )
        {
          oss << sep << (*p_description_)[j] << String( twidth - (*p_description_)[j].size(), ' ' );
        }
      oss << " ]\n";
    }

  const char * newline = "";
  for( UnsignedLong i = 0; i < size_; ++i, newline = "\n" )
    {
      String sti = OSS() << i;
      oss << newline << offset << String( iwidth - sti.size(), ' ' ) << sti << " : [ ";
      const char * sep = "";
      for( UnsignedLong j = 0; j < dimension_; ++j, sep = " " )
        {
          String st = OSS() << data_[i * dimension_ + j];
          size_t dotpos = st.find( '.' );
          oss << sep << String( lwidth - ((dotpos != String::npos) ? dotpos : st.size()), ' ' )
              << st
              << String( rwidth - ((dotpos != String::npos) ? st.size() - dotpos : 0), ' ' );
        }
      oss << " ]";
    }
  return oss;
}





/* Appends an element to the collection */
NumericalSampleImplementation & NumericalSampleImplementation::add(const NumericalPoint & point)
{
  if ( (dimension_ != 0) && (point.getDimension() != dimension_) )
    throw InvalidArgumentException(HERE) << "Point has invalid dimension ("
                                         << point.getDimension()
                                         << ") expected : "
                                         << getDimension();
  const UnsignedLong oldSize = size_;
  ++size_;
  data_.resize( size_ * dimension_ );
  //for(UnsignedLong j=0; j<dimension_; ++j)
  //  data_[(size_-1)*dimension_ + j] = point[j];
  memcpy( &data_[oldSize * dimension_], &point[0], dimension_ * sizeof(NumericalScalar) );
  return *this;
}


/* Appends another sample to the collection */
NumericalSampleImplementation & NumericalSampleImplementation::add(const NumericalSampleImplementation & sample)
{
  if ( (dimension_ != 0) && (sample.getDimension() != dimension_) )
    throw InvalidArgumentException(HERE) << "Sample has invalid dimension ("
                                         << sample.getDimension()
                                         << ") expected : "
                                         << getDimension();
  const UnsignedLong oldSize = size_;
  size_ += sample.getSize();
  data_.resize( size_ * dimension_ );
  memmove( &data_[oldSize * dimension_], &(sample.data_[0]), sample.getSize() * dimension_ * sizeof(NumericalScalar) );
  return *this;
}


struct AddPolicy
{
  typedef NumericalPoint value_type;

  static inline value_type GetInvariant(const NumericalSampleImplementation & nsi)
  {
    return value_type(nsi.getDimension(), 0.0);
  }


  template <typename T>
  static inline value_type & inplace_op( value_type & a, const T & pt )
  {
    const UnsignedLong dim = a.getDimension();
    for (UnsignedLong i = 0; i < dim; ++i) a[i] += pt[i];
    return a;
  }
}; /* end struct AddPolicy */

template <typename OP>
struct ReductionFunctor
{
  const NumericalSampleImplementation & nsi_;
  const OP & op_;
  typename OP::value_type accumulator_;

  ReductionFunctor(const NumericalSampleImplementation & nsi, const OP & op = OP())
    : nsi_(nsi), op_(op), accumulator_(OP::GetInvariant(nsi_)) {}

  ReductionFunctor(const ReductionFunctor & other, TBB::Split)
    : nsi_(other.nsi_), op_(other.op_), accumulator_(OP::GetInvariant(nsi_)) {}

  void operator() (const TBB::BlockedRange<UnsignedLong> & r)
  {
    for (UnsignedLong i = r.begin(); i != r.end(); ++i) op_.inplace_op( accumulator_, nsi_[i] );
  }

  void join(const ReductionFunctor & other)
  {
    op_.inplace_op( accumulator_, other.accumulator_ );
  }

}; /* end struct ReductionFunctor */


template <typename OP>
class ParallelFunctor
{
  NumericalSampleImplementation & nsi_;
  const OP & op_;
public:
  ParallelFunctor(NumericalSampleImplementation & nsi, const OP & op) : nsi_(nsi), op_(op) {}

  void operator() (const TBB::BlockedRange<UnsignedLong> & r) const
  {
    for (UnsignedLong i = r.begin(); i != r.end(); ++i) op_.inplace_op( nsi_[i] );
  }

}; /* end class ParallelFunctor */



/*
 * Gives the mean of the sample, based on the formula
 * mean = sum of the elements in the sample / size of the sample
 */
NumericalPoint NumericalSampleImplementation::computeMean() const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the mean of an empty sample.";
  ReductionFunctor<AddPolicy> functor( *this );
  TBB::ParallelReduce( 0, size_, functor );
  return functor.accumulator_ * (1.0 / size_);
}

struct CovariancePolicy
{
  typedef NumericalPoint value_type;

  const value_type & mean_;
  const UnsignedLong dimension_;

  CovariancePolicy( const value_type & mean)
    : mean_(mean), dimension_(mean_.getDimension()) {}

  static inline value_type GetInvariant(const NumericalSampleImplementation & nsi)
  {
    return value_type(nsi.getDimension() * nsi.getDimension(), 0.0);
  }

  inline value_type & inplace_op( value_type & var, NSI_const_point point ) const
  {
    UnsignedLong baseIndex(0);
    for (UnsignedLong i = 0; i < dimension_; ++i)
      {
	const NumericalScalar deltaI(point[i] - mean_[i]);
	for (UnsignedLong j = i; j < dimension_; ++j)
	  {
	    const NumericalScalar deltaJ(point[j] - mean_[j]);
	    var[baseIndex + j] += deltaI * deltaJ;
	  }
	baseIndex += dimension_;
      }
    return var;
  }

  static inline value_type & inplace_op( value_type & var, const value_type & point )
  {
    return var += point;
  }

}; /* end struct CovariancePolicy */

/*
 * Gives the covariance matrix of the sample, normalization by 1 / (size - 1) if size > 1
 */
CovarianceMatrix NumericalSampleImplementation::computeCovariance() const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the covariance of an empty sample.";
  // Special case for a sample of size 1
  if (size_ == 1) return CovarianceMatrix(dimension_, NumericalPoint(dimension_ * dimension_));

  const NumericalPoint mean(computeMean());

  const CovariancePolicy policy ( mean );
  ReductionFunctor<CovariancePolicy> functor( *this, policy );
  TBB::ParallelReduce( 0, size_, functor );
  CovarianceMatrix result(dimension_, functor.accumulator_ / (size_ - 1));
  return result;
}

/*
 * Gives the standard deviation of the sample, i.e. the square-root of the covariance matrix.
 */
SquareMatrix NumericalSampleImplementation::computeStandardDeviation() const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the standard deviation of an empty sample.";
  return computeCovariance().computeCholesky();
}


struct VariancePerComponentPolicy
{
  typedef NumericalPoint value_type;

  const value_type & mean_;
  const UnsignedLong dimension_;

  VariancePerComponentPolicy( const value_type & mean)
    : mean_(mean), dimension_(mean_.getDimension()) {}

  static inline value_type GetInvariant(const NumericalSampleImplementation & nsi)
  {
    return value_type(nsi.getDimension(), 0.0);
  }

  inline value_type & inplace_op( value_type & var, NSI_const_point point ) const
  {
    for (UnsignedLong i = 0; i < dimension_; ++i)
      {
	const NumericalScalar val(point[i] - mean_[i]);
	var[i] += val * val;
      }
    return var;
  }

  static inline value_type & inplace_op( value_type & var, const value_type & point )
  {
    return var += point;
  }

}; /* end struct VariancePerComponentPolicy */

/*
 * Gives the variance of the sample (by component)
 */
NumericalPoint NumericalSampleImplementation::computeVariancePerComponent() const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the variance per component of an empty sample.";

  // Special case for a sample of size 1
  if (size_ == 1) return NumericalPoint(dimension_, 0.0);
  const NumericalPoint mean( computeMean() );

  const VariancePerComponentPolicy policy ( mean );
  ReductionFunctor<VariancePerComponentPolicy> functor( *this, policy );
  TBB::ParallelReduce( 0, size_, functor );
  return functor.accumulator_ / (size_ - 1);
}

/*
 * Gives the standard deviation of each component of the sample
 */
NumericalPoint NumericalSampleImplementation::computeStandardDeviationPerComponent() const
{
  NumericalPoint sd(computeVariancePerComponent());
  for (UnsignedLong i = 0; i < dimension_; ++i) sd[i] = sqrt(sd[i]);

  return sd;
}



/*
 * Gives the Pearson correlation matrix of the sample
 */
CorrelationMatrix NumericalSampleImplementation::computePearsonCorrelation() const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the Pearson correlation of an empty sample.";
  CorrelationMatrix correlation(dimension_);
  if (dimension_ == 1) return correlation;

  const CovarianceMatrix covariance(computeCovariance());
  NumericalPoint sd(dimension_);
  for (UnsignedLong i = 0; i < dimension_; ++i)
    {
      sd[i] = sqrt( covariance(i, i) );
      for (UnsignedLong j = 0; j < i; ++j)
        correlation(i, j) = covariance(i, j) / (sd[i] * sd[j]);
    }

  return correlation;
}

struct Pair
{
  NumericalScalar value_;
  UnsignedLong index_;
  Pair() : value_(0.0), index_() {}
  Pair(NumericalScalar value, UnsignedLong index) : value_(value), index_(index) {}
  Bool operator < (const Pair & other) const
  {
    return value_ < other.value_;
  }
};

typedef Collection<Pair>                   PairCollection;
typedef Collection<PairCollection>         PairCollectionCollection;
typedef Collection<UnsignedLong>           UnsignedLongCollection;
typedef Collection<UnsignedLongCollection> UnsignedLongCollectionCollection;




struct Comparison
{
  // Main sorting key
  UnsignedLong first_;
  // Secondary sorting key
  UnsignedLong second_;
  // Pointer to the data
  const NumericalSampleImplementation & nsi_;
  // Sorting permutation
  UnsignedLongCollection permutation_;
  // True if sample has ties
  mutable Bool hasTies_;

  Comparison(UnsignedLong first,
             const NumericalSampleImplementation & nsi)
    : first_(first), second_(first), nsi_(nsi), permutation_(nsi_.getSize()), hasTies_(false)
  {
    const UnsignedLong size = nsi_.getSize();
    for (UnsignedLong i = 0; i < size; ++i) permutation_[i] = i;
  }

  Comparison(UnsignedLong first, UnsignedLong second,
             const NumericalSampleImplementation & nsi)
    : first_(first), second_(second), nsi_(nsi), permutation_(nsi_.getSize()), hasTies_(false)
  {
    const UnsignedLong size = nsi_.getSize();
    for (UnsignedLong i = 0; i < size; ++i) permutation_[i] = i;
  }

  Bool operator() (const UnsignedLong i, const UnsignedLong j) const
  {
    const NumericalScalar xI(nsi_[ permutation_[i] ][ first_  ]);
    const NumericalScalar xJ(nsi_[ permutation_[j] ][ first_  ]);
    const NumericalScalar yI(nsi_[ permutation_[i] ][ second_ ]);
    const NumericalScalar yJ(nsi_[ permutation_[j] ][ second_ ]);
    hasTies_ |= (xI == xJ);
    return ( (xI < xJ) || ((xI == xJ) && (yI < yJ)) );
  }
}; // struct Comparison




/* Ranked sample */
NumericalSampleImplementation NumericalSampleImplementation::rank() const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot rank an empty sample.";
  NumericalSampleImplementation rankedSample(size_, dimension_);

  PairCollectionCollection sortedMarginalSamples(dimension_, PairCollection(size_));

  // Sort and rank all the marginal samples
  for (UnsignedLong i = 0; i < dimension_; ++i)
    {
      for (UnsignedLong j = 0; j < size_; ++j)
        {
          sortedMarginalSamples[i][j].value_ = (*this)[j][i];
          sortedMarginalSamples[i][j].index_ = j;
        }
      // sort
      TBB::ParallelSort(sortedMarginalSamples[i].begin(), sortedMarginalSamples[i].end());
      // rank
      for (UnsignedLong j = 0; j < size_; ++j)
        rankedSample[ sortedMarginalSamples[i][j].index_ ][i] = j;
    }

  return rankedSample;
}

/* Ranked component */
NumericalSampleImplementation NumericalSampleImplementation::rank(const UnsignedLong index) const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot rank an empty sample.";
  if (index >= dimension_) throw OutOfBoundException(HERE) << "The requested index is too large, index=" << index << ", dimension=" << dimension_;

  NumericalSampleImplementation rankedSample(size_, 1);

  PairCollectionCollection sortedMarginalSamples(1, PairCollection(size_));

  // Sort and rank the marginal sample number index
  for (UnsignedLong j = 0; j < size_; ++j)
    {
      sortedMarginalSamples[0][j].value_ = (*this)[j][index];
      sortedMarginalSamples[0][j].index_ = j;
    }
  // sort
  TBB::ParallelSort(sortedMarginalSamples[0].begin(), sortedMarginalSamples[0].end());
  // rank
  for (UnsignedLong j = 0; j < size_; ++j)
    rankedSample[ sortedMarginalSamples[0][j].index_ ][0] = j;

  return rankedSample;
}

/* Sorted sample, component by component */
NumericalSampleImplementation NumericalSampleImplementation::sort() const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot sort an empty sample.";

  NumericalSampleImplementation sortedSample(size_, dimension_);
  Collection<NumericalScalar> component(size_);

  // Sort all the marginal samples
  for (UnsignedLong i = 0; i < dimension_; ++i)
    {
      for (UnsignedLong j = 0; j < size_; ++j)
        component[j] = (*this)[j][i];
      // sort
      TBB::ParallelSort(component.begin(), component.end());

      // copy
      for (UnsignedLong j = 0; j < size_; ++j)
        sortedSample[j][i] = component[j];
    } // loop over dimension

  return sortedSample;
}

/* Sorted sample, one component */
NumericalSampleImplementation NumericalSampleImplementation::sort(const UnsignedLong index) const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot sort an empty sample.";

  if (index >= getDimension()) throw OutOfBoundException(HERE) << "The requested index is too large, index=" << index << ", dimension=" << getDimension();

  NumericalSampleImplementation sortedSample(size_, 1);
  Collection<NumericalScalar> component(size_);

  // Sort the requested component
  for (UnsignedLong j = 0; j < size_; ++j)
    component[j] = (*this)[j][index];

  // sort
  TBB::ParallelSort(component.begin(), component.end());

  // copy
  for (UnsignedLong j = 0; j < size_; ++j)
    sortedSample[j][0] = component[j];

  return sortedSample;
}

/* Sorted according a component */
NumericalSampleImplementation NumericalSampleImplementation::sortAccordingToAComponent(const UnsignedLong index) const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot sort an empty sample.";

  const NumericalSampleImplementation rankedIndex(rank(index));
  NumericalSampleImplementation result(size_, dimension_);
  for (UnsignedLong i = 0; i < size_; ++i)
    result[static_cast<UnsignedLong>( round(rankedIndex[i][0]) ) ] = (*this)[i];

  return result;
}

/*
 * Gives the Spearman correlation matrix of the sample
 */
CorrelationMatrix NumericalSampleImplementation::computeSpearmanCorrelation() const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the Spearman correlation of an empty sample.";

  return rank().computePearsonCorrelation();
}

/*
 * Gives the Kendall tau matrix of the sample
 * The correction for ties should be made according to http://www.statsdirect.com/help/nonparametric_methods/kend.htm
 */

struct MergeSortParameters
{
  const NumericalSampleImplementation & nsi_;
  const UnsignedLong activeDimension_;
  UnsignedLongCollection & ordering_;
  UnsignedLongCollection & buffer_;

  MergeSortParameters(const NumericalSampleImplementation & nsi,
                      const UnsignedLong activeDimension,
                      UnsignedLongCollection & ordering,
                      UnsignedLongCollection & buffer)
    : nsi_(nsi), activeDimension_(activeDimension), ordering_(ordering), buffer_(buffer) {}

}; // end struct MergeSortParameters

static inline
// uint64_t mergeSortedChunks(const NumericalSampleImplementation & nsi,
//                           const UnsignedLong offset,
//                           const UnsignedLong length,
//                           const UnsignedLong activeDimension,
//                           UnsignedLongCollection & ordering,
//                           UnsignedLongCollection & buffer)
uint64_t mergeSortedChunks(const MergeSortParameters & params,
                           const UnsignedLong offset,
                           const UnsignedLong length)
{
  uint64_t exchanges(0);

  const NumericalSampleImplementation & nsi = params.nsi_;
  const UnsignedLong activeDimension        = params.activeDimension_;
  UnsignedLongCollection & ordering         = params.ordering_;
  UnsignedLongCollection & buffer           = params.buffer_;

  const UnsignedLong length0(length / 2);
  const UnsignedLong length1(length - length0);
  const UnsignedLong middle(offset + length0);

  UnsignedLong i(0);
  UnsignedLong j(0);
  UnsignedLong k(0);

  if (nsi[ ordering[middle - 1] ][activeDimension] < nsi[ ordering[middle] ][activeDimension]) return exchanges;

  // Merge the left and right arrays
  while ((j < length0) || (k < length1))
    {
      int d(0);
      if ( (k >= length1) ||
           ( (j < length0) &&
             (nsi[ ordering[offset + j] ][activeDimension] <= nsi[ ordering[middle + k] ][activeDimension]) ))
        {
          buffer[offset + i] = ordering[offset + j];
          d = i - j;
          ++j;

        }
      else
        {
          buffer[offset + i] = ordering[middle + k];
          d = (offset + i) - (middle + k);
          ++k;
        }

      if (d > 0) exchanges += d;
      ++i;
    } // merge of the arrays

  for (UnsignedLong m = 0; m < length; ++m)
    ordering[offset + m] = buffer[offset + m];

  return exchanges;
}

/* Private merge sort for the fast computation of Kendall's tau */
static inline
uint64_t mergeSort(const MergeSortParameters & params,
                   const UnsignedLong offset,
                   const UnsignedLong length)
{
  uint64_t exchanges(0);

  const NumericalSampleImplementation & nsi = params.nsi_;
  const UnsignedLong activeDimension        = params.activeDimension_;
  UnsignedLongCollection & ordering         = params.ordering_;

  assert( length > 0);
  // Array of length 1, no swap
  if (length == 1) return 0;

  // Swap?
  if (length == 2)
    {
      if ( nsi[ ordering[offset] ][activeDimension] <= nsi[ ordering[offset + 1] ][activeDimension]) return 0;
      std::swap( ordering[offset], ordering[offset + 1] );
      return 1;
    }

  const UnsignedLong length0(length / 2);
  const UnsignedLong length1(length - length0);
  const UnsignedLong middle(offset + length0);

  // Recursive sort of left and right parts
  exchanges += mergeSort( params, offset, length0 );
  exchanges += mergeSort( params, middle, length1 );
  exchanges += mergeSortedChunks( params, offset, length );
  return exchanges;
}

/* Lexical ordering between two dimensions: ranking with respect to i then to j if ties */
static inline
NumericalSampleImplementation::UnsignedLongCollection lexicalRank(const NumericalSampleImplementation & nsi,
                                                                  const UnsignedLong i,
                                                                  const UnsignedLong j)
{
  // Sort and rank all the marginal samples
  Comparison comparison(i, j, nsi);
  std::stable_sort(comparison.permutation_.begin(), comparison.permutation_.end(), comparison);
  return comparison.permutation_;
}


#ifdef HAVE_TBB
struct MergeSortContinuationTask : public tbb::task
{
  const MergeSortParameters & params_;
  const UnsignedLong offset_;
  const UnsignedLong length_;
  uint64_t & exchanges_;
  uint64_t exchangesLeft_;
  uint64_t exchangesRight_;

  MergeSortContinuationTask(const MergeSortParameters & params,
                            UnsignedLong offset,
                            UnsignedLong length,
                            uint64_t & exchanges)
    : params_(params), offset_(offset), length_(length),
      exchanges_(exchanges), exchangesLeft_(0), exchangesRight_(0) {}

  tbb::task * execute()
  {
    exchanges_ += exchangesLeft_;
    exchanges_ += exchangesRight_;
    exchanges_ += mergeSortedChunks( params_, offset_, length_ );
    return NULL;
  }
}; // end class MergeSortContinuationTask

struct MergeSortTask : public tbb::task
{
  const MergeSortParameters & params_;
  const UnsignedLong offset_;
  const UnsignedLong length_;
  uint64_t & exchanges_;

  MergeSortTask(const MergeSortParameters & params,
                UnsignedLong offset,
                UnsignedLong length,
                uint64_t & exchanges)
    : params_(params), offset_(offset), length_(length),
      exchanges_(exchanges)
  {}

  tbb::task * execute()
  {
    if (length_ < 500)
      {
        exchanges_ = mergeSort( params_, offset_, length_ );

      }
    else
      {
        MergeSortContinuationTask & cont = *new( allocate_continuation() ) MergeSortContinuationTask( params_, offset_, length_, exchanges_);

        const UnsignedLong length0 = length_ / 2;
        const UnsignedLong length1 = length_ - length0;
        const UnsignedLong middle  = offset_ + length0;

        MergeSortTask & left  = *new( cont.allocate_child() ) MergeSortTask( params_, offset_, length0, cont.exchangesLeft_  );
        MergeSortTask & right = *new( cont.allocate_child() ) MergeSortTask( params_, middle, length1, cont.exchangesRight_ );

        cont.set_ref_count( 2 );
        cont.spawn( right );
        cont.spawn( left  );

      }
    return NULL;
  }

}; // end class MergeSortTask
#endif /* HAVE_TBB */

class KendallTauFunctor
{
  const NumericalSampleImplementation & nsi_;
  const Comparison & comp_;
  MatrixImplementation & tau_;
  const UnsignedLong i_;
  const NumericalScalar total_;
public:
  KendallTauFunctor(const NumericalSampleImplementation & nsi,
                    const Comparison & comp,
                    UnsignedLong i,
                    const NumericalScalar total,
                    MatrixImplementation & tau)
    : nsi_(nsi), comp_(comp), tau_(tau), i_(i), total_(total)
  {}

  void operator() (const TBB::BlockedRange<UnsignedLong> & r) const
  {
    const UnsignedLong size = nsi_.getSize();
    UnsignedLongCollection buffer(size);
    UnsignedLongCollection ordering;

    for (UnsignedLong j = r.begin(); j < r.end(); ++j)
      {
        // If there are ties according to X_i, solve the ties by a lexical sorting of (X_i, X_j)
        if (comp_.hasTies_) ordering = lexicalRank(nsi_, i_, j);
        else ordering = comp_.permutation_;

        NumericalScalar t = 0.0;
        NumericalScalar u = 0.0;
        NumericalScalar v = 0.0;
        UnsignedLong firstT = 0;
        UnsignedLong firstU = 0;
        UnsignedLong firstV = 0;

        // Check for ties
        // joint ties
        for (UnsignedLong k = 1; k < size; ++k)
          {
            if ( ( nsi_[ordering[firstT]][i_] != nsi_[ordering[k]][i_] ) ||
                 ( nsi_[ordering[firstT]][j ] != nsi_[ordering[k]][j ] ) )
              {
                t += (k - firstT) * (k - firstT - 1);
                firstT = k;
              }
            if (nsi_[ordering[firstU]][i_] != nsi_[ordering[k]][i_])
              {
                u += (k - firstU) * (k - firstU - 1);
                firstU = k;
              }
          } // ties in x and y

        t += (size - firstT) * (size - firstT - 1);
        u += (size - firstU) * (size - firstU - 1);

        // swaps
        // It is casted into a NumericalScalar as it will be used in an expression using real values
        MergeSortParameters params( nsi_, j, ordering, buffer );
#ifdef HAVE_TBB
        uint64_t exchangesWork = 0;
        MergeSortTask & work  = *new( tbb::task::allocate_root() ) MergeSortTask( params, 0, size, exchangesWork );
        tbb::task::spawn_root_and_wait( work );
        const NumericalScalar exchanges = exchangesWork;
#else /* HAVE_TBB */
        const NumericalScalar exchanges = mergeSort( params, 0, size );
#endif /* HAVE_TBB */

        // ties in y after mergesort with counting
        for (UnsignedLong k = 1; k < size; ++k)
          {
            if ( nsi_[ordering[firstV]][j] != nsi_[ordering[k]][j] )
              {
                v += (k - firstV) * (k - firstV - 1);
                firstV = k;
              }
          } // ties in y after mergesort and counting

        v += (size - firstV) * (size - firstV - 1);

        tau_(i_, j) = ((total_ == u) && (total_ == v)) ?
          1.0 :
          ((total_ - (v + u - t)) - 4.0 * exchanges) / (sqrt((total_ - u) * (total_ - v)));

      } // end for j

  }
}; // end class KendallTauFunctor

/*
 * Gives the Kendall tau matrix of the sample, including ties correction
 */
CorrelationMatrix NumericalSampleImplementation::computeKendallTau() const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the Kendall tau of an empty sample.";

#define KENDALLTAU_PARALLEL 0

#if KENDALLTAU_PARALLEL
  const NumericalScalar total = size_ * (size_ - 1);
#else
  // Use external efficient C implementation of the O(Nlog(N)) or O(N^2) Kendall tau computation depending on the sample size
  Bool useSmallCase(size_ < ResourceMap::GetAsUnsignedLong("NumericalSampleImplementation-SmallKendallTau"));
  NumericalPoint x(size_);
  NumericalPoint y(size_);
#endif
  // The resulting matrix
  CorrelationMatrix tau(dimension_);

  for (UnsignedLong i = 0; i < dimension_ - 1; ++i)
    {
#if KENDALLTAU_PARALLEL
      Comparison comparison(i, i, *this);
      std::stable_sort(comparison.permutation_.begin(), comparison.permutation_.end(), comparison);
      KendallTauFunctor functor( *this, comparison, i, total, *tau.getImplementation() );
      TBB::ParallelFor( i + 1, dimension_, functor );
#else /* KENDALLTAU_PARALLEL */
      for (UnsignedLong j = i + 1; j < dimension_; ++j)
        {
          LOGINFO(OSS() << "in NumericalSampleImplementation::computeKendallTau(), computing tau[" << i << ", " << j << "]");
          // We have to copy both the row i and j at each call to the external computation of Kendall's tau as the arrays will be modified in place
          for (UnsignedLong k = 0; k < size_; ++k)
            {
              x[k] = (*this)[k][i];
              y[k] = (*this)[k][j];
            }
          if (useSmallCase) tau(i, j) = kendallSmallN(&x[0], &y[0], size_);
          else tau(i, j) = kendallNlogN(&x[0], &y[0], size_);
          LOGINFO(OSS() << "value=" << tau(i, j));
        }
#endif /* KENDALLTAU_PARALLEL */
    } // end for i

  return tau;
}

/*
 * Gives the range of the sample (by component)
 */
NumericalPoint NumericalSampleImplementation::computeRangePerComponent() const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the range per component of an empty sample.";
  return getMax() - getMin();
}

/*
 * Gives the median of the sample (by component)
 */
NumericalPoint NumericalSampleImplementation::computeMedianPerComponent() const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the median per component of an empty sample.";
  return computeQuantilePerComponent(0.5);
}

struct SkewnessPerComponentPolicy
{
  typedef NumericalPoint value_type;

  const value_type & mean_;
  const UnsignedLong dimension_;

  SkewnessPerComponentPolicy( const value_type & mean)
    : mean_(mean), dimension_(mean_.getDimension()) {}

  static inline value_type GetInvariant(const NumericalSampleImplementation & nsi)
  {
    return value_type(2 * nsi.getDimension(), 0.0);
  }

  inline value_type & inplace_op( value_type & var, NSI_const_point point ) const
  {
    for (UnsignedLong i = 0; i < dimension_; ++i)
      {
	const NumericalScalar val(point[i] - mean_[i]);
	const NumericalScalar val2(val * val);
	var[i] += val2;
	var[i + dimension_] += val2 * val;
      }
    return var;
  }

  static inline value_type & inplace_op( value_type & var, const value_type & point )
  {
    return var += point;
  }

}; /* end struct SkewnessPerComponentPolicy */

/*
 * Gives the skewness of the sample (by component)
 */
NumericalPoint NumericalSampleImplementation::computeSkewnessPerComponent() const
{
  if (size_ <= 2) throw InternalException(HERE) << "Error: cannot compute the skewness per component of a sample of size less than 3.";

  const NumericalPoint mean(computeMean());
  const SkewnessPerComponentPolicy policy ( mean );
  ReductionFunctor<SkewnessPerComponentPolicy> functor( *this, policy );
  TBB::ParallelReduce( 0, size_, functor );
  NumericalPoint skewness(dimension_);
  const NumericalScalar factor(size_ * sqrt(size_ - 1) / (size_ - 2));
  for (UnsignedLong i = 0; i < dimension_; ++i) skewness[i] = factor * functor.accumulator_[i + dimension_] / pow(functor.accumulator_[i], 1.5);
  return skewness;
}

struct KurtosisPerComponentPolicy
{
  typedef NumericalPoint value_type;

  const value_type & mean_;
  const UnsignedLong dimension_;

  KurtosisPerComponentPolicy( const value_type & mean)
    : mean_(mean), dimension_(mean_.getDimension()) {}

  static inline value_type GetInvariant(const NumericalSampleImplementation & nsi)
  {
    return value_type(2 * nsi.getDimension(), 0.0);
  }

  inline value_type & inplace_op( value_type & var, NSI_const_point point ) const
  {
    for (UnsignedLong i = 0; i < dimension_; ++i)
      {
	const NumericalScalar val(point[i] - mean_[i]);
	const NumericalScalar val2(val * val);
	var[i] += val2;
	var[i + dimension_] += val2 * val2;
      }
    return var;
  }

  static inline value_type & inplace_op( value_type & var, const value_type & point )
  {
    return var += point;
  }

}; /* end struct KurtosisPerComponentPolicy */

/*
 * Gives the kurtosis of the sample (by component)
 */
NumericalPoint NumericalSampleImplementation::computeKurtosisPerComponent() const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the kurtosis per component of an empty sample.";

  // Special case for a sample of size 1
  if (size_ == 1) return NumericalPoint(dimension_, 0.0);

  const NumericalPoint mean(computeMean());
  const KurtosisPerComponentPolicy policy ( mean );
  ReductionFunctor<KurtosisPerComponentPolicy> functor( *this, policy );
  TBB::ParallelReduce( 0, size_, functor );
  NumericalPoint kurtosis(dimension_);
  const NumericalScalar factor1((size_ + 1.0) * size_ * (size_ - 1.0) / ((size_ - 2.0) * (size_ - 3.0)));
  const NumericalScalar factor2(-3.0 * (3.0 * size_ - 5.0) / ((size_ - 2.0) * (size_ - 3.0)));
  for (UnsignedLong i = 0; i < dimension_; ++i) kurtosis[i] = factor1 * functor.accumulator_[i + dimension_] / (functor.accumulator_[i] * functor.accumulator_[i]) + factor2;
  return kurtosis;
}

struct CenteredMomentPerComponentPolicy
{
  typedef NumericalPoint value_type;

  const value_type & mean_;
  const UnsignedLong k_;
  const UnsignedLong dimension_;

  CenteredMomentPerComponentPolicy( const value_type & mean, const UnsignedLong k)
    : mean_(mean), k_(k), dimension_(mean_.getDimension()) {}

  static inline value_type GetInvariant(const NumericalSampleImplementation & nsi)
  {
    return value_type(nsi.getDimension(), 0.0);
  }

  inline value_type & inplace_op( value_type & var, NSI_const_point point ) const
  {
    for (UnsignedLong i = 0; i < dimension_; ++i)
      {
	const NumericalScalar val(point[i] - mean_[i]);
	var[i] += pow(val, k_);
      }
    return var;
  }

  static inline value_type & inplace_op( value_type & var, const value_type & point )
  {
    return var += point;
  }

}; /* end struct CenteredMomentPerComponentPolicy */

/*
 * Gives the centered moment of order k of the sample (by component)
 */
NumericalPoint NumericalSampleImplementation::computeCenteredMomentPerComponent(const UnsignedLong k) const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the centered moments per component of an empty sample.";

  if (size_ == 0) throw InvalidArgumentException(HERE) << "Cannot compute centered moments on an empty sample";

  // Special case: order 0, return (size,...,size)
  if (k == 0) return NumericalPoint(dimension_, size_);
  // Special case: order 1, return (0,...,0)
  if (k == 1) return NumericalPoint(dimension_, 0.0);
  // Special case: order 2, return biased variance estimator
  if (k == 2) return computeVariancePerComponent() * (size_ - 1.0) / size_;
  // General case
  const NumericalPoint mean(computeMean());

  const CenteredMomentPerComponentPolicy policy ( mean, k );
  ReductionFunctor<CenteredMomentPerComponentPolicy> functor( *this, policy );
  TBB::ParallelReduce( 0, size_, functor );
  return functor.accumulator_ / size_;
}

/*
 * Gives the quantile per component of the sample
 */
NumericalPoint NumericalSampleImplementation::computeQuantilePerComponent(const NumericalScalar prob) const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the quantile per component of an empty sample.";

  // Special case for prob outside of the allowed bounds
  const NumericalScalar scalarIndex(prob * size_ - 0.5);
  if (scalarIndex >= size_ - 1) return getMax();
  // Special case for prob <= 0.0
  if (scalarIndex <= 0.0) return getMin();

  const UnsignedLong index(static_cast<UnsignedLong>( floor( scalarIndex) ));
  const NumericalScalar beta(scalarIndex - index);
  const NumericalScalar alpha(1.0 - beta);
  NumericalPoint quantile(dimension_);
  NumericalPoint component(size_);
  for (UnsignedLong j = 0; j < dimension_; ++j)
    {
      for (UnsignedLong i = 0; i < size_; ++i)
        component[i] = operator[](i)[j];

      TBB::ParallelSort(component.begin(), component.end());

      // Interpolation between the two adjacent empirical quantiles
      quantile[j] = alpha * component[index] + beta * component[index + 1];
    } // end for

  return quantile;
}

/*
 * Gives the N-dimension quantile of the sample
 */
NumericalPoint NumericalSampleImplementation::computeQuantile(const NumericalScalar prob) const
{
  if (size_ == 0) throw InternalException(HERE) << "Error: cannot compute the quantile of an empty sample.";

  if (getDimension() == 1) return computeQuantilePerComponent(prob);
  throw NotYetImplementedException(HERE);
}

struct CDFPolicy
{
  typedef NumericalScalar value_type;

  const NumericalPoint & point_;
  const Bool tail_;
  const UnsignedLong dimension_;

  CDFPolicy( const NumericalSampleImplementation & nsi,
             const NumericalPoint & point,
             const Bool tail )
    : point_(point), tail_(tail), dimension_(nsi.getDimension()) {}

  static inline value_type GetInvariant(const NumericalSampleImplementation &)
  {
    return value_type(0);
  }

  inline value_type & inplace_op( value_type & a,
                                  const value_type & other ) const
  {
    return a += other;
  }

  template <typename T>
  inline value_type & inplace_op( value_type & a,
                                  const T & pt ) const
  {
    UnsignedLong j(0);
    while ( (j < dimension_) && (tail_ ^ (pt[j] <= point_[j])) ) ++j;
    if (j == dimension_) ++a;
    return a;
  }

}; /* end struct CDFPolicy */

/*
 * Get the empirical CDF of the sample
 */
NumericalScalar NumericalSampleImplementation::computeEmpiricalCDF(const NumericalPoint & point,
                                                                   const Bool tail) const
{
  if (size_ == 0) throw InvalidArgumentException(HERE) << "Cannot compute the empirical CDF of an empty sample.";
  if (getDimension() != point.getDimension()) throw InvalidArgumentException(HERE) << "Point has incorrect dimension. Got "
                                                                                   << point.getDimension() << ". Expected " << getDimension();

  const CDFPolicy policy( *this, point, tail );
  ReductionFunctor<CDFPolicy> functor( *this, policy );
  TBB::ParallelReduce( 0, size_, functor );
  return static_cast < NumericalScalar > (functor.accumulator_) / size_;
}

struct MaxPerComponentPolicy
{
  typedef NumericalPoint value_type;

  static inline value_type GetInvariant(const NumericalSampleImplementation & nsi)
  {
    return value_type(nsi.getDimension(), - std::numeric_limits<NumericalScalar>::max());
  }

  template <typename T>
  static inline value_type & inplace_op( value_type & a, const T & b)
  {
    const UnsignedLong dim = a.getDimension();
    for (UnsignedLong j = 0; j < dim; ++j) a[j] = std::max( a[j], b[j] );
    return a;
  }
}; /* end struct MaxPerComponentPolicy */

struct MinPerComponentPolicy
{
  typedef NumericalPoint value_type;

  static inline value_type GetInvariant(const NumericalSampleImplementation & nsi)
  {
    return value_type(nsi.getDimension(), std::numeric_limits<NumericalScalar>::max());
  }

  template <typename T>
  static inline value_type & inplace_op( value_type & a, const T & b)
  {
    const UnsignedLong dim = a.getDimension();
    for (UnsignedLong j = 0; j < dim; ++j) a[j] = std::min( a[j], b[j] );
    return a;
  }
}; /* end struct MinPerComponentPolicy */


/* Maximum accessor */
NumericalPoint NumericalSampleImplementation::getMax() const
{
  if (size_ == 0) throw InternalException(HERE) << "Impossible to get the maximum of an empty NumericalSample";

  ReductionFunctor<MaxPerComponentPolicy> functor( *this, MaxPerComponentPolicy() );
  functor.accumulator_ = operator[](0);
  TBB::ParallelReduce( 1, size_, functor );
  return functor.accumulator_;
}

/* Minimum accessor */
NumericalPoint NumericalSampleImplementation::getMin() const
{
  if (size_ == 0) throw InternalException(HERE) << "Impossible to get the minimum of an empty NumericalSample";

  ReductionFunctor<MinPerComponentPolicy> functor( *this, MinPerComponentPolicy() );
  functor.accumulator_ = operator[](0);
  TBB::ParallelReduce( 1, size_, functor );
  return functor.accumulator_;
}


struct TranslationPolicy
{
  const NumericalPoint & translation_;
  const UnsignedLong dimension_;

  TranslationPolicy( const NumericalPoint & translation)
    : translation_(translation), dimension_(translation_.getDimension()) {}

  inline void inplace_op( NSI_point point ) const
  {
    for (UnsignedLong j = 0; j < dimension_; ++j) point[j] += translation_[j];
  }

}; /* end struct TranslationPolicy */

/*
 * Translate realizations in-place
 */
void NumericalSampleImplementation::translate(const NumericalPoint & translation)
{
  if (dimension_ != translation.getDimension())
    throw InvalidArgumentException(HERE) << "Translation point has incorrect dimension. Got " << translation.getDimension()
                                         << ". Expected " << dimension_;

  if (size_ == 0) return;

  const TranslationPolicy policy( translation );
  ParallelFunctor<TranslationPolicy> functor( *this, policy );
  TBB::ParallelFor( 0, size_, functor );
}

NumericalSampleImplementation & NumericalSampleImplementation::operator += (const NumericalPoint & translation)
{
  translate(translation);
  return *this;
}

NumericalSampleImplementation & NumericalSampleImplementation::operator -= (const NumericalPoint & translation)
{
  translate(translation * (-1.0));
  return *this;
}

/* Get the i-th marginal distribution */
NumericalSampleImplementation NumericalSampleImplementation::getMarginal(const UnsignedLong index) const
{
  if (index >= dimension_) throw InvalidArgumentException(HERE) << "The index of a marginal sample must be in the range [0, dim-1]";

  // Special case for dimension 1
  if (dimension_ == 1) return *this;

  // General case
  NumericalSampleImplementation marginalSample(size_, 1);
  const Description description(getDescription());

  // If the sample has a description, extract the marginal description
  if (description.getSize() == dimension_)
    marginalSample.setDescription(Description(1, getDescription()[index]));

  for (UnsignedLong i = 0; i < size_; ++i)
    marginalSample[i][0] = operator[](i)[index];

  return marginalSample;
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
NumericalSampleImplementation NumericalSampleImplementation::getMarginal(const Indices & indices) const
{
  if (!indices.check(dimension_ - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal sample must be in the range [0, dim-1] and  must be different";

  // Special case for dimension 1
  if (dimension_ == 1) return *this;

  // General case
  const UnsignedLong outputDimension(indices.getSize());
  NumericalSampleImplementation marginalSample(size_, outputDimension);
  const Description description(getDescription());

  // If the sample has a description, extract the marginal description
  if (description.getSize() == dimension_)
    {
      Description marginalDescription(outputDimension);
      for (UnsignedLong i = 0; i < outputDimension; ++i)
        marginalDescription[i] = description[indices[i]];
      marginalSample.setDescription(marginalDescription);
    }

  for (UnsignedLong i = 0; i < size_; ++i)
    {
      for (UnsignedLong j = 0; j < outputDimension; ++j)
        {
          // We access directly to the component of the NumericalPoint for performance reason
          marginalSample[i][j] = operator[](i)[indices[j]];
        }
    }

  return marginalSample;
}


struct ScalingPolicy
{
  const NumericalPoint & scale_;
  const UnsignedLong dimension_;

  ScalingPolicy( const NumericalPoint & scale) : scale_(scale), dimension_(scale_.getDimension()) {}

  inline void inplace_op( NSI_point point ) const
  {
    for (UnsignedLong j = 0; j < dimension_; ++j) point[j] *= scale_[j];
  }

}; /* end struct ScalingPolicy */

/*
 * Scale realizations componentwise in-place
 */
void NumericalSampleImplementation::scale(const NumericalPoint & scaling)
{
  if (dimension_ != scaling.getDimension())
    throw InvalidArgumentException(HERE) << "Scaling point has incorrect dimension. Got " << scaling.getDimension()
                                         << ". Expected " << dimension_;

  if (size_ == 0) return;

  const ScalingPolicy policy( scaling );
  ParallelFunctor<ScalingPolicy> functor( *this, policy );
  TBB::ParallelFor( 0, size_, functor );
}

NumericalSampleImplementation & NumericalSampleImplementation::operator *= (const NumericalPoint & scaling)
{
  scale(scaling);
  return *this;
}

struct MatrixMultiplyPolicy
{
  const SquareMatrix & scale_;
  const UnsignedLong dimension_;

  MatrixMultiplyPolicy( const SquareMatrix & scale) : scale_(scale), dimension_(scale_.getDimension()) {}

  inline void inplace_op( NSI_point point ) const
  {
    point = scale_ * point;
  }

}; /* end struct MatrixMultiplyPolicy */

NumericalSampleImplementation & NumericalSampleImplementation::operator *= (const SquareMatrix & scaling)
{
  if (dimension_ != scaling.getDimension())
    throw InvalidArgumentException(HERE) << "Scaling point has incorrect dimension. Got " << scaling.getDimension()
                                         << ". Expected " << dimension_;

  if (size_ == 0) return *this;

  const MatrixMultiplyPolicy policy( scaling );
  ParallelFunctor<MatrixMultiplyPolicy> functor( *this, policy );
  TBB::ParallelFor( 0, size_, functor );
}

/* Save to CSV file */
void NumericalSampleImplementation::exportToCSVFile(const FileName & filename,
                                                    const String & csvSeparator,
                                                    const Bool withDescription) const
{

  std::ofstream csvFile(filename.c_str());
  if (csvFile.fail())
    throw FileOpenException(HERE) << "Could not open file " << filename;
  csvFile.imbue(std::locale("C"));
  csvFile.precision(16);
  // Export the description
  if (withDescription)
    {
      const Description description(getDescription());
      const UnsignedLong descriptionSize(description.getSize());

      // Write the description if any
      if (descriptionSize > 0)
        {
          String separator;
          for (UnsignedLong i = 0; i < descriptionSize; ++i, separator = csvSeparator)
            csvFile << separator << "\"" << description[i] << "\"";
          csvFile << Os::GetEndOfLine();
        }
    }

  // Write the data
  for(UnsignedLong i = 0; i < size_; ++i, csvFile << Os::GetEndOfLine())
    {
      String separator;
      for(UnsignedLong j = 0; j < dimension_; ++j, separator = csvSeparator)
        csvFile << separator << std::scientific << operator[](i)[j];
    }

  // Close the file
  csvFile.close();
}



/* Method save() stores the object through the StorageManager */
void NumericalSampleImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "size_", size_);
  adv.saveAttribute( "dimension_", dimension_);
  adv.saveAttribute( "data_", data_);
  if (!p_description_.isNull())
    adv.saveAttribute( "description_", *p_description_ );
}


/* Method load() reloads the object from the StorageManager */
void NumericalSampleImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "size_", size_);
  adv.loadAttribute( "dimension_", dimension_);
  adv.loadAttribute( "data_", data_);
  Description description;
  adv.loadAttribute( "description_", description );
  if (description.getSize() != 0) setDescription(description);
}


END_NAMESPACE_OPENTURNS
