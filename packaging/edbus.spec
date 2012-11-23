Name:           edbus
Version:        1.7.2
Release:        1
License:        BSD 2-clause
Summary:        EFL Wrapper for DBus
Url:            http://www.enlightenment.org/
Group:          System/Libraries
Source0:        e_dbus-%{version}.tar.bz2
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

%build
%configure --disable-static
make %{?_smp_mflags}

%install
%make_install




%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig





%files
%defattr(-,root,root,-)
%doc COPYING
%{_bindir}/e_dbus_*
/usr/bin/e-notify-send
%{_libdir}/libe*.so.*
/usr/share/e_dbus/logo.png

%files devel
%defattr(-,root,root,-)
%{_includedir}/e_dbus-1/*.h
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc
%{_includedir}/e_dbus-1/connman0_7x/E_Connman.h
/usr/bin/e-notify-send


%changelog
