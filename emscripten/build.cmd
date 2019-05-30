
del *.o
call emcc ..\sources\bitmaps.cpp -c
call emcc ..\sources\debug-report.cpp -c
call emcc ..\sources\debug.cpp -c
call emcc ..\sources\demons.cpp -c
call emcc ..\sources\drawing.cpp -c
call emcc ..\sources\filehandling.cpp -c
call emcc ..\sources\gameaux.cpp -c
call emcc ..\sources\gamecycle.cpp -c
call emcc ..\sources\gameinteractions.cpp -c
call emcc ..\sources\gameobjects.cpp -c
call emcc ..\sources\gametiles.cpp -c
call emcc ..\sources\ingamecycle.cpp -c
call emcc ..\sources\keyboard.cpp -c
call emcc ..\sources\line_clip.cpp -c
call emcc ..\sources\loadpcx.cpp -c
call emcc ..\sources\main.cpp -c
call emcc ..\sources\passwords.cpp -c
call emcc ..\sources\path.cpp -c
call emcc ..\sources\render.cpp -c
call emcc ..\sources\roomchange.cpp -c
call emcc ..\sources\shrines.cpp -c
call emcc ..\sources\sound.cpp -c
call emcc --shell-file  shell-minimal.html bitmaps.o debug-report.o debug.o demons.o drawing.o filehandling.o gameaux.o gamecycle.o gameinteractions.o gameobjects.o gametiles.o ingamecycle.o keyboard.o line_clip.o loadpcx.o main.o passwords.o path.o render.o roomchange.o shrines.o sound.o -o mog.html --preload-file ../assets@/ -s TOTAL_MEMORY=67108864 --use-preload-plugins -s STB_IMAGE=1 -O2

