# --enable-cmake-plugin=yes/no
AC_ARG_ENABLE([cmake-plugin],
              [AS_HELP_STRING([--enable-cmake-plugin=@<:@yes/no@:>@],
                              [Build with support for the CMake build system])],
              [enable_cmake_plugin=$enableval],
              [enable_cmake_plugin=yes])

# for if ENABLE_CMAKE_PLUGIN in Makefile.am
AM_CONDITIONAL(ENABLE_CMAKE_PLUGIN, test x$enable_cmake_plugin = xyes)

# Ensure our makefile is generated by autoconf
AC_CONFIG_FILES([plugins/cmake/Makefile])
