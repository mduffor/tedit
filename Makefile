#
#
PROGNAME=tedit
MAJORVERSION=0
MINORVERSION=1
RELEASEVERSION=1

OWNER=mduffy
GROUP=programming
CTAGS= ctags -x >tags
CFLAGS= -O -Wall -DLINUX -I./ -I../cppunit/include/
DEBUGFLAGS= -g -DDEBUG
#-ansi
# remove symobl table and relcoation info from executable
LDFLAGS= -s -L$(HOME)/lib -lncurses
LDFLAGSTEST= ${LDFLAGS} -lcppunit -ldl -L/usr/lib/
#LDFLAGSTEST= ${LDFLAGS} -Wl,--no-as-needed -lcppunit

CC=g++
GET=cvs co
INSTALL=install
LDCONFIG=/sbin/ldconfig


SRCS=Types.cpp Debug.cpp RStr.cpp BaseArray.cpp IntArray.cpp RStrArray.cpp RegEx.cpp \
     FilePath.cpp  RStrParser.cpp GapBuffer.cpp GapBufferManager.cpp Main.cpp \
     Command.cpp BufferCommands.cpp FormatInfo.cpp EditorSettings.cpp EntryFieldHandler.cpp \
     NCursesShell.cpp SyntaxParser.cpp
OBJS=$(SRCS:.cpp=.o)

TESTSRCS=UnitMain.cpp Types.cpp Debug.cpp RStr.cpp BaseArray.cpp IntArray.cpp RStrArray.cpp RegEx.cpp \
     FilePath.cpp  RStrParser.cpp GapBuffer.cpp GapBufferManager.cpp \
     Command.cpp BufferCommands.cpp FormatInfo.cpp EditorSettings.cpp EntryFieldHandler.cpp \
     NCursesShell.cpp SyntaxParser.cpp \
     GapBufferTest.cpp IntArrayTest.cpp CommandTest.cpp RegExTest.cpp
TESTOBJS=$(TESTSRCS:.cpp=.o)


OBJPATH=./obj
SHAR=shar
MANDIR=
BINDIR=
DEPEND= makedepend $(CFLAS)
VPATH=$(OBJPATH)

.SUFFIXES: .cpp .hpp .o


all: ${PROGNAME} $(PROGNAME)_test
#	@echo "Testing all\n"
#	@echo $(OBJS)
	@echo "\n\nUnit Testing\n"
	@./${PROGNAME}_test

test: ${PROGNAME}_test
	@echo "\n\nUnit Testing\n"
	./${PROGNAME}_test

# get things out of CVS
#$(SRCS):
#	$(GET) $@

# to make an executable

$(PROGNAME): $(OBJS)
#	@echo "Testing progname\n"
#	echo $(OBJS)
#	@echo ""
	@echo "Linking $(PROGNAME)"
	mkdir -p $(OBJPATH)
	$(CC) -o $@ $(OBJS:%=$(OBJPATH)/%) $(LDFLAGS) 
	
$(PROGNAME)_test: $(TESTOBJS)
	@echo ""
	@echo "Linking $(PROGNAME)_test"
	mkdir -p $(OBJPATH)
	$(CC) -o $@ $(TESTOBJS:%=$(OBJPATH)/%) $(LDFLAGSTEST)

clean:
	rm -f $(OBJS:%=$(OBJPATH)/%)

Clean:
	rm -f $(OBJS:%=$(OBJPATH)/%) ${PROGNAME}
	rmdir $(OBJPATH);
distclean:
	Clean

cleandocs:
	rm -rf docs/
docs:
	doxygen Doxyfile > doxygen.log

debug:
	@$(MAKE) CFLAGS='$(CFLAGS) $(DEBUGFLAGS)' LIBS='$(LIBS) $(DEBUGLIBS)'

install:
	$(INSTALL) -m 0775 -g $GROUP -o $OWNER $(PROGNAME) ${HOME}/lib/$(PROGNAME)
	$(LDCONFIG) -n ${HOME}/lib
uninstall:
	rm ${HOME}/lib/$(PROGNAME)


%.o : %.cpp
	@echo ""
	@echo "Compling $(@)"
	$(CC) $(CFLAGS) -o $(OBJPATH)/$(@) -c $(@:.o=.cpp)


