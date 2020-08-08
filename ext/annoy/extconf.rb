require 'mkmf'

abort 'libstdc++ is not found.' unless have_library('stdc++')

$CXXFLAGS << " -march=native"
$INCFLAGS << " -I$(srcdir)/src"
$VPATH << "$(srcdir)/src"

create_makefile('annoy/annoy')
