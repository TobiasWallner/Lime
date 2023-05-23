

add_library(cpp-terminal-gui
	cpp-terminal-gui/RenderTrait.hpp
	cpp-terminal-gui/Command.hpp
	cpp-terminal-gui/CommandList.hpp
	cpp-terminal-gui/Label.cpp			cpp-terminal-gui/Label.hpp
	
)

target_include_directories(cpp-terminal-gui PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cpp-terminal-gui 
	utf8_string
	cpp-terminal
)