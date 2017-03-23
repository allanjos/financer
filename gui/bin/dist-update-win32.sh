#!/bin/sh

# Copy application

cp build/msys/bin/financer.exe etc/dist/win32/financer/financer.exe

# Copy language files

cp etc/data/config/lang/*.xml etc/dist/win32/financer/config/lang/

# Create documentation

pushd etc/data/doc

zip -r -9 ../../../etc/dist/win32/financer/doc.zip *

popd
