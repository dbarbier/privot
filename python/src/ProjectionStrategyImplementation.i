// SWIG file ProjectionStrategyImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ProjectionStrategyImplementation.hxx"
%}

%include ProjectionStrategyImplementation.hxx
namespace OT{ %extend ProjectionStrategyImplementation { ProjectionStrategyImplementation(const ProjectionStrategyImplementation & other) { return new OT::ProjectionStrategyImplementation(other); } } }
