rm -f hello
rm -f hello.o
echo "assembling"
nasm -felf64 hello.asm
echo "linking"
# gcc -nostartfiles -o hello hello.o -lSDL2
ld -o hello -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -lSDL2 hello.o
echo "executing"
./hello


# nm -gD /lib/x86_64-linux-gnu/libSDL2.so | grep CreateWindow
