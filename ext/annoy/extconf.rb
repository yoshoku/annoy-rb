require 'mkmf'

IS_MSWIN = /mswin/ =~ RUBY_PLATFORM
IS_DARWIN = /darwin/ =~ RUBY_PLATFORM

if IS_DARWIN
  warn 'libc++ is not found.' unless have_library('c++')
end

if !IS_DARWIN && !IS_MSWIN
  warn 'libstdc++ is not found.' unless have_library('stdc++')
end

if IS_MSWIN
  $CXXFLAGS << ' /std:c++17'
else
  $CXXFLAGS << ' -std=c++14'
end

$INCFLAGS << " -I$(srcdir)/src"
$VPATH << "$(srcdir)/src"

create_makefile('annoy/annoyext')
