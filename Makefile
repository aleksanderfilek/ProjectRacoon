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
	LIBDIR = lib/windows
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

else
	$(MKDIR) build/debug/$(OS)/assets
	$(COPY) assets build/debug/$(OS)/assets
	$(COPY) $(LIBDIR)/linux build/debug
endif

compile:
ifeq ($(OS),windows)

else
	$(CC) -D DEBUG -c -I$(INCDIR) -L$(LIBDIR)/$(OS) $(SOURCES) $(LIBS)
endif

link:
ifeq ($(OS),windows)

else
	$(CC) -D DEBUG -I$(INCDIR) -L$(LIBDIR)/$(OS) -Wl,-rpath,. -o build/debug/linux/$(TARGET) $(OBJECTS)  $(LIBS)
endif

clear:
ifeq ($(OS),windows)
	$(IFEXIST) build/debug/$(OS) $(RMDIR) build/debug/$(OS)
else
	$(RMDIR) build/debug/$(OS)
endif

clean:
	$(RM) *.o

run:
ifeq ($(OS),windows)

else
	./build/debug/$(OS)/$(TARGET)
endif
