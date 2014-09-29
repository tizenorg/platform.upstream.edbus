Name:           edbus
Version:        1.7.8
Release:        0
License:        BSD-2-Clause
Summary:        EFL Wrapper for DBus
Url:            http://www.enlightenment.org/
Group:          Graphics & UI Framework/EFL
Source0:        e_dbus-%{version}.tar.bz2
Source1001:     edbus.manifest
BuildRequires:  doxygen
BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(ecore)
BuildRequires:  pkgconfig(eina)
BuildRequires:  pkgconfig(evas)

%description
e_dbus provides a convenience wrapper for EFL applications using DBus.

%package devel
Summary:        Development components for the e_dbus package
Group:          Development/Libraries
Requires:       %{name} = %{version}

%description devel
Development files for e_dbus

%prep
%setup -q -n e_dbus-%{version}
cp %{SOURCE1001} .

%build
%reconfigure --disable-static
%__make %{?_smp_mflags}

%install
%make_install

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%license COPYING
%{_bindir}/e_dbus_*
%{_bindir}/e-notify-send
%{_libdir}/libe*.so.*
%{_datadir}/e_dbus/logo.png

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/e_dbus-1/*.h
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc
%{_includedir}/e_dbus-1/connman0_7x/E_Connman.h
%{_bindir}/e-notify-send
