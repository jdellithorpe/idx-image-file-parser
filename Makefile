all: idx-image-file-parser

idx-image-file-parser: idx-image-file-parser.cc
	g++ -o $@ $^
