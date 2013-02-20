//                                               -*- C++ -*-
/**
 *  @file  Tensor.cxx
 *  @brief Tensor implements the classical mathematical tensor
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include "Tensor.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Tensor);

/* Default constructor */
Tensor::Tensor()
  : TypedInterfaceObject<TensorImplementation>(new TensorImplementation())
{
  // Nothing to do
}

/* Constructor with size (rowDim, colDim and sheetDim) */
/* The tensor is made up of a collection of rowDim*colDim*sheetDim elements */
/* The tensor is viewed as a set of column vectors read one after another, one sheet after another */
Tensor::Tensor(const UnsignedLong rowDim,
               const UnsignedLong colDim,
               const UnsignedLong sheetDim)
  : TypedInterfaceObject<TensorImplementation>(new TensorImplementation(rowDim, colDim, sheetDim))
{
  // Nothing to do
}

/* Constructor from external collection */
Tensor::Tensor(const UnsignedLong rowDim,
               const UnsignedLong colDim,
               const UnsignedLong sheetDim,
               const Collection<NumericalScalar> & elementsValues)
  : TypedInterfaceObject<TensorImplementation>(new TensorImplementation(rowDim, colDim, sheetDim, elementsValues))
{
  // Nothing to do
}

/* Constructor with implementation */
Tensor::Tensor(const Implementation & i)
  : TypedInterfaceObject<TensorImplementation>(i)
{
  // Nothing to do
}


/* String converter */
String Tensor::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " implementation=" << getImplementation()->__repr__();
}

/* String converter */
String Tensor::__str__(const String & offset) const
{
  OSS oss;
  const UnsignedLong rows   = getNbRows();
  const UnsignedLong cols   = getNbColumns();
  const UnsignedLong sheets = getNbSheets();
  if ( (rows   >= ResourceMap::GetAsUnsignedLong("Tensor-size-visible-in-str-from")) ||
       (cols   >= ResourceMap::GetAsUnsignedLong("Tensor-size-visible-in-str-from")) ||
       (sheets >= ResourceMap::GetAsUnsignedLong("Tensor-size-visible-in-str-from")) )
    oss << rows << "x" << cols << "x" << sheets << "\n";

  size_t lwidth = 0;
  size_t rwidth = 0;
  for( UnsignedLong k = 0; k < sheets; ++k )
    for( UnsignedLong i = 0; i < rows; ++i )
      for( UnsignedLong j = 0; j < cols; ++j )
        {
          String st = OSS() << (*this)(i, j, k);
          size_t dotpos = st.find( '.' );
          lwidth = std::max( lwidth, (dotpos != String::npos) ? dotpos             : st.size() );
          rwidth = std::max( rwidth, (dotpos != String::npos) ? st.size() - dotpos : 0         );
        }

  const char * nl = "";
  for( UnsignedLong k = 0; k < sheets; ++k, nl = "\n" )
    {
      oss << nl << "sheet #" << k << "\n";
      const char * bracket = "[";
      const char * newline = "";
      for( UnsignedLong i = 0; i < rows; ++i, newline = "\n", bracket = " " )
        {
          oss << newline << offset << bracket << "[ ";
          const char * sep = "";
          for( UnsignedLong j = 0; j < cols; ++j, sep = " " )
            {
              String st = OSS() << (*this)(i, j, k);
              size_t dotpos = st.find( '.' );
              oss << sep << String( lwidth - ((dotpos != String::npos) ? dotpos : st.size()), ' ' )
                  << (*this)(i, j, k)
                  << String( rwidth - ((dotpos != String::npos) ? st.size() - dotpos : 0), ' ' );
            }
          oss << " ]";
        }
      oss << "]";
    }
  return oss;
}

/* Operator () gives access to the elements of the tensor (to modify these elements) */
/* The element of the tensor is designated by its row number i, its column number j and its sheet number k */
NumericalScalar & Tensor::operator () (const UnsignedLong i,
                                       const UnsignedLong j,
                                       const UnsignedLong k)
{
  copyOnWrite();
  return (*getImplementation())(i, j, k);
}

/* Operator () gives access to the elements of the tensor (read only) */
/* The element of the tensor is designated by its row number i, its column number j and its sheet number k */
const NumericalScalar & Tensor::operator () (const UnsignedLong i,
                                             const UnsignedLong j,
                                             const UnsignedLong k) const
{
  return (*getImplementation())(i, j, k);
}


/* getSheet returns the sheet specified by its sheet number k */
Matrix Tensor::getSheet(const UnsignedLong k) const
{
  return getImplementation()->getSheet(k);
}

/* setSheet sets matrix m as the sheet specified by its sheet number k  */
void Tensor::setSheet(const UnsignedLong k,
                      const Matrix & m)
{
  copyOnWrite();
  getImplementation()->setSheet(k, m);
}

/* Get the dimensions of the tensor : number of rows */
const UnsignedLong Tensor::getNbRows() const
{
  return getImplementation()->getNbRows();
}

/* Get the dimensions of the Tensor : number of columns */
const UnsignedLong Tensor::getNbColumns() const
{
  return getImplementation()->getNbColumns();
}

/* Get the dimensions of the Tensor : number of sheets */
const UnsignedLong Tensor::getNbSheets() const
{
  return getImplementation()->getNbSheets();
}

/* Empty returns true if there is no element in the Tensor */
const Bool Tensor::isEmpty() const
{
  return getImplementation()->isEmpty();
}

/* Comparison operator */
Bool Tensor::operator == (const Tensor & rhs) const
{
  const Tensor &lhs(*this);
  return (*(lhs.getImplementation()) == *(rhs.getImplementation()) );
}

const NumericalScalar* Tensor::__baseaddress__() const
{
  return getImplementation()->__baseaddress__();
}

UnsignedLong Tensor::__elementsize__() const
{
  return getImplementation()->__elementsize__();
}

UnsignedLong Tensor::__stride__(UnsignedLong dim) const
{
  return getImplementation()->__stride__(dim);
}


END_NAMESPACE_OPENTURNS
