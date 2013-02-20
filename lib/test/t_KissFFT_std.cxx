//                                               -*- C++ -*-
/**
 *  @file  t_KissFFT_std.cxx
 *  @brief The test file of class KissFFT
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

typedef Collection<NumericalComplex> NumericalComplexCollection;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);
  setRandomGenerator();

  try
    {

      // for fft, the best implementation is given for N = 2^p
      const UnsignedLong size(16);

      // collection for test
      NumericalComplexCollection collection(size);

      // Fill the data with artificial values

      // Create a complex gaussian sample
      for (UnsignedLong index = 0; index < size; ++index)
        {
          const NumericalScalar realPart( 0.1 * (1.0 + index) / size );
          const NumericalScalar imagPart( 0.3 * (1.0 + index) / size );
          collection[index] = NumericalComplex(realPart, imagPart);
        }

      /* Instanciation of FFT class */
      KissFFT myFFT;
      fullprint << "myFFT = " << myFFT << std::endl;

      // Initial transformation
      fullprint << "collection = " << collection << std::endl;

      // FFT transform
      const NumericalComplexCollection transformedCollection(myFFT.transform(collection));
      fullprint << "FFT result = " << transformedCollection << std::endl;

      // Inverse transformation
      const NumericalComplexCollection inverseTransformedCollection(myFFT.inverseTransform(transformedCollection));
      fullprint << "FFT back=" << inverseTransformedCollection << std::endl;


    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
