SET(CPP_FILES 
	node.cpp
	node_differentiate.cpp
	node_reduction.cpp
	node_prettyprinter.cpp
)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(libsym_equation)

ADD_LIBRARY(sym_equation SHARED ${CPP_FILES})

ADD_EXECUTABLE(sample sample.cpp)

target_link_libraries(sample sym_equation) 


#INSTALL(TARGETS sym_equation LIBRARY DESTINATION lib)
