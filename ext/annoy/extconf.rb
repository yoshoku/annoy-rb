require 'mkmf'

abort 'Missing stdc++' unless have_library('stdc++')
# $CXXFLAGS += " -std=c++14"
$INCFLAGS << " -I$(srcdir)/src"
$VPATH << "$(srcdir)/src"

create_makefile('annoy/annoy')
