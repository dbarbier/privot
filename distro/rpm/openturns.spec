# norootforbuild
%if ! (0%{?fedora} > 12 || 0%{?rhel} > 5)
%{!?python_sitearch: %global python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(1))")}
%endif

%define __cmake %{_bindir}/cmake
%define _cmake_lib_suffix64 -DLIB_SUFFIX=64
%define cmake \
CFLAGS="${CFLAGS:-%optflags}" ; export CFLAGS ; \
CXXFLAGS="${CXXFLAGS:-%optflags}" ; export CXXFLAGS ; \
FFLAGS="${FFLAGS:-%optflags}" ; export FFLAGS ; \
%__cmake \\\
-DCMAKE_INSTALL_PREFIX:PATH=%{_prefix} \\\
%if "%{?_lib}" == "lib64" \
%{?_cmake_lib_suffix64} \\\
%endif \
-DBUILD_SHARED_LIBS:BOOL=ON

Name:           openturns 
Version:        1.1
Release:        1%{?dist}
Summary:        Uncertainty treatment library
Group:          System Environment/Libraries
License:        LGPLv2 
URL:            http://www.openturns.org
Source0:        http://downloads.sourceforge.net/openturns/openturns/openturns-1.1.tar.bz2
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:  gcc-c++, bison, flex
BuildRequires:  cmake >= 2.8
BuildRequires:  libxml2-devel
%if 0%{?suse_version}
BuildRequires:  lapack
BuildRequires:  gcc-fortran
BuildRequires:  ghostscript-library
BuildRequires:  -post-build-checks
BuildRequires:  fdupes
%else
BuildRequires:  lapack-devel
BuildRequires:  gcc-gfortran
%endif
%if 0%{?rhel_version} || 0%{?centos_version}
BuildRequires:  blas-devel
%endif
BuildRequires:  python-devel
BuildRequires:  swig >= 1.3.35
%if 0%{?fedora}
# choices: atlas / atlas-sse2
BuildRequires:  atlas
%endif

%description
OpenTURNS Uncertainty treatment library

%package libs
Summary:        Uncertainty treatment library
Group:          Development/Libraries/C and C++
%if 0%{?mdkversion}
%ifarch x86_64
Requires:       lib64lapack3
%else
Requires:       liblapack3
%endif
%else
Requires:       lapack
%endif

%description libs
Uncertainty treatment library binaries 

%package devel
Summary:        Open TURNS development files
Group:          Development/Libraries/C and C++
Requires:       %{name}-libs = %{version}
Requires:       libxml2-devel
%if 0%{?suse_version}
%else
Requires:       lapack-devel
%endif
%if 0%{?rhel_version} || 0%{?centos_version}
BuildRequires:  blas-devel
%endif

%description devel
Development files for OpenTURNS uncertainty library

%package examples
Summary:        Open TURNS examples
Group:          Productivity/Scientific/Math

%description examples
OpenTURNS python examples

%package validation
BuildArch:      noarch
Summary:        Open TURNS validation files
Group:          Productivity/Scientific/Math

%description validation
OpenTURNS validation text files

%package -n python-%{name}
Summary:        Uncertainty treatment library
Group:          Productivity/Scientific/Math
%if 0%{?suse_version}
Requires:       python-base
%else
Requires:       python
%endif
%if 0%{?fedora}
Requires:       atlas
%endif

%description -n python-%{name}
Python textual interface to OpenTURNS uncertainty library

%package -n python-%{name}-devel
BuildArch:      noarch
Summary:        Uncertainty treatment library
Group:          Development/Libraries/C and C++
Requires:       python-%{name} = %{version}
Requires:       %{name}-devel = %{version}
Requires:       python-devel

%description -n python-%{name}-devel
Python textual interface to OpenTURNS uncertainty library development

%prep
%setup -q

%build
%cmake . -DOPENTURNS_SYSTEM_INSTALL=ON \
         -DOPENTURNS_SYSCONFIG_PATH=/etc \
         -DCMAKE_C_FLAGS_RELEASE="%optflags -fno-cse-follow-jumps"
make %{?_smp_mflags} OT
make

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}
rm -r %{buildroot}%{_datadir}/%{name}/doc
cd %{buildroot}%{_datadir}/%{name}/examples
ls | grep -v '\.cxx$' | grep -v '\.py$' | xargs rm -r
mkdir -p %{buildroot}%{_sysconfdir}/ld.so.conf.d
echo %{_libdir}/openturns > %{buildroot}%{_sysconfdir}/ld.so.conf.d/openturns.conf
%if 0%{?suse_version}
  %fdupes %{buildroot} 
%endif

%check
LD_LIBRARY_PATH=%{buildroot}%{_libdir}/%{name} make check %{?_smp_mflags} || cat lib/test/Testing/Temporary/LastTest.log
LD_LIBRARY_PATH=%{buildroot}%{_libdir}/%{name} PYTHONPATH=%{buildroot}%{python_sitearch} %{__python} -B -c "import openturns"

%clean
rm -rf %{buildroot}

%post   libs -p /sbin/ldconfig
%postun libs -p /sbin/ldconfig 

%files libs
%defattr(-,root,root,-)
%doc AUTHORS COPYING README
%dir %{_sysconfdir}/%{name}
%config %{_sysconfdir}/%{name}/%{name}.conf
%config %{_sysconfdir}/ld.so.conf.d/openturns.conf
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/*.so.*
%dir %{_datadir}/%{name}
%dir %{_libdir}/%{name}/wrappers
%{_libdir}/%{name}/wrappers/wrapper*.dtd
%{_libdir}/%{name}/wrappers/generic.*

%files devel
%defattr(-,root,root,-)
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/*.h*
%{_libdir}/%{name}/*.so
%{_libdir}/%{name}/*.cmake
%{_bindir}/%{name}-config
%{_bindir}/%{name}-module
%{_datadir}/%{name}/WrapperTemplates/

%files examples
%defattr(-,root,root,-)
%{_datadir}/%{name}/examples/
%{_libdir}/%{name}/wrappers/external_code_threads*
%{_libdir}/%{name}/wrappers/minimal_wrapper*
%{_libdir}/%{name}/wrappers/poutre*
%{_libdir}/%{name}/wrappers/testwrapper*
%{_libdir}/%{name}/wrappers/Test*
%{_libdir}/%{name}/wrappers/wrapper.xml

%files validation
%defattr(-,root,root,-)
%{_datadir}/%{name}/validation/

%files -n python-%{name}
%defattr(-,root,root,-)
%{python_sitearch}/%{name}/

%files -n python-%{name}-devel
%defattr(-,root,root,-)
%{_includedir}/%{name}/swig/

%changelog
* Wed Nov 28 2012 Julien Schueller <schueller at phimeca dot com> 1.1-1
- New upstream release

* Sat Feb 18 2012 Julien Schueller <schueller at phimeca dot com> 1.0-1
- New upstream release

* Sat Jul 30 2011 Julien Schueller <schueller at phimeca dot com> 0.15-1
- New upstream release

* Sat Apr 9 2011 Julien Schueller <schueller at phimeca dot com> 0.14.0-1
- New upstream release

* Sat Oct 9 2010 Julien Schueller <schueller at phimeca dot com> 0.13.2-1
- New upstream release

* Mon Nov 26 2007 Remy Pagniez 0.11.1-1
- Initial package creation
