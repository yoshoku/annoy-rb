require 'mkmf'

abort 'libstdc++ is not found.' unless have_library('stdc++')

$CXXFLAGS << " -std=c++14"
$INCFLAGS << " -I$(srcdir)/src"
$VPATH << "$(srcdir)/src"

create_makefile('annoy/annoyext')
