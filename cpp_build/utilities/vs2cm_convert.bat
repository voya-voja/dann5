@echo off
title Conversion from Visual Studio to CMake solution

SET USER_PATH=C:\Users\Nebojsa.Vojinovic
SET CONDA3_PATH=%USER_PATH%\AppData\Local\Continuum\anaconda3
SET ENV_PATH=%CONDA3_PATH%\envs\ocean38
SET CMAKE_CONVERTER=%ENV_PATH%\Lib\site-packages\cmake_converter

%ENV_PATH%\python %CMAKE_CONVERTER%\main.py %1 %2 %3 %4 %5 %6 %7 %8 %9
pause
