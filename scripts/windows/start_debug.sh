rm ./bin/debug.exe 2> ./debug/debug_script_errors.txt

git pull 2> ./debug/debug_script_errors.txt

./scripts/windows/debug_compile.sh

./bin/debug.exe