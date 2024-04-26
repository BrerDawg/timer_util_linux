# A Makefile for both Linux and Windows, 06-dec-2017

#define all executables here
app_name= timer_util_linux


all: ${app_name}


#define compiler options	
CC=g++

ifneq ($(OS),Windows_NT)			#linux?
#	CFLAGS=-g -O0 -fno-inline -Dbuild_date="\"`date +%Y-%b-%d`\"" #-Dbuild_date="\"2016-Mar-23\""
	CFLAGS=-g -O0  -Wfatal-errors -fpermissive -fno-inline -Dbuild_date="\"`date +%Y-%b-%d`\"" #-Dbuild_date="\"2016-Mar-23\""			#64 bit
#	LIBS=-L/usr/X11/lib -L/usr/local/lib -lfltk_images /usr/local/lib/libfltk.a -lpng -lz -ljpeg -lrt -lm -lXcursor -lXfixes -lXext -lXft -lfontconfig -lXinerama -lpthread -ldl -lX11 -lfftw3 #-ljack
#	INCLUDE= -I/usr/local/include
	LIBS=-lfltk -lfltk_images -lX11 -lpng -lz -ljpeg -lrt -lm -lXcursor -lXfixes -lXext -lXft -lfontconfig -lXinerama -lXrender -ldl -lX11	#64 bit
	INCLUDE= -I/usr/include/cairo	#64 bit

else								#windows?
	CFLAGS=-g -O0 -fno-inline -DWIN32 -mms-bitfields -Dcompile_for_windows -Dbuild_date="\"`date +%Y\ %b\ %d`\""
LIBS= -L/usr/local/lib -static -mwindows -lfltk_images -lfltk -lfltk_png -lfltk_jpeg -lole32 -luuid -lcomctl32 -lwsock32 -lWs2_32 -lm -lfftw3 -lwinmm
	INCLUDE= -I/usr/local/include
endif



#define object files for each executable, see dependancy list at bottom
obj1= timer_util_linux.o GCProfile.o pref.o GCLed.o GCCol.o fluid.o button_mouse_click.o
#obj2= backprop.o layer.o



#linker definition
timer_util_linux: $(obj1)
	$(CC) $(CFLAGS) -o $@ $(obj1) $(LIBS)


#linker definition
#backprop: $(obj2)
#	$(CC) $(CFLAGS) -o $@ $(obj2) $(LIBS)



#compile definition for all cpp files to be complied into .o files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

%.o: %.cxx
	$(CC) $(CFLAGS) $(INCLUDE) -c $<



#dependancy list per each .o file
timer_util_linux.o: timer_util_linux.h globals.h GCProfile.h pref.h GCCol.h GCLed.h fluid.h button_mouse_click.h
GCProfile.o: GCProfile.h
pref.o: pref.h GCCol.h GCLed.h
GCCol.o:  GCCol.h
GCLed.o: GCLed.h
fluid.o: fluid.h globals.h
button_mouse_click.o: button_mouse_click.h

#layer.o: layer.h


.PHONY : clean
clean : 
		-rm $(obj1)					#remove obj files
ifneq ($(OS),Windows_NT)
		-rm ${app_name}				#remove linux exec
else
		-rm ${app_name}.exe			#remove windows exec
endif


