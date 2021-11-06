OS ?= windows

CC = gcc
TARGET = libhero
INCDIR = include
LIBDIR = lib
SOURCES = source/*.c
OBJECTS = *.o
BUILDDIR = lib/windows
TARGET = main
VER ?= debug
VERFLAGS ?= -g -D DEBUG
VEREXT ?= d

ifeq ($(VER), release)
	VERFLAGS = -O3
	VEREXT = 
endif

ifeq ($(OS),windows)
	RM = del /F /Q
	RMDIR = rmdir /S /Q
	MKDIR = mkdir
	COPY = xcopy /s /e
	IFEXIST = if exist
	LIBDIR = lib\windows
	LIBS = -lhero$(VEREXT) -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lsoil -lglew32 -lopengl32 -lglu32 -lm
	EXTENSION = .exe
else
	OS = linux
	RM = rm -rf
	RMDIR = rm -rf
	MKDIR = mkdir -p
	COPY = cp -R
	LIBDIR = lib
	LIBS = -lhero$(VEREXT) -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSOIL -lGLEW -lGL -lGLU -lm
endif

build: clear pack compile link clean
rebuild: compile link clean

pack:
ifeq ($(OS),windows)
	$(MKDIR) build\$(VER)\$(OS)\assets
	$(COPY) assets build\$(VER)\$(OS)\assets
	$(COPY) $(LIBDIR) build\$(VER)\windows
else
	$(MKDIR) build/$(VER)/$(OS)/assets
	$(COPY) assets build/$(VER)/$(OS)/assets
	$(COPY) $(LIBDIR)/linux build/$(VER)
endif

compile:
ifeq ($(OS),windows)
	$(CC) $(VERFLAGS) -c -I$(INCDIR) -L$(LIBDIR) $(SOURCES) $(LIBS)
else
	$(CC) $(VERFLAGS) -c -I$(INCDIR) -L$(LIBDIR)/$(OS) $(SOURCES) $(LIBS)
endif

link:
ifeq ($(OS),windows)
	$(CC) $(VERFLAGS) -I$(INCDIR) -L$(LIBDIR) -o build/$(VER)/windows/$(TARGET) $(OBJECTS)  $(LIBS)
else
	$(CC) $(VERFLAGS) -I$(INCDIR) -L$(LIBDIR)/$(OS) -Wl,-rpath,. -o build/$(VER)/linux/$(TARGET) $(OBJECTS)  $(LIBS)
endif

clear:
ifeq ($(OS),windows)
	$(IFEXIST) build\$(VER)\$(OS) $(RMDIR) build\$(VER)\$(OS)
else
	$(RMDIR) build/$(VER)/$(OS)
endif

clean:
	$(RM) *.o

run:
ifeq ($(OS),windows)
	./build/debug/$(OS)/$(TARGET).exe
else
	./build/debug/$(OS)/$(TARGET)
endif
