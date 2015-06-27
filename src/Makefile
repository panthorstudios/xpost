#  X Project Makefile
#
#  Patrick Joiner 
#  org
#  ver: 
#
CC=	gcc 
EXEC=	xpost	
LIBS=	-lX11 -lXaw -lXmu -lXt -lm
HEADERS=	main.h

SLIBS=	-lcurses -ltermcap -lm

SOURCES=	init.c \
		disk.c \
		diskmenu.c  \
		main.c \
		views.c\
		cameramenu.c \
		menusetup.c\
		widgets.c\
		movemenu.c \
		splmenu.c \
		mergemenu.c \
		splice.c \
		merge.c \
		3d.c \
		lightmenu.c \
                spl.c 

OBJECTS=	init.o	\
		disk.o \
		diskmenu.o  \
		main.o \
		views.o\
		cameramenu.o \
		menusetup.o\
		widgets.o\
		movemenu.o \
		splmenu.o \
		mergemenu.o \
		splice.o \
		merge.o \
		3d.o \
		lightmenu.o \
                spl.o 
 
$(EXEC):	$(OBJECTS) $(HEADERS)
		$(CC) -o $(EXEC) $(OBJECTS) $(LIBS)
		@cp xpost ../target/xpost
		@chmod 755 ../target/xpost
		@chmod 755 xpost
