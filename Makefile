SHELL=cmd.exe
USE_DEBUG = NO
USE_FLAMES = NO

ifeq ($(USE_DEBUG),YES)
CFLAGS=-Wall -O -g -mwindows
LFLAGS=
else
CFLAGS=-Wall -O3 -mwindows
LFLAGS=-s
endif
CFLAGS += -Wno-write-strings
CFLAGS += -Weffc++
CFLAGS += -std=c++0x

CSRC=winiface.cpp alg_selector.cpp gfuncs.cpp ezfont.cpp rgb_data.cpp palettes.cpp \
	font.dialog.cpp gobjects.cpp stained_glass.cpp flames.cpp faces.cpp \
	rainbow.cpp pixels.cpp ascii.cpp rcolors.cpp line_games.cpp lines.cpp \
	gpalettes.cpp xnpalette.cpp xpalette.cpp xrect.cpp rect.cpp polygon.cpp \
   squares.cpp circles.cpp colorbars.cpp bitblt.cpp triangles.cpp wincolors.cpp \
	acidwarp.cpp lut.cpp

ifeq ($(USE_FLAMES),YES)
CSRC += flame_palettes.cpp
else
CFLAGS += -DSKIP_FLAME_PALETTES
LiFLAGS += -DSKIP_FLAME_PALETTES
endif

OBJS = $(CSRC:.cpp=.o) rc.o

BINS=gstuff.exe

%.o: %.cpp
	g++ $(CFLAGS) -c $<

all: $(BINS)

clean:
	rm -f *.o *~ $(BINS) *.zip

dist:
	rm -f gstuff.zip
	zip gstuff.zip gstuff.* gfuncs.* demo.* ezfont.* rgb_data.* palettes.*

depend:
	makedepend $(CSRC)

lint:
	cmd /C "c:\lint9\lint-nt +v -width(160,4) $(LiFLAGS) -ic:\lint9 mingw.lnt -os(_lint.tmp) lintdefs.cpp $(CSRC)"

lint8:
	cmd /C "c:\lint8\lint-nt +v -width(160,4) $(LiFLAGS) -ic:\lint8 mingw.lnt -os(_lint.tmp) lintdefs.cpp $(CSRC)"

gstuff.exe: $(OBJS)
	g++ $(CFLAGS) $(LFLAGS) $(OBJS) -o $@ -lgdi32

rc.o: gstuff.rc
	windres -O COFF $< -o $@

# DO NOT DELETE

winiface.o: gstuff.h palettes.h gobjects.h gfuncs.h keywin32.h
alg_selector.o: gstuff.h palettes.h gobjects.h gfuncs.h keywin32.h
alg_selector.o: alg_selector.h ezfont.h
gfuncs.o: ezfont.h palettes.h
ezfont.o: gfuncs.h ezfont.h
rgb_data.o: rgb_data.h
palettes.o: palettes.h
font.dialog.o: gstuff.h palettes.h gobjects.h gfuncs.h
gobjects.o: gstuff.h palettes.h gobjects.h
stained_glass.o: gstuff.h palettes.h gobjects.h gfuncs.h
flames.o: gstuff.h gfuncs.h palettes.h gobjects.h
faces.o: gstuff.h gfuncs.h palettes.h gobjects.h ezfont.h
rainbow.o: gstuff.h gfuncs.h palettes.h gobjects.h ezfont.h
pixels.o: gstuff.h palettes.h gobjects.h gfuncs.h
ascii.o: gstuff.h palettes.h gobjects.h gfuncs.h ezfont.h
rcolors.o: gstuff.h palettes.h gobjects.h gfuncs.h
line_games.o: gstuff.h palettes.h gobjects.h gfuncs.h
lines.o: gstuff.h palettes.h gobjects.h gfuncs.h
gpalettes.o: gstuff.h palettes.h gobjects.h gfuncs.h rgb_data.h cheetah.def
gpalettes.o: hometown.def
xnpalette.o: gstuff.h palettes.h gobjects.h gfuncs.h rgb_data.h
xpalette.o: gstuff.h palettes.h gobjects.h gfuncs.h rgb_data.h
xrect.o: gstuff.h palettes.h gobjects.h gfuncs.h rgb_data.h
rect.o: gstuff.h palettes.h gobjects.h gfuncs.h
polygon.o: gstuff.h palettes.h gobjects.h gfuncs.h
squares.o: gstuff.h palettes.h gobjects.h gfuncs.h
circles.o: gstuff.h palettes.h gobjects.h gfuncs.h
colorbars.o: gstuff.h palettes.h gobjects.h gfuncs.h
bitblt.o: gstuff.h palettes.h gobjects.h gfuncs.h
triangles.o: gstuff.h palettes.h gobjects.h gfuncs.h
wincolors.o: gstuff.h palettes.h gobjects.h gfuncs.h
acidwarp.o: gstuff.h palettes.h gobjects.h gfuncs.h lut.h
lut.o: lut.h
