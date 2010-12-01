#!/bin/bash

make all
./bin/lzw < input_test > packed_test
./bin/unlzw < packed_test > unpacked_test
diff input_test unpacked_test 