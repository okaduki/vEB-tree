CFLAGS=-I. -g -Wall -Werror -O3 -g
CXXFLAGS=${CFLAGS} -std=c++17
TARGET=vebtree_test

INCS= \
Set.hpp \
vEBTree.hpp \
RSvEBTree.hpp

SRCS= \
vebtree_test.cpp

OBJS=$(addsuffix .o, $(basename $(SRCS)))

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJS): $(INCS)

.PHONY: clean
clean:
	rm -rf $(TARGET) *.o
