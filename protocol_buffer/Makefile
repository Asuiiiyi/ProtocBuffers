PROTOC=protoc
GPP=g++
PROTO_DIR=/home/asuiiiyi/Documents/protocol_buffer/test
OUTPUT_DIR=/home/asuiiiyi/Documents/protocol_buffer

all: generate compile

generate:
	$(PROTOC) --proto_path=$(PROTO_DIR) $(PROTO_DIR)/Person.proto --cpp_out=$(PROTO_DIR) && $(PROTOC) --proto_path=$(PROTO_DIR) $(PROTO_DIR)/Color.proto --cpp_out=$(PROTO_DIR)
compile:
	$(GPP) -o $(OUTPUT_DIR)/demo $(OUTPUT_DIR)/demo.cpp $(PROTO_DIR)/Person.pb.cc $(PROTO_DIR)/Color.pb.cc -std=c++11 -lprotobuf -L/usr/lib

clean:
	rm -f $(PROTO_DIR)/Person.pb.h $(PROTO_DIR)/Person.pb.cc $(PROTO_DIR)/Color.pb.h $(PROTO_DIR)/Color.pb.cc $(OUTPUT_DIR)/demo
