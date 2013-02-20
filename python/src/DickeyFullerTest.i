// SWIG file DickeyFullerTest.i
// Author : $LastChangedBy: schueller $
// Date : $LastChangedDate: 2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012) $
// Id : $Id: DickeyFullerTest.i 2287 2012-01-02 10:44:01Z schueller $

%{
#include "DickeyFullerTest.hxx"
%}

%include DickeyFullerTest.hxx
namespace OT { %extend DickeyFullerTest { DickeyFullerTest(const DickeyFullerTest & other) { return new OT::DickeyFullerTest(other); } } }
