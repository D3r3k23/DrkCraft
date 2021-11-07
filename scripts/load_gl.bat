@echo OFF

pushd .
cd lib\glad\glad

python -m glad --generator c --no-loader --out-path ..\output

popd
