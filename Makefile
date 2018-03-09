CFLAGS = -c -std=c++11

LDFLAGS = 

DEBUG = -g

TARGET = feed-grab

BUILDIR = build

INCLUDE = -Icore/Headers

SQLITECORE_HEADERS = core/Headers/grabber.h \

SQLITECORE_SOURCES = core/Sources/grabber.cpp \

SOURCES = 	main.cpp \
			$(SQLITECORE_SOURCES) \

OBJECTS = 	$(addprefix $(BUILDIR)/, \
			main.o \
			grabber.o)

all: $(OBJECTS)
	$(CXX) $(DEBUG) $(OBJECTS) -o $(BUILDIR)/$(TARGET)

$(BUILDIR)/main.o: core/Headers/grabber.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) main.cpp -o $@

$(BUILDIR)/grabber.o: core/Headers/grabber.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) core/Sources/grabber.cpp -o $@	

clean:
	$(RM) $(BUILDIR)/*