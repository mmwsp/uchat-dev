CLANG = clang -std=c11 -Wall -Wextra -Werror -Wpedantic #-fsanitize=address -fno-omit-frame-pointer -g -Iinc -I/opt/homebrew/Cellar/json-c/0.16/include/ -I/opt/homebrew/include
UCSF = uchat_src/*.c 
UCSSF = uchat_server_src/*.c 
OD = obj
UCFN = uchat
UCSFN = uchat_server
LIBPATH = -ljson-c -lsqlite3 #-L../libraries/lib -L/opt/homebrew/lib -pthread -ldl
LIB = 
GTKINC = -D_GNU_SOURCE `pkg-config --cflags gtk+-3.0`
GTKLIB = -rdynamic `pkg-config --cflags --libs gtk+-3.0`

all: clean uchat uchat_server clean 
	@brew install gtk+3
	@brew install json-c
	@brew install sqlite3

uchat: clean
	@${CLANG} ${GTKLIB} -o ${UCFN} ${UCSF} ${LIBPATH} 

uchat_server: clean
	@make -sC uchat_server_src
	

uninstall: clean
	@make uninstall -sC uchat_server_src
	@rm -f ${UCFN}
	@rm -f ${UCSFN}

clean:
	@make clean -sC uchat_server_src
	@rm -rdf ${OD} 

reinstall: uninstall uchat uchat_server clean 

