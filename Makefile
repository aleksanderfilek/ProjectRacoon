OS ?= windows

CC = gcc
TARGET = libhero
INCDIR = include
LIBDIR = lib
SOURCES = source/*.c
OBJECTS = *.o
BUILDDIR = lib/windows
TARGET = main
VERSION ?= debug
VERSIONFLAGS ?= -g -D DEBUG

ifeq ($(VERSION), release)
	VERSIONFLAGS = -O3
endif

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
rebuild: compile link clean

pack:
ifeq ($(OS),windows)
	$(MKDIR) build\$(VERSION)\$(OS)\assets
	$(COPY) assets build\$(VERSION)\$(OS)\assets
	$(COPY) $(LIBDIR) build\$(VERSION)\windows
else
	$(MKDIR) build/$(VERSION)/$(OS)/assets
	$(COPY) assets build/$(VERSION)/$(OS)/assets
	$(COPY) $(LIBDIR)/linux build/$(VERSION)
endif

compile:
ifeq ($(OS),windows)
	$(CC) $(VERSIONFLAGS) -c -I$(INCDIR) -L$(LIBDIR) $(SOURCES) $(LIBS)
else
	$(CC) $(VERSIONFLAGS) -c -I$(INCDIR) -L$(LIBDIR)/$(OS) $(SOURCES) $(LIBS)
endif

link:
ifeq ($(OS),windows)
	$(CC) $(VERSIONFLAGS) -I$(INCDIR) -L$(LIBDIR) -o build/$(VERSION)/windows/$(TARGET) $(OBJECTS)  $(LIBS)
else
	$(CC) $(VERSIONFLAGS) -I$(INCDIR) -L$(LIBDIR)/$(OS) -Wl,-rpath,. -o build/$(VERSION)/linux/$(TARGET) $(OBJECTS)  $(LIBS)
endif

clear:
ifeq ($(OS),windows)
	$(IFEXIST) build\$(VERSION)\$(OS) $(RMDIR) build\$(VERSION)\$(OS)
else
	$(RMDIR) build/$(VERSION)/$(OS)
endif

clean:
	$(RM) *.o

run:
ifeq ($(OS),windows)
	./build/debug/$(OS)/$(TARGET).exe
else
	./build/debug/$(OS)/$(TARGET)
endif
