# Define the name of your application
APP_NAME = test_app

# Source files for your application
APP_SRCS = main.cpp
APP_OBJS = $(APP_SRCS:.cpp=.o)

# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -Werror -fexceptions

# Library name and source files
LIB_NAME = libmath_module.so
LIB_SRCS = math_module.cpp
LIB_OBJS = $(LIB_SRCS:.cpp=.o)

# Additional flags for creating PIC code
PICFLAGS = -fPIC

# The default target is 'all', which builds your application
all: $(APP_NAME) $(LIB_NAME)

$(APP_NAME): $(APP_OBJS) $(LIB_NAME)
	$(CXX) $(CXXFLAGS) -o $@ $(APP_OBJS) -L. -lmath_module -Wl,-rpath,'$$ORIGIN'

$(LIB_NAME): $(LIB_OBJS)
	$(CXX) -shared -o $@ $(LIB_OBJS) $(CXXFLAGS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(PICFLAGS) -c $< -o $@

# Clean rule to remove the executable, object files, and library
clean:
	rm -f $(APP_NAME) $(APP_OBJS) $(LIB_NAME) $(LIB_OBJS)
