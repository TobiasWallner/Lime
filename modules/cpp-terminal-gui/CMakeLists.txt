

add_library(cpp-terminal-gui
	cpp-terminal-gui/RenderTrait.hpp
	cpp-terminal-gui/EditTrait.hpp
	cpp-terminal-gui/FontStyle.hpp
	cpp-terminal-gui/FontStyleList.cpp	cpp-terminal-gui/FontStyleList.hpp
	cpp-terminal-gui/ColorString.cpp	cpp-terminal-gui/ColorString.hpp
	cpp-terminal-gui/TextEditor.cpp		cpp-terminal-gui/TextEditor.hpp
	cpp-terminal-gui/CommandLine.cpp	cpp-terminal-gui/CommandLine.hpp
	cpp-terminal-gui/VerticalGrid.cpp	cpp-terminal-gui/VerticalGrid.hpp
	
)

target_include_directories(cpp-terminal-gui PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cpp-terminal-gui 
	utf8_string
	cpp-terminal
)


option(CPP_TERMINAL_GUI_ENABLE_TESTING "Set to ON to enable testing" ON)

if(CPP_TERMINAL_GUI_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()