CXXFLAGS = -Wall -pedantic -std=c++17

HEADERS = $(wildcard src/*.h)
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=bin/%.o)

.PHONY: all
all: compile doc

.PHONY: run
run: compile
	./formama9

.PHONY: compile
compile: formama9

formama9: bin/main
	cp $< $@

bin/main: $(OBJECTS)
	mkdir -p $(@D)
	g++ $^ -o $@

bin/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(CXXFLAGS) $< -c -o $@

doc: Doxyfile $(HEADERS)
	doxygen Doxyfile

.PHONY: clean
clean:
	rm -rf formama9 bin/ doc/ 2>/dev/null


bin/CMenu.o: src/CMenu.cpp src/CMenu.h src/CPromptmsg.h src/CSettings.h \
     src/CTemplate.h src/CParameter.h src/CDownloader.h
bin/CSettings.o: src/CSettings.cpp src/CSettings.h src/CMenu.h \
     src/CPromptmsg.h src/CTemplate.h src/CParameter.h
bin/CPromptmsg.o: src/CPromptmsg.cpp src/CPromptmsg.h
bin/CTemplate.o: src/CTemplate.cpp src/CTemplate.h src/CParameter.h \
     src/CUrl.h src/CDirectory.h src/CType.h
bin/CUrl.o: src/CUrl.cpp src/CUrl.h src/CParameter.h src/CPromptmsg.h
bin/CDirectory.o: src/CDirectory.cpp src/CDirectory.h src/CParameter.h \
     src/CPromptmsg.h
bin/CType.o: src/CType.cpp src/CType.h src/CParameter.h src/CPromptmsg.h
bin/CSocket.o: src/CSocket.cpp src/CSocket.h
bin/CDownloader.o: src/CDownloader.cpp src/CDownloader.h src/CMenu.h \
     src/CPromptmsg.h src/CSettings.h src/CTemplate.h src/CParameter.h \
     src/CDownload.h src/CSocket.h src/CDowtype.h
bin/CDownload.o: src/CDownload.cpp src/CDownload.h src/CDownloader.h \
     src/CMenu.h src/CPromptmsg.h src/CSettings.h src/CTemplate.h \
     src/CParameter.h src/CSocket.h src/CDowtype.h src/CDowhtml.h \
     src/CDowcss.h src/CDowjs.h src/CDowimg.h
bin/CDowcss.o: src/CDowcss.cpp src/CDowcss.h src/CDowtype.h src/CTemplate.h \
     src/CParameter.h src/CSocket.h
bin/CDowjs.o: src/CDowjs.cpp src/CDowjs.h src/CDowtype.h src/CTemplate.h \
     src/CParameter.h src/CSocket.h
bin/CDowimg.o: src/CDowimg.cpp src/CDowimg.h src/CDowtype.h src/CTemplate.h \
     src/CParameter.h src/CSocket.h
bin/CDowhtml.o: src/CDowhtml.cpp src/CDowhtml.h src/CDowtype.h \
     src/CTemplate.h src/CParameter.h
bin/CArguments.o: src/CArguments.cpp src/CArguments.h src/CTemplate.h \
	 src/CParameter.h src/CSettings.h src/CMenu.h src/CPromptmsg.h
