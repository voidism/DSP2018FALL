.PHONY: all clean

CFLAGS+= -O3
CPPFLAGS += $(CFLAGS)
LDFLAGS+=-lm     # link to math library

TARGET=test_hmm train test accuracy

all: $(TARGET)
# type make/make all to compile test_hmm

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
