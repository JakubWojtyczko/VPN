CXX=g++
OBJ_DIR=Out
CLIENT=Client
SERVER=Server

SOURCE_DIR=Src
CPPFLAGS=-g -Wall -DDEBUG -std=c++17

# Add libs for SSL
LIBS=-lssl -lcrypto

# Add directory with common headers
INC = -I$(SOURCE_DIR)

TARGETS=server client
TARGETS_WIN_EXT=server.exe client.exe

SRC_COMMON = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC_COMMON))
OBJ_COMMON = $(subst Src/,Out/,$(OBJ))

SRC_SERVER = $(wildcard $(SOURCE_DIR)/$(SERVER)/*.cpp)
OBJ_S = $(patsubst %.cpp,%.o,$(SRC_SERVER))
OBJ_SERVER = $(subst Src/,Out/,$(OBJ_S))

SRC_CLIENT = $(wildcard $(SOURCE_DIR)/$(CLIENT)/*.cpp)
OBJ_C = $(patsubst %.cpp,%.o,$(SRC_CLIENT))
OBJ_CLIENT = $(subst Src/,Out/,$(OBJ_C))


ifeq ($(OS),Windows_NT)
	# add library for WinSockets when Windows
	WIN_LIBS=-lWs2_32 -lgdi32
	# add library for iphlpapi
	WIN_LIBS+=-liphlpapi
	# add Library for packet capturing
	WIN_LIBS+=-lwpcap
else
	LFLAGS=-pthread
	LIBS+=-lpcap
endif


all: dir $(TARGETS)
 

dir: | $(OBJ_DIR)


$(OBJ_DIR):
ifeq ($(OS),Windows_NT)
	@mkdir $@
	@mkdir $@\$(CLIENT)
	@mkdir $@\$(SERVER)
else
	@mkdir -p $@/$(CLIENT)
	@mkdir -p $@/$(SERVER)
endif


server: $(OBJ_COMMON) $(OBJ_SERVER)
	$(CXX) $(LFLAGS) $(OBJ_COMMON) $(OBJ_SERVER) -o $@ $(LIBS) $(WIN_LIBS)

client: $(OBJ_COMMON) $(OBJ_CLIENT)
	$(CXX) $(LFLAGS) $(OBJ_COMMON) $(OBJ_CLIENT) -o $@ $(LIBS) $(WIN_LIBS)

$(OBJ_COMMON): $(SRC_COMMON)
	$(CXX) $(CPPFLAGS) -c -o $@ $(subst Out/,Src/,$(subst .o,.cpp,$@))

$(OBJ_SERVER): $(SRC_SERVER)
	$(CXX) $(CPPFLAGS) $(INC) -c -o $@ $(subst Out/,Src/,$(subst .o,.cpp,$@))

$(OBJ_CLIENT): $(SRC_CLIENT)
	$(CXX) $(CPPFLAGS) $(INC) -c -o $@ $(subst Out/,Src/,$(subst .o,.cpp,$@))


clean:
ifeq ($(OS),Windows_NT)
	@rmdir /S /Q $(OBJ_DIR)
	@del /Q $(TARGETS_WIN_EXT)
else
	@rm -fr $(TARGETS) $(OBJ_DIR)
endif

.PHONY: all dir clean
