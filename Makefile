CC=gcc
COMMITMESSAGE=""
EDIT=vi
ICON=icon.ico
SRC=main.c
STRIP=strip
WINSTRIP=i686-w64-mingw32-strip
TARGET=Pikalang
WINCC=i686-w64-mingw32-gcc
WINDRES=i686-w64-mingw32-windres
help:
	echo "Makefile commands:\n\tcommit [COMMITMESSAGE=<message>]  Commit to the repository using\n\t                                  <message> as the commit message (Leave\n\t                                  empty to enter description as well)\n\tcompileFreeBSD                    Compile for FreeBSD\n\tcompileLinux                      Compile for Linux\n\tcompileOSX                        Compile for OSX\n\tcompileWindows                    Compile for Windows\n\tedit [EDIT=<editor>]              Edit the source code (using Vim by\n\t                                  default)\n\tfetch                             Fetch updates from the repository\n\thelp                              Show help\n\tpush                              Push updates to the repository\n\trun                               Run on Linux"
commit:
	git add .
	if test -n "${COMMITMESSAGE}";then\
		git commit -m "${COMMITMESSAGE}";\
	else\
		git commit;\
	fi
compileFreeBSD:
	${CC} ${SRC} -o ${TARGET}FreeBSD -static -m32 -Wall -Wextra -s
	${STRIP} -s ${TARGET}FreeBSD
compileLinux:
	${CC} ${SRC} -o ${TARGET}Linux -m32 -static -Wall -Wextra -s
	${STRIP} -s ${TARGET}Linux
compileOSX:
	${CC} ${SRC} -o ${TARGET}OSX_x64 -Wall -Wextra --target=x86_64-apple-darwin
	${CC} ${SRC} -o ${TARGET}OSX_arm64 -Wall -Wextra --target=arm64-apple-darwin
	lipo -create -output ${TARGET}OSX ${TARGET}OSX_x64 ${TARGET}OSX_arm64
	rm ${TARGET}OSX_x64 ${TARGET}OSX_arm64
	${STRIP} ${TARGET}OSX
compileWindows:
	echo 'MAINICON ICON "${ICON}"'>resource.rc
	${WINDRES} -O coff -o resource.res resource.rc
	${WINCC} ${SRC} -o ${TARGET}Windows.exe resource.res -static -Wall -Wextra -s
	rm resource.rc resource.res
	${WINSTRIP} -s ${TARGET}Windows.exe
edit:
	${EDIT} ${SRC}
fetch:
	git fetch
	curl https://raw.githubusercontent.com/Amirreza-Ipchi-Haq/dynastr/refs/heads/main/dynastr.h>dynastr.h
push:
	git push
run:
	./${TARGET}Linux
