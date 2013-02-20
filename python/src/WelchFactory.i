// SWIG file WelchFactory.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "WelchFactory.hxx"
%}

%include WelchFactory.hxx
namespace OT { 

  %extend WelchFactory {

     WelchFactory(const WelchFactory & other)
       {
         return new OT::WelchFactory(other);
       }

   }
 }
