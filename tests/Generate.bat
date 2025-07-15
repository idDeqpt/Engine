mkdir build
cd build

cmake ..
cmake --build .

cd ..
copy "..\bin\lib\Debug\Math.dll" ".\bin\Debug\"

pause