APP_NAME = main
APP_SRCS = main.cpp memory.cpp
APP_OBJS = $(APP_SRCS:.cpp=.o)

CXX = g++
CXXFLAGS = -Wall -Werror

all: $(APP_NAME)

$(APP_NAME): $(APP_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(APP_OBJS)

clean:
	rm -f $(APP_NAME) $(APP_OBJS)

$(APP_OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all