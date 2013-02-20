// SWIG file LinearModel.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearModel.hxx"
%}

%rename(LinearModel_operator___eq__) OT::operator ==(const LinearModel & lhs, const LinearModel & rhs);
%template(ConfidenceIntervalCollection) OT::Collection<OT::ConfidenceInterval>;

%include LinearModel.hxx
namespace OT{ %extend LinearModel { LinearModel(const LinearModel & other) { return new OT::LinearModel(other); } } }
