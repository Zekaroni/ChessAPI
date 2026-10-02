rm ./bin/main.exe 2> ./debug/run_script_errors.txt

git pull 2> ./debug/run_script_errors.txt

./scripts/windows/compile.sh

./bin/main.exe