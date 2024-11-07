CC=gcc
EDIT=vi
ICON=icon.ico
SRC=main.c
TARGET=Pikalang
WINCC=i686-w64-mingw32-gcc
WINDRES=i686-w64-mingw32-windres
edit:
	${EDIT} ${SRC}
compileLinux:
	${CC} ${SRC} -o ${TARGET}Linux -m32 -static-libgcc -Wall -Wextra
compileWindows:
	echo 'MAINICON ICON "${ICON}"'>resource.rc
	${WINDRES} -O coff -o resource.res resource.rc
	${WINCC} ${SRC} -o ${TARGET}Windows.exe resource.res -static-libgcc -Wall -Wextra
	rm resource.rc resource.res
run:
	./${TARGET}Linux
