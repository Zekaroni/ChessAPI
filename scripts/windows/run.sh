rm ./bin/main.exe 2> ./debug/run_script_errors.txt

git pull 2> ./debug/run_script_errors.txt

./scripts/windows/main_compile.sh

./bin/main.exe