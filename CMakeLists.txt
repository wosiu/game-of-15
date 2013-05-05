set (SOURCES
	main.cpp  
	mainwindow.cpp
	mysquare.cpp  
	plansza.cpp
	)

set (HEADERS
	mainwindow.h  
	mysquare.h  
	plansza.h
	)

set (FORMS
	mainwindow.ui
	)

qt4_wrap_cpp (HEADERS_MOC ${HEADERS})
qt4_wrap_ui (FORMS_HEADERS ${FORMS})

add_executable (Przesuwanka ${SOURCES} ${HEADERS_MOC} ${FORMS_HEADERS} ${RESOURCES_RCC})
target_link_libraries (Przesuwanka ${QT_LIBRARIES})
include_directories (${CMAKE_CURRENT_BINARY_DIR})
