CXX := g++
CXXFLAGS := -std=c++11 -Wall -I.

CLIENT_DIR := Client

SERVER_SRCS := Solver_API.cc ODE_Solver.cc Server.cc
SERVER_OBJS := $(SERVER_SRCS:.cc=.o)
SERVER_BIN := server

CLIENT_SRCS := $(CLIENT_DIR)/Solver_Client.cc $(CLIENT_DIR)/Mock_Client.cc
CLIENT_OBJS := $(CLIENT_SRCS:.cc=.o)
CLIENT_BIN := client

.PHONY: all tidy clean

all: $(SERVER_BIN) $(CLIENT_BIN) tidy

$(SERVER_BIN): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CLIENT_BIN): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CLIENT_DIR)/%.o: $(CLIENT_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

tidy:
	rm -f *.o $(CLIENT_DIR)/*.o

clean:
	rm -f *.o $(CLIENT_DIR)/*.o $(SERVER_BIN) $(CLIENT_BIN)