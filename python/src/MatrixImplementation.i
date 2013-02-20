// SWIG file MatrixImplementation.i
// @author schueller
// @date   2012-02-15 18:09:50 +0100 (Wed, 15 Feb 2012)

%{
#include "MatrixImplementation.hxx"
%}

%rename(MatrixImplementation_operator___eq__) OT::operator ==(const MatrixImplementation & lhs, const MatrixImplementation & rhs);

%include MatrixImplementation.hxx
namespace OT{ %extend MatrixImplementation { MatrixImplementation(const MatrixImplementation & other) { return new OT::MatrixImplementation(other); } } }
