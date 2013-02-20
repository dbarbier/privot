#
# Compile and create OpenTURNS installer for windows
#
# Rules dependencies make things done in the right order, however it does not
# detect if OpenTURNS sources has changed. This has to be specified manually:
# - to recompute a rules, remove the corresponding file
#   i.e. some OpenTURNS .cxx have changed:
#     rm ot-compile; make
# - to recompute every rules: make clean; make
#

###############################################################################
# adapt these following lines to your configuration :

# OpenTurns paths
OT_SRC    ?= $(PWD)/../..
OT_BUILD  ?= $(OT_SRC)/mingw_build
OT_PREFIX ?= $(OT_BUILD)/install

# openturns-config not present until installed..
#OT_VERSION ?= $(shell $(OT_PREFIX)/bin/openturns-config --version)
OT_VERSION ?= $(shell cat ../../VERSION)

WINDEPS = $(PWD)/openturns-developers-windeps-$(OT_VERSION)

# compiler prefix
TARGET_MACHINE = i386-mingw32

ifneq ($(shell cmake --version),cmake version 2.8.0)
  SYSTEM_NAME = Windows-GNU-CXX
else
  SYSTEM_NAME = Windows-g++
endif

WINE_PREFIX = $(WINDEPS)/wine
export WINEPREFIX = $(WINE_PREFIX)

MINGW_PREFIX = $(WINDEPS)/mingw-3.4.5
# OpenTURNS dependencies
PYTHON_VERSION = 27
PYTHON_PREFIX = $(WINE_PREFIX)/drive_c/Python$(PYTHON_VERSION)
R_PATH = $(WINE_PREFIX)/drive_c/R/R-2.12.0
PATH := $(R_PATH)/bin:$(PATH)
REGEX_PREFIX = $(MINGW_PREFIX)/opt/regex
LIBXML2_PREFIX = $(MINGW_PREFIX)/opt/libxml2

# launch OpenTURNS checktests before creating the installer (y,n)
CHECK_OT = y

# parallel compilation (y,n)
DISTCC = n

# verbose (ON/OFF)
VERBOSE = ON

ifeq ($(DISTCC),y)
  # hosts that will be used for library compilation
  JOBS ?= 5
  DISTCC_LIBS_HOSTS ?= "localhost/5"
  # hosts that will be used for python wrapper compilation (needs lots of ram)
  JOBS_PYTHON ?= 2
  DISTCC_WRAPPERS_HOSTS ?= "localhost/2"
  # user define distcc hosts
  include distcc-hosts.mk

  # mingw compiler available on every distcc nodes
  export PATH := $(MINGW_PREFIX)/bin:$(PATH)
  CC=$(MINGW_PREFIX)/bin/$(TARGET_MACHINE)-gcc
  CPP=$(MINGW_PREFIX)/bin/$(TARGET_MACHINE)-g++
  #CXX=$(MINGW_PREFIX)/bin/$(TARGET_MACHINE)-g++
  LD=$(MINGW_PREFIX)/bin/$(TARGET_MACHINE)-ld
else
  JOBS=$(shell grep -c processor /proc/cpuinfo)
  JOBS_PYTHON=$(JOBS)
  export PATH := $(MINGW_PREFIX)/bin:$(PATH)
  CC=$(TARGET_MACHINE)-gcc
  CPP=$(TARGET_MACHINE)-g++
  LD=$(TARGET_MACHINE)-ld
endif

DEBUG_OT=n
ifeq ($(DEBUG_OT),n)
  DEBUG_FLAGS=--enable-debug=none CXXFLAGS=-O2 CFLAGS="-O2 -fno-cse-follow-jumps" FFLAGS=-O2
else
  DEBUG_FLAGS=--enable-debug CXXFLAGS=-ggdb CFLAGS=-ggdb FFLAGS=-ggdb
endif

###############################################################################
# should not be modified:

