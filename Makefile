#
#
PROGNAME=tedit
MAJORVERSION=0
MINORVERSION=1
RELEASEVERSION=1

OWNER=mduffy
GROUP=programming
CTAGS= ctags -x >tags
CFLAGS= -O -Wall -DLINUX -I./ -I../RavenLib/
DEBUGFLAGS= -g -DDEBUG
#-ansi
# remove symobl table and relcoation info from executable
LDFLAGS= -s -L$(HOME)/lib

CC=g++
GET=cvs co
INSTALL=install
LDCONFIG=/sbin/ldconfig


SRCS=Types.cpp Debug.cpp RStr.cpp BaseArray.cpp IntArray.cpp RStrArray.cpp RegEx.cpp \
     FilePath.cpp  RStrParser.cpp GapBuffer.cpp Main.cpp 
OBJS=$(SRCS:.cpp=.o)
OBJPATH=./obj
SHAR=shar
MANDIR=
BINDIR=
DEPEND= makedepend $(CFLAS)
VPATH=$(OBJPATH)

.SUFFIXES: .cpp .hpp .o


all: ${PROGNAME}
	@echo "Testing all\n"
	echo $(OBJS)

# get things out of CVS
#$(SRCS):
#	$(GET) $@

# to make an executable

$(PROGNAME): $(OBJS)
	@echo "Testing all\n"
	echo $(OBJS)
	@echo ""
	@echo "Linking $(PROGNAME)"
	mkdir -p $(OBJPATH)
	$(CC) $(LDFLAGS) -o $@ $(OBJS:%=$(OBJPATH)/%)


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

install:
	$(INSTALL) -m 0775 -g $GROUP -o $OWNER $(PROGNAME) ${HOME}/lib/$(PROGNAME)
	$(LDCONFIG) -n ${HOME}/lib
uninstall:
	rm ${HOME}/lib/$(PROGNAME)


%.o : %.cpp
	@echo ""
	@echo "Compling $(@)"
	$(CC) $(CFLAGS) -o $(OBJPATH)/$(@) -c $(@:.o=.cpp)


