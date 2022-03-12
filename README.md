# lz4

lz4 for embedded

# Reference

https://github.com/lz4/lz4

# Compile

make clean && make

# Test

head -c 1M </dev/urandom > ./testfile1.bin

./lz4 -c testfile1.bin testfile1.lz4

./lz4 -d testfile1.lz4 testfile2.bin

md5sum testfile1.bin

md5sum testfile2.bin

rm -rif testfile1.bin testfile2.bin testfile1.lz4



