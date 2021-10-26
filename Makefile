PROJDIR = $(realpath $(CURDIR))
HEADERDIR = include
SOURCEDIR = source

TARGET = main
CC = gcc
INC = -Iinclude

SOURCES = $(wildcard $(SOURCEDIR)/*.c)
OBJECTS = $(patsubst $(SOURCEDIR)/%.c, ./%.o, $(SOURCES))

ifeq ($(OS),Windows_NT)
	RM = del /F /Q
	RMDIR = rmdir /S /Q
	MKDIR = mkdir
	COPY = xcopy /s /e
	IFEXIST = if exist
	LIBDIR = lib\windows
	LIBS = -Llib\windows -lhero -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lsoil -lglew32 -lopengl32 -lglu32 -lm
	EXTENSION = .exe
else
	RM = rm -rf
	RMDIR = rm -rf
	MKDIR = mkdir -p
	IFEXIST = 
	LIBDIR = lib\linux
	LIBS = -lhero -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSOIL -lGLEW -lGL -lGLU -lm
	EXTENSION
endif

debug: clearDebug buildDebug clean
release: clearRelease buildRelease clean
runDebug: 
	build\debug\$(OS)\$(TARGET)$(EXTENSION)
runRelease: 
	build\release\$(OS)\$(TARGET)$(EXTENSION)

buildDebug:
	$(MKDIR) build\debug\$(OS)\assets
	$(COPY) assets build\debug\$(OS)\assets
	$(COPY) $(LIBDIR) build\debug\$(OS)
	$(CC) $(INC) -D DEBUG -c $(SOURCES)
	$(CC) $(INC) -D DEBUG -o build\debug\$(OS)\$(TARGET) $(OBJECTS) $(LIBS)

buildRelease:
	$(MKDIR) build\release\$(OS)\assets
	$(COPY) assets build\release\$(OS)\assets
	$(COPY) $(LIBDIR) build\release\$(OS)
	$(CC) $(INC) -O3 -c $(SOURCES)
	$(CC) $(INC) -O3 -mwindows -o build\release\$(OS)\$(TARGET) $(OBJECTS) $(LIBS)

clearDebug:
	$(IFEXIST) build\debug\$(OS) $(RMDIR) build\debug\$(OS)

clearRelease:
	$(IFEXIST) build\release\$(OS) $(RMDIR) build\release\$(OS)

clean:
	$(RM) *.o