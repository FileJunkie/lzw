#!/bin/bash

make all
./lzw < input_test > packed_test
./unlzw < packed_test > unpacked_test
diff input_test unpacked_test 