#/bin/sh

cd inputs
clang -O0 -emit-llvm -c *.c
for file in `ls *.bc | sed -e 's/\.[a-zA-Z]*$//'`
do
  opt -mem2reg "$file.bc" -o "$file.reg.bc"
  llvm-dis "$file.reg.bc"
done
