CC=g++
SourceFiles="../main.cpp ../expr_tree.cpp ../parser.cpp"
CompilerFlags="-g -fsanitize=address -fsanitize=leak -Wall -Wextra"
Output="SD"

if [ ! -d "bin" ] 
then 
    mkdir bin
fi

cd bin/

$CC $SourceFiles $CompilerFlags -o $Output

cd ..