OS ?= windows

CC = gcc
TARGET = libhero
INCDIR = include
LIBDIR = lib
SOURCES = source/*.c
OBJECTS = *.o
BUILDDIR = lib/windows
TARGET = main

ifeq ($(OS),windows)
	RM = del /F /Q
	RMDIR = rmdir /S /Q
	MKDIR = mkdir
	COPY = xcopy /s /e
	IFEXIST = if exist
	LIBDIR = lib\windows
	LIBS = -lhero -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lsoil -lglew32 -lopengl32 -lglu32 -lm
	EXTENSION = .exe
else
	OS = linux
	RM = rm -rf
	RMDIR = rm -rf
	MKDIR = mkdir -p
	COPY = cp -R
	LIBDIR = lib
	LIBS = -lhero -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSOIL -lGLEW -lGL -lGLU -lm
endif

build: clear pack compile link clean

pack:
ifeq ($(OS),windows)
	$(MKDIR) build\debug\$(OS)\assets
	$(COPY) assets build\debug\$(OS)\assets
	$(COPY) $(LIBDIR) build\debug\windows
else
	$(MKDIR) build/debug/$(OS)/assets
	$(COPY) assets build/debug/$(OS)/assets
	$(COPY) $(LIBDIR)/linux build/debug
endif

compile:
ifeq ($(OS),windows)
	$(CC) -g -D DEBUG -c -I$(INCDIR) -L$(LIBDIR) $(SOURCES) $(LIBS)
else
	$(CC) -g -D DEBUG -c -I$(INCDIR) -L$(LIBDIR)/$(OS) $(SOURCES) $(LIBS)
endif

link:
ifeq ($(OS),windows)
	$(CC) -g -D DEBUG -I$(INCDIR) -L$(LIBDIR) -o build/debug/windows/$(TARGET) $(OBJECTS)  $(LIBS)
else
	$(CC) -g -D DEBUG -I$(INCDIR) -L$(LIBDIR)/$(OS) -Wl,-rpath,. -o build/debug/linux/$(TARGET) $(OBJECTS)  $(LIBS)
endif

clear:
ifeq ($(OS),windows)
	$(IFEXIST) build\debug\$(OS) $(RMDIR) build\debug\$(OS)
else
	$(RMDIR) build/debug/$(OS)
endif

clean:
	$(RM) *.o

run:
ifeq ($(OS),windows)
	./build/debug/$(OS)/$(TARGET).exe
else
	./build/debug/$(OS)/$(TARGET)
endif
