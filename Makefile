CXX=g++
OBJ_DIR=Out
CLIENT=Client
SERVER=Server

SOURCE_DIR=Src
CPPFLAGS=-g -Wall


SRC_COMMON = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC_COMMON))
OBJ_COMMON = $(subst Src/,Out/,$(OBJ))

SRC_SERVER = $(wildcard $(SOURCE_DIR)/$(SERVER)/*.cpp)
OBJ_S = $(patsubst %.cpp,%.o,$(SRC_SERVER))
OBJ_SERVER = $(subst Src/,Out/,$(OBJ_S))

SRC_CLIENT = $(wildcard $(SOURCE_DIR)/$(CLIENT)/*.cpp)
OBJ_C = $(patsubst %.cpp,%.o,$(SRC_CLIENT))
OBJ_CLIENT = $(subst Src/,Out/,$(OBJ_C))

all: dir server client


dir: | $(OBJ_DIR)


$(OBJ_DIR):
	@echo "Creating object dirs"
	@mkdir $@
	@mkdir $@/$(CLIENT)
	@mkdir $@/$(SERVER)


server: $(OBJ_COMMON) $(OBJ_SERVER)
	$(CXX) $(OBJ_COMMON) $(OBJ_SERVER) -o $@

client: $(OBJ_COMMON) $(OBJ_CLIENT)
	$(CXX) $(OBJ_COMMON) $(OBJ_CLIENT) -o $@

$(OBJ_COMMON): $(SRC_COMMON)
	$(CXX) $(CPPFLAGS) -c -o $@ $(subst Out/,Src/,$(subst .o,.cpp,$@))

$(OBJ_SERVER): $(SRC_SERVER)
	$(CXX) $(CPPFLAGS) -c -o $@ $(subst Out/,Src/,$(subst .o,.cpp,$@))
    
$(OBJ_CLIENT): $(SRC_CLIENT)
	$(CXX) $(CPPFLAGS) -c -o $@ $(subst Out/,Src/,$(subst .o,.cpp,$@))

clean:
	@rm -fr Out
	@rm client server

.PHONY: all dir clean