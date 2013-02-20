// SWIG file EventProcess.i
// @author dutka
// @date   2008-05-21 11:21:38 +0200 (mer. 21 mai 2008)

%{
#include "EventProcess.hxx"
%}

%include EventProcess.hxx
namespace OT { %extend EventProcess { EventProcess(const EventProcess & other) { return new OT::EventProcess(other); } } }
