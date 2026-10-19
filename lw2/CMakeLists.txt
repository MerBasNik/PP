add_executable(blur
	main.cpp
	Bmp.cpp
)

target_link_libraries(blur PRIVATE Threads::Threads)
target_include_directories(blur SYSTEM PRIVATE third_party/stb)
