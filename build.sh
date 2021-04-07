#!/bin/bash
mkdir conan_cache
SCRIPT=`realpath $0`
SCRIPT=`dirname $SCRIPT`
export CONAN_USER_HOME=${SCRIPT}/conan_cache
cd build && conan install -e BUILD_SHARED_LIBS=ON .. && cmake .. && cmake --build . 
STATUSCODE=$?

cd ..

exit ${STATUSCODE}