#!/bin/sh

PATH=$PATH:/bin

OT_PREFIX="C:\Python27\Lib\site-packages\openturns"
cd $OT_PREFIX\\examples
# add python dir
PATH=$PATH:$PWD/../../../..

FAILED_TEST=""
function check_error {
  if [ "$?" != "0" ]; then
    FAILED_TEST="$FAILED_TEST $1"
  fi
}


echo "./start_check_test.sh"
./start_check_test.sh
check_error "./start_check_test.sh"


echo "./start_installcheck_test.sh"
./start_installcheck_test.sh
check_error "./start_installcheck_test.sh"


echo "./start_pythoninstallcheck_test.sh"
./start_pythoninstallcheck_test.sh
check_error "./start_pythoninstallcheck_test.sh"


echo "test C++ compilation against ot lib"
cd simple_cxx_example
start //B //WAIT make.bat
cd ..
check_error "C++ compilation against ot lib"


echo "test compilation of C wrapper"
cd WrapperTemplates/mingw_wrapper_calling_shell_command 
start //B //WAIT make.bat
start //B //WAIT launch.bat
cd ../..
check_error "test compilation of C wrapper"


RC=0
if [ "$FAILED_TEST" = "" ]; then
  echo "Every check tests are OK."
else
  echo "Some tests in$FAILED_TEST failed!"
  RC=1
fi

echo
echo "Press any key to exit."
read ANYKEY

exit $RC


