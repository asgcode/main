#
# Makefile to build ProcessSensorData project
#

# Name of the executable
TARGET = ProcessSensorData

# Include path for header files relative to Makefile
INCLUDE_PATH = ../../include

# Source code path relative to Makefile
VPATH        = ../../source
OBJ_DIR 	 = obj


# Header files
_HEADERS = \
		   CommonUtils.h \
		   FileIO.h \
		   ProcessSensor.h

# C source code files 
SOURCES = \
    	Main.c \
    	CommonUtils.c \
    	FileIO.c \
    	ProcessSensorData.c

# Rules to convert sources to object file
_OBJ = $(SOURCES:.c=.o)
DEPS = $(patsubst %,$(INCLUDE_PATH)/%,$(_HEADERS))
OBJ  = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

# Compiler options, gcc compiler, enable warnings
CC     = gcc
CFLAGS = -I$(INCLUDE_PATH) -Wall

$(OBJ_DIR)/%.o: %.c $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)


$(TARGET): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)