export PYTHON=$(PYTHON_PREFIX)/python.exe
export PYTHON_NOVERSIONCHECK=1
export PYTHON_CPPFLAGS=-I$(PYTHON_PREFIX)/include
export PYTHON_LDFLAGS=-L$(PYTHON_PREFIX)/libs -lpython$(PYTHON_VERSION)
export PYTHON_SITE_PKG=$(PYTHON_PREFIX)/Lib/site-packages
# if PYTHON_EXTRA_LIBS is not set to " ", autotools set PYTHON_EXTRA_LIBS to "None" ...
nullstring :=
export PYTHON_EXTRA_LIBS := $(nullstring) # end of the line

BUILD_MACHINE=$(shell gcc -dumpmachine)


###############################################################################
# rules:

.PHONY: all clean-python clean mrproper


all: ot-installer


ot-check-compiler:
	@echo -n "Check OpenTURNS compilation dependencies: " 
	@ls $(OT_SRC) $(PYTHON_PREFIX) $(PYTHON) $(R_PATH) $(REGEX_PREFIX) \
	  $(LIBXML2_PREFIX) $(PYTHON_SITE_PKG) > /dev/null
	@echo "ok"
	@echo -n "Check mingw-g++ compiler: "
	@$(CPP) -v > /dev/null 2>&1
	@echo "ok"
	touch ot-check-compiler


ot-config: ot-check-compiler
	-mkdir -p $(OT_BUILD)
	cd $(OT_BUILD) && rm -f CMakeCache.txt && cmake .. \
	  -DCMAKE_SYSTEM_NAME=$(SYSTEM_NAME) \
	  -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
	  -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
	  -DCMAKE_C_COMPILER=$(MINGW_PREFIX)/bin/$(TARGET_MACHINE)-gcc \
	  -DCMAKE_CXX_COMPILER=$(MINGW_PREFIX)/bin/$(TARGET_MACHINE)-g++ \
	  -DCMAKE_Fortran_COMPILER=$(MINGW_PREFIX)/bin/$(TARGET_MACHINE)-g77 \
	  -DCMAKE_VERBOSE_MAKEFILE=$(VERBOSE) \
	  -DFortranCInterface_EXE=$(OT_BUILD)/CMakeFiles/FortranCInterface/FortranCInterface.exe \
	  -DBLAS_blas_LIBRARY=$(MINGW_PREFIX)/opt/blaslapack/blas.dll \
	  -DLAPACK_lapack_LIBRARY=$(MINGW_PREFIX)/opt/blaslapack/lapack.dll \
	  -DLIBDL_INCLUDE_DIR=$(MINGW_PREFIX)/opt/dlfcn/include \
	  -DLIBDL_LIBRARIES=$(MINGW_PREFIX)/opt/dlfcn/lib/libdl.dll.a \
	  -DLIBXML2_INCLUDE_DIR=$(LIBXML2_PREFIX)/include/libxml2 \
	  -DLIBXML2_LIBRARIES=$(LIBXML2_PREFIX)/lib/libxml2.dll.a \
	  -DREGEX_INCLUDE_DIR=$(REGEX_PREFIX)/include \
	  -DREGEX_LIBRARIES=$(REGEX_PREFIX)/lib/libgnurx.dll.a \
	  -DPYTHON_EXECUTABLE=$(PYTHON_PREFIX)/python.exe \
	  -DPYTHON_INCLUDE_DIR=$(PYTHON_PREFIX)/include \
	  -DPYTHON_LIBRARY=$(PYTHON_PREFIX)/libs/libpython$(PYTHON_VERSION).a \
	  -DR_EXECUTABLE=$(R_PATH)/bin/R.exe \
	  -DR_LIBRARIES=$(R_PATH)/bin/i386/R.dll \
	  -DCMAKE_CXX_FLAGS_RELEASE="-DNDEBUG -O2" \
	  -DCMAKE_C_FLAGS_RELEASE="-DNDEBUG -O2" \
	  -DCMAKE_Fortran_FLAGS_RELEASE="-DNDEBUG -O2" \
	  -DCMAKE_INSTALL_PREFIX=$(OT_PREFIX)
	touch ot-config

ifeq ($(DISTCC),n)
ot-compile: ot-config
	cd $(OT_BUILD)/lib && make -j$(JOBS)
	cd $(OT_BUILD) && make -j$(JOBS_PYTHON)
	-beep
	touch ot-compile
