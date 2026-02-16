require 'mkmf'

if /mswin/ =~ RUBY_PLATFORM
  $CXXFLAGS << ' /std:c++17'
else
  $CXXFLAGS << ' -std=c++14'
end

$INCFLAGS << " -I$(srcdir)/src"
$VPATH << "$(srcdir)/src"

create_makefile('annoy/annoyext')
