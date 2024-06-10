/*
Copyright (C) 2024 BrerDawg

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
hint
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/



//timer_util_linux.h
//v1.07		

#define _FILE_OFFSET_BITS 64			//large file handling, must be before all #include...
//#define _LARGE_FILES

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <locale.h>
#include <string>
#include <vector>
#include <wchar.h>
#include <algorithm>
#include <unistd.h>														//for 'readlink()'

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_File_Input.H>
#include <FL/Fl_Input_Choice.H>

#include "globals.h"
#include "pref.h"
#include "GCProfile.h"
#include "GCLed.h"
#include "fluid.h"
#include "button_mouse_click.h"

//linux code
#ifndef compile_for_windows

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
//#include <X11/Xaw/Form.h>							//64bit
//#include <X11/Xaw/Command.h>						//64bit

#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>		//MakeIniPathFilename(..) needs this
#endif


//windows code
#ifdef compile_for_windows
#include <windows.h>
#include <process.h>
#include <winnls.h>
#include <share.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <conio.h>

#define WC_ERR_INVALID_CHARS 0x0080		//missing from gcc's winnls.h
#endif


using namespace std;


#define cnFontEditor 4
#define cnSizeEditor 12

#define cnGap 2
#define cnCslHei 270

#define cnsLogName "log.txt"						//log filename
#define cnsLogSwapName "log_swap.txt"	//log swap file for culling older entries at begining of log file

//linux code
#ifndef compile_for_windows
#define LLU "llu"
#endif


//windows code
#ifdef compile_for_windows
#define LLU "I64u"
#endif


#define cns_help_filename "help.txt"

//linux code
#ifndef compile_for_windows
#define cns_open_editor "open_editor.sh"		// !! don't use white spaces
#endif

//windows code
#ifdef compile_for_windows
#define cns_open_editor "open_editor.bat"		// !! don't use white spaces
#endif


#define cns_beep_cmd "beep"
#define cn_beep_freq 7000
#define cn_beep_delay 200
#define cn_beep_repetition 2




enum en_options
{
eo_cycle = 1,							//must be 'orable' vals
eo_beep = 2,
eo_win_to_front = 4,
eo_run_cmd = 8,
};



struct st_time_tag
{
int time;								//0x235959 is displayed as 23:59:59, 23h  59min  59sec
int interval_time;

int state;								//0=stopped, 1=countdown, 2=pause
bool cycle;

int options;							//see: en_options

int beeps;
int pitch;
};










