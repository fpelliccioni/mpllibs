# Copyright Abel Sinkovics (abel@sinkovics.hu)  2012.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  subdirs(src test example)
else()
  subdirs(src test example doc)
endif()

