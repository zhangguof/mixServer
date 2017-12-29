#!/bin/sh
# bin/protoc --proto_path=proto \
# --cpp_out=proto \
# --python_out=proto \
# proto/test.proto
cd proto/gen_proto
python gen_pb.py