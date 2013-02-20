#! /usr/bin/env python

import os
import sys
from openturns import *

try:
    #Log.Show( Log.ALL )

    # Template input file
    f = open('inFILE_generic','w')
    f.write('F=10.\nE=5.\n')
    f.close()

    # Variables
    vars = ({ "id_" : "F",
              "type_" : "in",
              "regexp_" : "^\S*F\S*=\S*\R\S*$",
              "format_" : "F=%20.13G"  },

            { "id_" : "E",
              "type_" : "in",
              "regexp_" : "^\S*E\S*=\S*\R\S*$",
              "format_" : "E=%20.13G"  },

            { "id_" : "Z",
              "type_" : "out",
              "regexp_" : "^\S*Z\S*=\S*(\R)\S*$" },
            )

    # Files
    files = ({ "id_" : "infile",
              "type_" : "in",
              "path_" : "inFILE_generic",
              "subst_" : "E,F"  },

            { "id_" : "outfile",
              "type_" : "out",
              "path_" : "outFILE_generic",
              "subst_" : "Z"    },
            )


    # Parameters
    params = {
        "command_" : sys.executable + " -c \"exec(compile(open('inFILE_generic').read(), 'inFILE_generic', 'exec')); Z=E*F; print('Z=' + str(Z))\" > outFILE_generic",
        "userPrefix_" : "GenericWrapperTest",
    }

    # Framework
    framework = { "studyid_" : 1,
                  "studycase_" : "SalOT",
                  "componentname_" : "Beurk"
                  }

    wrp = makeWrapper( vars, files, params, framework )
    #print wrp
    #wrp.writeFile( "wrp.xml" )

    model = NumericalMathFunction( wrp )

    inP = NumericalPoint( (101,255) )
    #print inP
    
    outP = model( inP )
    print outP

    # cleanup
    os.remove('inFILE_generic')

    # check these constants are defined
    print WrapperDataVariableType.IN, WrapperDataVariableType.OUT
    print WrapperDataFileType.IN, WrapperDataFileType.OUT

except :
    import sys, traceback
    traceback.print_exc()
