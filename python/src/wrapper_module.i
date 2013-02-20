// SWIG file wrapper_module.i
// @author schueller
// @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)

%module(package="openturns", docstring="Wrapping utilities.") wrapper
#pragma SWIG nowarn=509
%feature("autodoc","1");

%pythoncode %{
def makeVariableList( vars ):
  """
  Transform the vars list into a WrapperVariableList structure
  ex : vars = ({ "id_" : "F", "type_" : WrapperDataVariableType.IN  },
                    { "id_" : "E", "type_" : WrapperDataVariableType.IN  },
                    { "id_" : "Z", "type_" : WrapperDataVariableType.OUT },
                    )
            varlist = makeVariableList( vars )

        Note: The keys of the dictionary are the field of the WrapperDataVariable structure.
  """
  vl = WrapperDataVariableCollection()
  for var in vars:
    if var.__class__ == dict().__class__ :
      v = WrapperDataVariable()
      for key in var.keys():
        v.__setattr__( key, var[key] )
      vl.add( v )
  return vl

def makeFileList( files ):
  """
  Transform the files list into a WrapperFileList structure
  ex : files = ({ "id_" : "infile",  "type_" : WrapperDataFileType.IN,
                        "path_" : "/tmp/inFILE",  "subst_" : "E,F"
                      },
                      { "id_" : "outfile", "type_" : WrapperDataFileType.OUT,
                        "path_" : "/tmp/outFILE", "subst_" : "Z"
                      },
                      )
              filelist = makeFileList( files )

        Note: The keys of the dictionary are the field of the WrapperDataFile structure.
  """
  fl = WrapperDataFileCollection()
  for file in files:
    if file.__class__ == dict().__class__ :
      f = WrapperDataFile()
      for key in file.keys():
        f.__setattr__( key, file[key] )
      fl.add( f )
  return fl

def makeParameters( params ):
  """
  Transform the params dictionnary into a WrapperParameter structure
  ex : params = { "command_" : "/path/to/my/code < someredir > anotherredir",
                        "userPrefix_" : "MyCode",
                      }
            parameters = makeParameters( params )

        Note: The keys of the dictionary are the field of the WrapperParameter structure.
  """
  p = WrapperParameter()
  for key in params.keys():
    p.__setattr__( key, params[key] )
  return p

def makeFramework( fw ):
  """
  Transform the fw dictionnary into a WrapperFramework structure
  ex : fw = { "studyid_" : 1733,
                    "studycase_" : "SalomeOpenTURNS_testcase",
                    "componentname_" : "SalomeOpenTURNSComponent"
                  }
            framework = makeFramework( fw )

        Note: The keys of the dictionary are the field of the WrapperFrameworkData structure.
  """
  f = WrapperFrameworkData()
  for key in fw.keys():
    f.__setattr__( key, fw[key] )
  return f

def makeWrapper( vars, files, params, framework ):
  """
  Build a new WrapperFile object from the data provided by the "generic" wrapper
  and replace them with the data from vars, files, params and framework.
  See docstring for makeVariableList(), makeFileList(), makeParameters() and
  makeFramework() functions to get the actual structure of these arguments.
  """
  wrp = WrapperFile.FindWrapperByName( "generic" )
  wd = wrp.getWrapperData()
  vl = makeVariableList( vars )
  wd.setVariableList( vl )
  fl = makeFileList( files )
  wd.setFileList( fl )
  wp = makeParameters( params )
  wd.setParameters( wp )
  fw = makeFramework( framework )
  wd.setFrameworkData( fw )
  wrp.setWrapperData( wd )
  return wrp
%}

%{
#include "OTconfig.hxx"
#include "OTCommon.hxx"
#include "Collection.hxx"
#include "WrapperData.hxx"
#include "WrapperFile.hxx"
%}

%include typemaps.i
%include OTtypes.i
%include OTexceptions.i
%include std_vector.i

/* Base/Common */
%import common_module.i

/* Base/Type */
%include Collection.i

/* Base/Func */
%include WrapperData.i
%include WrapperFile.i

/* At last we include template definitions */
%include BaseWrapperTemplateDefs.i
