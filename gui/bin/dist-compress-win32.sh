#!/bin/sh

strip -s etc/dist/win32/financer/financer.exe

bin/upx.exe -9 etc/dist/win32/financer/financer.exe