else
ot-compile: ot-config
	cd $(OT_BUILD)/lib && make -j$(JOBS) CXX="distcc $(CPP)" DISTCC_HOSTS=$(DISTCC_LIBS_HOSTS)
	cd $(OT_BUILD) && make -j$(JOBS_PYTHON) CXX="distcc $(CPP)" DISTCC_HOSTS=$(DISTCC_WRAPPERS_HOSTS)
	-beep
	touch ot-compile
endif


warmup-wine = echo warmup-wine; wine ping -n 5&


ot-check: ot-compile ot-install
ifeq ($(CHECK_OT),y)
	$(call warmup-wine)
	cd $(OT_BUILD) && make check -j$(JOBS)
endif
	touch ot-check


ot-install: ot-compile
	cd $(OT_BUILD) && make install -j$(JOBS) 
	-beep -r 2
	touch ot-install


ot-cppinstallcheck: ot-install
ifeq ($(CHECK_OT),y)
	$(call warmup-wine)
	cd $(OT_BUILD) && PATH=$(PATH):$(PYTHON_PREFIX) make cppinstallcheck -j$(JOBS) VERBOSE=1
	touch ot-cppinstallcheck
endif

ot-pyinstallcheck: ot-install
ifeq ($(CHECK_OT),y)
	$(call warmup-wine)
	cd $(OT_BUILD) && PATH=$(PATH):$(PYTHON_PREFIX) make pyinstallcheck -j$(JOBS) VERBOSE=1
	touch ot-pyinstallcheck
endif

ot-installcheck: ot-cppinstallcheck ot-pyinstallcheck

ot-strip: ot-install
ifeq ($(DEBUG_OT),n)
	$(TARGET_MACHINE)-strip  \
	  $(shell find $(OT_PREFIX) -name "*.a") \
	  $(shell find $(OT_PREFIX) -name "*.dll") \
	  $(shell find $(OT_PREFIX) -name "*.pyd") \
	  $(shell find $(OT_PREFIX) -name "*.exe")
endif
	touch ot-strip


ot-check-installer: ot-install
	@echo -n "Check OpenTURNS prefix: "
	@ls $(OT_PREFIX) > /dev/null
	@echo "ok"
	@echo -n "Check OpenTURNS windeps directory: "
	@ls $(WINDEPS)/bin/pthreadGC2.dll > /dev/null 2>&1
	@echo "ok"
	@echo -n "Check nsis compiler: "
	@makensis -VERSION > /dev/null 2>&1
	@echo "ok"
	@echo "OpenTURNS installer dependencies seems all right."
	touch ot-check-installer


ot-user-installer: ot-strip ot-check ot-check-installer ot-cppinstallcheck ot-pyinstallcheck ot-check-installer
	makensis -DOPENTURNS_PREFIX=$(OT_PREFIX) -DPRODUCT_VERSION=$(OT_VERSION) openturns_script.nsi
	touch ot-user-installer

ot-module-installer: ot-strip ot-check ot-check-installer ot-cppinstallcheck ot-pyinstallcheck ot-check-installer
	makensis -DOPENTURNS_PREFIX=$(OT_PREFIX) -DPRODUCT_VERSION=$(OT_VERSION) openturns.nsi
	touch ot-module-installer


ot-dev-installer: ot-strip ot-check ot-check-installer
	makensis -DOPENTURNS_PREFIX=$(OT_PREFIX) -DPRODUCT_VERSION=$(OT_VERSION) openturns_developer.nsi
	touch ot-dev-installer


ot-installer: ot-module-installer ot-dev-installer
	#ot-user-installer
	-beep -r 3



clean-python:
	cd $(OT_BUILD)/python && make clean

clean:
	# Clean rules does not clean OpenTURNS build directory.
	# If you need to do it, do it manually.
	-rm -f ot-*
	-rm -f $(OT_BUILD)/python/src/*.c??


mrproper: clean
	-rm -rf $(OT_PREFIX) $(OT_BUILD)
	-rm -f *.exe


