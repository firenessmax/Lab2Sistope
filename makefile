build_dir=build
src_dir=src

all: folders
	g++ -o $(build_dir)/main.o $(src_dir)/main.cpp -lpthread

folders:
	mkdir $(build_dir)