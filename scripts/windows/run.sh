rm ./bin/main.exe

git pull 2> ./debug/git_errors.txt

./scripts/windows/compile.sh

./bin/main.exe