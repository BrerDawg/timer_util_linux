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

//button_mouse_click.h

//v1.01

#ifndef button_mouse_click_h
#define button_mouse_click_h




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
#include <math.h>
#include <complex>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
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




using namespace std;






class button_mouse_click : public Fl_Button
{
private:										//private var



public:											//public var
int left_button;								//this flag NEEDS to be cleared in your callback, and NOT in the 'handle()' function of this class
int right_button;								//this flag NEEDS to be cleared in your callback, and NOT in the 'handle()' function of this class
int ctrl_key, shift_key;
string stip_static;

public:											//public functions
button_mouse_click( int xx, int yy, int wid, int hei, const char *label = 0 );
~button_mouse_click();

private:										//private functions
int handle( int );

};



#endif

