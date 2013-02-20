// SWIG file NonStationaryCovarianceModelFactory.i
// Author : $LastChangedBy$
// Date : $LastChangedDate$
// Id : $Id$

%{
#include "NonStationaryCovarianceModelFactory.hxx"
%}

%include NonStationaryCovarianceModelFactory.hxx
namespace OT { 

  %extend NonStationaryCovarianceModelFactory {

     NonStationaryCovarianceModelFactory(const NonStationaryCovarianceModelFactory & other)
       {
         return new OT::NonStationaryCovarianceModelFactory(other);
       }

   }
 }
