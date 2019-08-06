# Where to copy the final library
#DEST	= ./plugin

# Where are mysql libraries
INCL	= -I/usr/include/mysql

TARGET = lib_mytanimoto.so
CFLAGS = -O2 -fPIC $(INCL) -DHAVE_DLOPEN=1


SRCS = 	lib_mytanimoto.cc
OBJS =  $(SRCS:%.cc=%.o)

all: $(TARGET)

install: $(TARGET)
	@echo
	@echo	"To install the udf functions, copy "
	@echo	"$(TARGET) to the mysql/plugins directory,"
	@echo	"then run the SQL file import.sql in MySQL "
	@echo   "(or MariaDB)"
	@echo

clean:
	$(RM) $(OBJS) $(TARGET)

%.o: %.cc
	$(CXX) -o $@ $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	$(LD) -shared -o $(TARGET) $(OBJS)
