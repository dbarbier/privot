// SWIG file ProjectionStrategy.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ProjectionStrategy.hxx"
%}

OTTypedInterfaceObjectHelper(ProjectionStrategy)

%include ProjectionStrategy.hxx
namespace OT{ %extend ProjectionStrategy { ProjectionStrategy(const ProjectionStrategy & other) { return new OT::ProjectionStrategy(other); } } }
