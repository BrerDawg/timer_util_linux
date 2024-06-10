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




//timer_util_linux.cpp

//v1.01		07-mar-2019			//timer utility
//v1.02		18-may-2019			//fixed 'bt_start1' and 'bt_start2' start/stop button tallies
//v1.03		26-feb-2021			//added eta display boxes, added 'bx_ctrl_minus'
//v1.04		29-jun-2021			//added toggle button 'bt_minus'
//v1.05		29-jun-2021			//fixed start button clearing current count down time, see cb_bt_start()
//v1.06		26-apr-2024			//added right button click subtract feature, see class 'button_mouse_click', added trim feature for 'remaining time', see 'cb_bt_trim()'
//v1.07		09-jun-2024			//added 'do_beep_by_idx()' for 'cb_bt_test()' to use

#include "timer_util_linux.h"



//asm("int3");						//usefull to trigger debugger






//global objs


//global vars
string csIniFilename;
string slast_filename;

int iBorderWidth;
int iBorderHeight;
string app_path;						//path this app resides on
string dir_seperator="\\";				//assume dos\windows folder directory seperator

int font_num = cnFontEditor;
int font_size = cnSizeEditor;
unsigned long long int ns_tim_start1;	//a ns timer start val


int iAppExistDontRun = 1;	//if set, app probably exist and user doesn't want to run another
							//set it to assume we don't want to run another incase user closes
							//wnd without pressing either 'Run Anyway' or 'Don't Run, Exit' buttons


#ifndef compile_for_windows
Display *gDisp;
#endif



int modulo_sec = 4;
int modulo_sec_cnt = 0;

int modulo_flash = 2;
int modulo_flash_cnt = 0;

char sz_tooltip0[] = "right click to subtract";							//v1.06
char sz_tooltip1[] = "trim remaining time (mins)";								//v1.06



vector<st_time_tag> vtime;


//function prototypes
void LoadSettings(string csIniFilename);
void SaveSettings(string csIniFilename);
int CheckInstanceExists(string csAppName);
int RunShell( string sin );
bool my_file_chooser( string &picked, const char* title, const char* pat, const char* start_path_filename, int type, Fl_Font fnt = -1, int fntsze = -1 );




//callbacks
void cb_btAbout(Fl_Widget *, void *);
void cb_btQuit(Fl_Widget *, void *);
void cb_timer1(void *);

























//----------------------------------------------------------------------

vector<st_fluidfl_extract> vfluidfl;		//refer: 'fluidfl_extract_dimensions()'
bool extracted_fluidfl = 0;

int fluidfl_depth = -1;



//build a vector of all wdg in supplied window, WARNING this code is reentrant, only run it once for each 'vv'(does not clear vv), see above usage comments
void fluidfl_extract_dimensions( Fl_Group *o, vector<st_fluidfl_extract> &vv, bool dump_printf = 0 )
{

st_fluidfl_extract ooo;

bool can_have_children = 0;
//try and work out what the pointer is in terms of fltk class objs by using 'dynamic_cast'
//refer https://en.wikibooks.org/wiki/C%2B%2B_Programming/RTTI

Fl_Window* ptr_match0 = dynamic_cast<Fl_Window*>(o);
if( ptr_match0 )
	{
//	printf("Fl_Window\n" );
	can_have_children = 1;
	}


Fl_Group* ptr_match1 = dynamic_cast<Fl_Group*>(o);
if( ptr_match1 )
	{
//	printf("Fl_Group\n" );
	can_have_children = 1;
	}


Fl_Scroll* ptr_match2 = dynamic_cast<Fl_Scroll*>(o);
if( ptr_match2 )
	{
//	printf("Fl_Scroll\n" );
	can_have_children = 1;
	}


mystr m1;
m1 = (char*)"";
string sindent;
m1.padstrchar( sindent, ' ', fluidfl_depth, 100 );

if( can_have_children ) 
	{
	fluidfl_depth++;
	m1 = (char*)"";
	m1.padstrchar( sindent, ' ', fluidfl_depth*4, 100 );
	if( dump_printf ) printf( "extract_fluidfl_dimensions() depth: %02d - %s grp: ( %d, %d, %d, %d, \"%s\" );\n", fluidfl_depth, sindent.c_str(), o->x(), o->y(), o->w(), o->h(), o->label() );

	ooo.wdg = o;
	ooo.label = (char*)"";
	if( o->label() != 0 ) ooo.label = o->label();
	ooo.label = o->label();
	ooo.x = o->x();
	ooo.y = o->y();
	ooo.w = o->w();
	ooo.h = o->h();
	vv.push_back(ooo);
	}
else{
//	printf( "extract_fluidfl_dimensions() depth: %02d - %s  wdg: ( %d, %d, %d, %d, \"%s\" );\n", fluidfl_depth, sindent.c_str(), o->x(), o->y(), o->w(), o->h() );
	}

if( can_have_children )
	{
	int cnt = o->children();
	if( cnt > 0 ) 
		{
//		printf( "cnt: %d\n", cnt );
		for( int i = 0; i < cnt; i++ )
			{
			Fl_Widget *grp = o->child( i );
			bool can_have_children2 = 0;
			//try and work out what the pointer is in terms of fltk class objs by using 'dynamic_cast'
			//https://en.wikibooks.org/wiki/C%2B%2B_Programming/RTTI
			
			Fl_Window* ptr_match0 = dynamic_cast<Fl_Window*>(grp);	//test if this a Fl_Window
			if( ptr_match0 )											//is test correct? 
				{
//				printf("Fl_Window\n" );
				can_have_children2 = 1;
				}
			
			
			Fl_Group* ptr_match1 = dynamic_cast<Fl_Group*>(grp);
			if( ptr_match1 )
				{
//				printf("Fl_Group\n" );
				can_have_children2 = 1;
				}
			
			
			Fl_Scroll* ptr_match2 = dynamic_cast<Fl_Scroll*>(grp);
			if( ptr_match2 )
				{
//				printf("Fl_Scroll\n" );
				can_have_children2 = 1;
				}
			
			

			if( can_have_children2 )
				{	
//				printf( "try reenter - extract_fluidfl_dimensions(depth: %d) - wdj: '%s', x: %d, y: %d, w: %d, h: %d, %d, %d, %d, %d\n", fluidfl_depth, grp->label(), grp->x(), grp->y(), grp->w(), grp->h(), grp->x(), grp->y(), grp->w(), grp->h() );
				fluidfl_extract_dimensions( (Fl_Group *)grp, vv );
				}
			else{
				Fl_Widget *wdg = o->child( i );
				if( dump_printf ) printf( "extract_fluidfl_dimensions() depth: %02d - %s  wdg: ( %d, %d, %d, %d, \"%s\" );\n", fluidfl_depth, sindent.c_str(), wdg->x(), wdg->y(), wdg->w(), wdg->h(), wdg->label() );

				ooo.wdg = wdg;
				ooo.label = "";
				if( wdg->label() != 0 ) ooo.label = wdg->label();
				ooo.x = wdg->x();
				ooo.y = wdg->y();
				ooo.w = wdg->w();
				ooo.h = wdg->h();
				vv.push_back(ooo);
				}
			}
		fluidfl_depth--;
		}		
	}
else{
	//never gets here
//	printf( "extract_fluidfl_dimensions() depth: %02d - no childs - %s  wdg: ( %d, %d, %d, %d, \"%s\" );\n", fluidfl_depth, sindent.c_str(), o->x(), o->y(), o->w(), o->h(), o->label() );
	}
}



//find spec wdg with matching 'label' and collect its dimensions, see above usage comments for 'extract_fluidfl_dimensions()'
//'idx' is set to the index in 'vv', with this you can access: 'vv.wdg'
bool fluidfl_find_dimensions( vector<st_fluidfl_extract> vv, string label, int &idx, int &xx, int &yy, int &ww, int &hh )
{
for( int i = 0; i < vv.size(); i++ )
	{
	if( vv[ i ].label.compare(label) == 0 )
		{
		idx = i;
		xx = vv[ i ].x;
		yy = vv[ i ].y;
		ww = vv[ i ].w;
		hh = vv[ i ].h;
		return 1;
		}
	}
printf("fluidfl_find_dimensions() - !!! failed to find fluid.fl wdg: '%s', exiting, needs to be fixed !!!\n", label.c_str() );
getchar();
exit(0);
return 0;
}

//----------------------------------------------------------------------







//----------------------------------------------------------------------

fluid_wnd::fluid_wnd( int xx, int yy, int wid, int hei, const char *label ) : Fl_Double_Window( xx, yy, wid ,hei, label )
{
ctrl_key = 0;
}




int fluid_wnd::handle( int e )
{
bool need_redraw = 0;
bool dont_pass_on = 0;


#ifdef compile_for_windows 
bDonePaste = 1;					//does not seem to be required for Windows, so nullify by setting to 1
#endif

if ( e == FL_KEYDOWN )
	{
	int key = Fl::event_key();
	
	if( ( key == FL_Control_L ) || ( key == FL_Control_R ) ) ctrl_key = 1;

if( ctrl_key == 1 ) 
	{
	bx_ctrl_minus->labelcolor(FL_RED);
	bt_minus->value( 1 );												//v1.04
	}

	need_redraw = 1;
	dont_pass_on = 1;
	}


if ( e == FL_KEYUP )
	{
	int key = Fl::event_key();
	
	if( ( key == FL_Control_L ) || ( key == FL_Control_R ) ) ctrl_key = 0;

if( ctrl_key == 0 ) 
	{
	bx_ctrl_minus->labelcolor(FL_BLACK);
	bt_minus->value( 0 );												//v1.04
	}

	need_redraw = 1;
	dont_pass_on = 1;
	}


if ( e == FL_MOUSEWHEEL )
	{
	mousewheel = Fl::event_dy();
	
	need_redraw = 1;
	dont_pass_on = 1;
	}
	
if ( need_redraw ) redraw();

if( dont_pass_on ) return 1;

return Fl_Double_Window::handle(e);
}

//----------------------------------------------------------------------



int get_sel()
{
if( bt_sel0->value() ) return 0;	
if( bt_sel1->value() ) return 1;	
if( bt_sel2->value() ) return 2;	
return 0;
}




int get_secs( int idx, bool interval_time )
{
if( idx >= vtime.size() ) return 0;
if( idx < 0 ) return 0;

st_time_tag o = vtime[idx];
string s1;

if( interval_time ) strpf( s1, "%02x", (o.interval_time) & 0xff );
else strpf( s1, "%02x", (o.time) & 0xff );

int iv;
sscanf( s1.c_str(), "%d", &iv );

return iv;
}






int get_mins( int idx, bool interval_time )
{
if( idx >= vtime.size() ) return 0;
if( idx < 0 ) return 0;

st_time_tag o = vtime[idx];
string s1;
if( interval_time ) strpf( s1, "%02x", (o.interval_time >> 8) & 0xff );
else strpf( s1, "%02x", (o.time >> 8) & 0xff );

int iv;
sscanf( s1.c_str(), "%d", &iv );

return iv;
}






int get_hrs( int idx, bool interval_time )
{
if( idx >= vtime.size() ) return 0;
if( idx < 0 ) return 0;

st_time_tag o = vtime[idx];
string s1;
if( interval_time ) strpf( s1, "%02x", (o.interval_time >> 16) & 0xff );
else strpf( s1, "%02x", (o.time >> 16) & 0xff );

int iv;
sscanf( s1.c_str(), "%d", &iv );

return iv;
}




void set_secs( int idx, bool interval_time, int secs )
{
if( secs < 0 ) secs = 0;
if( secs > 59 ) secs = 59;

if( idx >= vtime.size() ) return;
if( idx < 0 ) return;

st_time_tag o = vtime[idx];


string s1;
strpf( s1, "%d", secs );

int iv;
sscanf( s1.c_str(), "%x", &iv );

if( interval_time )
	{
	o.interval_time &= 0xffff00;
	o.interval_time |= iv;
	}
else{
	o.time &= 0xffff00;
	o.time |= iv;
	}

vtime[idx] = o;
}




void set_mins( int idx, bool interval_time, int mins )
{
if( mins < 0 ) mins = 0;
if( mins > 59 ) mins = 59;

if( idx >= vtime.size() ) return;
if( idx < 0 ) return;

st_time_tag o = vtime[idx];


string s1;
strpf( s1, "%d", mins );

int iv;
sscanf( s1.c_str(), "%x", &iv );

if( interval_time )
	{
	o.interval_time &= 0xff00ff;
	o.interval_time |= ( iv << 8 );
	}
else{
	o.time &= 0xff00ff;
	o.time |= ( iv << 8 );
	}

vtime[idx] = o;
}







void set_hrs( int idx, bool interval_time, int hrs )
{
if( hrs < 0 ) hrs = 0;
if( hrs > 99 ) hrs = 99;

if( idx >= vtime.size() ) return;
if( idx < 0 ) return;

st_time_tag o = vtime[idx];


string s1;
strpf( s1, "%d", hrs );

int iv;
sscanf( s1.c_str(), "%x", &iv );

if( interval_time )
	{
	o.interval_time &= 0x00ffff;
	o.interval_time |= ( iv << 16 );
	}
else{
	o.time &= 0x00ffff;
	o.time |= ( iv << 16 );
	}

vtime[idx] = o;
}




// +/- 59 sec max/min
void add_to_time( int idx, bool interval_time, int seconds )
{
if( seconds > 59 ) seconds = 59;
if( seconds < -59 ) seconds = -59;

if( idx >= vtime.size() ) return;
if( idx < 0 ) return;

int secs = get_secs( idx, interval_time );

secs += seconds;
set_secs( idx, interval_time, secs );


if( seconds > 0 )
	{
	if( secs > 59 )
		{
		secs -= 60;
		set_secs( idx, interval_time, secs );
		
		int mins = get_mins( idx, interval_time );	
		mins += 1;	
		set_mins( idx, interval_time, mins );

		if( mins > 59 )
			{
			set_mins( idx, interval_time, 0 );

			int hrs = get_hrs( idx, interval_time );
			hrs += 1;
			set_hrs( idx, interval_time, hrs );

			if( hrs > 99 )
				{
				set_hrs( idx, interval_time, 0 );
				}
			}
		
		}
	}
else{
	if( seconds < 0 )
		{
		if( secs < 0 )
			{
			secs += 60;
			set_secs( idx, interval_time, secs );
			
			int mins = get_mins( idx, interval_time );	
			mins -= 1;	
			set_mins( idx, interval_time, mins );

			if( mins < 0 )
				{
				set_mins( idx, interval_time, 59 );

				int hrs = get_hrs( idx, interval_time );
				hrs -= 1;
				set_hrs( idx, interval_time, hrs );

				if( hrs < 0 )
					{
					set_hrs( idx, interval_time, 99 );
					}
				}
			
			}
		}
	}

}








bool check_legal_time( int idx, bool interval_time )
{
if( idx >= vtime.size() ) return 0;
if( idx < 0 ) return 0;

st_time_tag o = vtime[idx];

int secs = get_secs( idx, interval_time );
int mins = get_mins( idx, interval_time );
int hrs = get_hrs( idx, interval_time );

if( secs > 59 ) return 0;
if( mins > 59 ) return 0;
if( hrs > 99 ) return 0;

return 1;
}






bool check_time_period_ended( int idx ) 
{
if( idx >= vtime.size() ) return 0;
if( idx < 0 ) return 0;

//st_time_tag o = vtime[idx];

int secs = get_secs( idx, 0 );
int mins = get_mins( idx, 0 );
int hrs = get_hrs( idx, 0 );

if( ( secs == 0 ) && ( mins == 0 ) && ( hrs == 0 ) )
	{
	return 1;	
	}

return 0;
}










//add 2 time periods to get a resultant 3rd time period
void time_offset( int ihrs0, int imins0, int isecs0, int ihrs1, int imins1, int isecs1, int &ihrs2, int &imins2, int &isecs2 )
{

int time_in_secs0 = ihrs0*3600 + imins0*60 + isecs0;
int time_in_secs1 = ihrs1*3600 + imins1*60 + isecs1;

int summed_secs = time_in_secs0 + time_in_secs1;

ihrs2 = summed_secs / 3600;
imins2 = (summed_secs - ihrs2 * 3600) / 60;
isecs2 = summed_secs - ihrs2 * 3600 - imins2 * 60;

}










void update_controls( bool all )
{
string s1, s2, stime_now, srem_time, sinterval_time;
mystr m1;

int idx = 0;

struct tm tt;
m1.get_time_now( tt );
strpf( s1, "%02d:",  tt.tm_hour );
stime_now += s1;
strpf( s1, "%02d:",  tt.tm_min );
stime_now += s1;
strpf( s1, "%02d",  tt.tm_sec );
stime_now += s1;

//int real_time_in_secs = tt.tm_hour*3600 + tt.tm_min*60 + tt.tm_sec;

fi_time_now->value( stime_now.c_str() );


for( int i = 0; i < vtime.size(); i++ )
	{
	st_time_tag o = vtime[i];

	srem_time = "";
	sinterval_time = "";
	
	strpf( s1, "%02x:", (o.time>>16) & 0xff );
	srem_time += s1;
	strpf( s1, "%02x:", (o.time>>8) & 0xff );
	srem_time += s1;
	strpf( s1, "%02x", (o.time) & 0xff );
	srem_time += s1;

	strpf( s1, "%02x:", (o.interval_time>>16) & 0xff );
	sinterval_time += s1;
	strpf( s1, "%02x:", (o.interval_time>>8) & 0xff );
	sinterval_time += s1;
	strpf( s1, "%02x", (o.interval_time) & 0xff );
	sinterval_time += s1;



	if( i == 0 ) 
		{
		Fl_Color col = fl_rgb_color( 0, 0, 0 );
		if( !check_legal_time( i, 1 ) ) col = fl_rgb_color( 255, 0, 0 );
		fi_interval_tim0->textcolor( col );
		fi_interval_tim0->value( sinterval_time.c_str() );
		fi_interval_tim0->redraw();

		col = fl_rgb_color( 255, 255, 255 );
		if( ( o.state ) & ( modulo_flash_cnt ) )  col = fl_rgb_color( 0, 255, 0 );
		fi_remaining_tim0->color( col );

		fi_remaining_tim0->value( srem_time.c_str() );
		fi_remaining_tim0->redraw();

		if( all )
			{
			if( o.options & eo_cycle ) ck_cycle0->value( 1 );
			else ck_cycle0->value( 0 );

			if( o.options & eo_beep ) ck_beep0->value( 1 );
			else ck_beep0->value( 0 );

			if( o.options & eo_win_to_front ) ck_wintop0->value( 1 );
			else ck_wintop0->value( 0 );

			if( o.options & eo_run_cmd ) ck_cmd_on0->value( 1 );
			else ck_cmd_on0->value( 0 );

//			fi_cmd0->value( vtime[i].cmd.c_str() );
			}

		if( vtime[i].state )
			{
			bt_start0->label("stop");	
			}
		else{
			bt_start0->label("start");	
			}
		
		
		//eta: show the real time that time out will occur at

		//-----
		int timer_hrs = (o.interval_time>>16) & 0xff;
		int timer_mins = (o.interval_time>>8) & 0xff;
		int timer_secs = (o.interval_time) & 0xff;

		strpf( s1, "%x", timer_hrs );		
		sscanf( s1.c_str(), "%d", &timer_hrs );

		strpf( s1, "%x", timer_mins );		
		sscanf( s1.c_str(), "%d", &timer_mins );

		strpf( s1, "%x", timer_secs );		
		sscanf( s1.c_str(), "%d", &timer_secs );

		int ihrs;
		int imins;
		int isecs;
		time_offset( tt.tm_hour, tt.tm_min, tt.tm_sec, timer_hrs, timer_mins, timer_secs, ihrs, imins, isecs );

		s2 = "";
		strpf( s1, "eta: %02d:", ihrs );
		s2 += s1;
		strpf( s1, "%02d:", imins );
		s2 += s1;
		strpf( s1, "%02d", isecs );
		s2 += s1;
		bx_interval_eta0->copy_label( s2.c_str() );
//		bx_eta0->redraw();
		//-----


		//-----
		timer_hrs = (o.time>>16) & 0xff;
		timer_mins = (o.time>>8) & 0xff;
		timer_secs = (o.time) & 0xff;

		strpf( s1, "%x", timer_hrs );		
		sscanf( s1.c_str(), "%d", &timer_hrs );

		strpf( s1, "%x", timer_mins );		
		sscanf( s1.c_str(), "%d", &timer_mins );

		strpf( s1, "%x", timer_secs );		
		sscanf( s1.c_str(), "%d", &timer_secs );

		time_offset( tt.tm_hour, tt.tm_min, tt.tm_sec, timer_hrs, timer_mins, timer_secs, ihrs, imins, isecs );

		if( o.state == 0 ) 
			{
			ihrs = imins = isecs = 0;	
			}
//int real_time_in_secs = tt.tm_hour*3600 + tt.tm_min*60 + tt.tm_sec;
		
//		int time_out_in_secs = timer_hrs*3600 + timer_mins*60 + timer_secs;

//		int offset_secs = real_time_in_secs + time_out_in_secs;
		

		s2 = "";
		strpf( s1, "eta: %02d:", ihrs%24 );
		s2 += s1;
		strpf( s1, "%02d:", imins );
		s2 += s1;
		strpf( s1, "%02d", isecs );
		s2 += s1;
		bx_eta0->copy_label( s2.c_str() );
//		bx_eta0->redraw();
		//-----
		}
		

	if( i == 1 ) 
		{
		Fl_Color col = fl_rgb_color( 0, 0, 0 );
		if( !check_legal_time( i, 1 ) ) col = fl_rgb_color( 255, 0, 0 );
		fi_interval_tim1->textcolor( col );
		fi_interval_tim1->value( sinterval_time.c_str() );
		fi_interval_tim1->redraw();

		col = fl_rgb_color( 255, 255, 255 );
		if( ( o.state ) & ( modulo_flash_cnt ) )  col = fl_rgb_color( 0, 255, 0 );
		fi_remaining_tim1->color( col );

		fi_remaining_tim1->value( srem_time.c_str() );
		fi_remaining_tim1->redraw();

		if( all )
			{
			if( o.options & eo_cycle ) ck_cycle1->value( 1 );
			else ck_cycle1->value( 0 );

			if( o.options & eo_beep ) ck_beep1->value( 1 );
			else ck_beep1->value( 0 );

			if( o.options & eo_win_to_front ) ck_wintop1->value( 1 );
			else ck_wintop1->value( 0 );

			if( o.options & eo_run_cmd ) ck_cmd_on1->value( 1 );
			else ck_cmd_on1->value( 0 );

//			fi_cmd0->value( vtime[i].cmd.c_str() );
			}

		if( vtime[i].state )
			{
			bt_start1->label("stop");	
			}
		else{
			bt_start1->label("start");	
			}

		//eta: show the real time that time out will occur at

		//-----
		int timer_hrs = (o.interval_time>>16) & 0xff;
		int timer_mins = (o.interval_time>>8) & 0xff;
		int timer_secs = (o.interval_time) & 0xff;

		strpf( s1, "%x", timer_hrs );		
		sscanf( s1.c_str(), "%d", &timer_hrs );

		strpf( s1, "%x", timer_mins );		
		sscanf( s1.c_str(), "%d", &timer_mins );

		strpf( s1, "%x", timer_secs );		
		sscanf( s1.c_str(), "%d", &timer_secs );

		int ihrs;
		int imins;
		int isecs;
		time_offset( tt.tm_hour, tt.tm_min, tt.tm_sec, timer_hrs, timer_mins, timer_secs, ihrs, imins, isecs );

		s2 = "";
		strpf( s1, "eta: %02d:", ihrs%24 );
		s2 += s1;
		strpf( s1, "%02d:", imins );
		s2 += s1;
		strpf( s1, "%02d", isecs );
		s2 += s1;
		bx_interval_eta1->copy_label( s2.c_str() );
//		bx_eta0->redraw();
		//-----


		//-----
		timer_hrs = (o.time>>16) & 0xff;
		timer_mins = (o.time>>8) & 0xff;
		timer_secs = (o.time) & 0xff;

		strpf( s1, "%x", timer_hrs );		
		sscanf( s1.c_str(), "%d", &timer_hrs );

		strpf( s1, "%x", timer_mins );		
		sscanf( s1.c_str(), "%d", &timer_mins );

		strpf( s1, "%x", timer_secs );		
		sscanf( s1.c_str(), "%d", &timer_secs );

		time_offset( tt.tm_hour, tt.tm_min, tt.tm_sec, timer_hrs, timer_mins, timer_secs, ihrs, imins, isecs );

		if( o.state == 0 ) 
			{
			ihrs = imins = isecs = 0;	
			}
		
		
		s2 = "";
		strpf( s1, "eta: %02d:", ihrs%24 );
		s2 += s1;
		strpf( s1, "%02d:", imins );
		s2 += s1;
		strpf( s1, "%02d", isecs );
		s2 += s1;
		bx_eta1->copy_label( s2.c_str() );
		bx_eta1->redraw();
		//-----
		}



	if( i == 2 ) 
		{
		Fl_Color col = fl_rgb_color( 0, 0, 0 );
		if( !check_legal_time( i, 1 ) ) col = fl_rgb_color( 255, 0, 0 );
		fi_interval_tim2->textcolor( col );
		fi_interval_tim2->value( sinterval_time.c_str() );
		fi_interval_tim2->redraw();

		col = fl_rgb_color( 255, 255, 255 );
		if( ( o.state ) & ( modulo_flash_cnt ) )  col = fl_rgb_color( 0, 255, 0 );
		fi_remaining_tim2->color( col );

		fi_remaining_tim2->value( srem_time.c_str() );
		fi_remaining_tim2->redraw();

		if( all )
			{
			if( o.options & eo_cycle ) ck_cycle2->value( 1 );
			else ck_cycle2->value( 0 );

			if( o.options & eo_beep ) ck_beep2->value( 1 );
			else ck_beep2->value( 0 );

			if( o.options & eo_win_to_front ) ck_wintop2->value( 1 );
			else ck_wintop2->value( 0 );

			if( o.options & eo_run_cmd ) ck_cmd_on2->value( 1 );
			else ck_cmd_on2->value( 0 );

//			fi_cmd0->value( vtime[i].cmd.c_str() );
			}

		if( vtime[i].state )
			{
			bt_start2->label("stop");	
			}
		else{
			bt_start2->label("start");	
			}

		//eta: show the real time that time out will occur at

		//-----
		int timer_hrs = (o.interval_time>>16) & 0xff;
		int timer_mins = (o.interval_time>>8) & 0xff;
		int timer_secs = (o.interval_time) & 0xff;

		strpf( s1, "%x", timer_hrs );		
		sscanf( s1.c_str(), "%d", &timer_hrs );

		strpf( s1, "%x", timer_mins );		
		sscanf( s1.c_str(), "%d", &timer_mins );

		strpf( s1, "%x", timer_secs );		
		sscanf( s1.c_str(), "%d", &timer_secs );

		int ihrs;
		int imins;
		int isecs;
		time_offset( tt.tm_hour, tt.tm_min, tt.tm_sec, timer_hrs, timer_mins, timer_secs, ihrs, imins, isecs );

		s2 = "";
		strpf( s1, "eta: %02d:", ihrs%24 );
		s2 += s1;
		strpf( s1, "%02d:", imins );
		s2 += s1;
		strpf( s1, "%02d", isecs );
		s2 += s1;
		bx_interval_eta2->copy_label( s2.c_str() );
//		bx_eta0->redraw();
		//-----


		//-----
		timer_hrs = (o.time>>16) & 0xff;
		timer_mins = (o.time>>8) & 0xff;
		timer_secs = (o.time) & 0xff;

		strpf( s1, "%x", timer_hrs );		
		sscanf( s1.c_str(), "%d", &timer_hrs );

		strpf( s1, "%x", timer_mins );		
		sscanf( s1.c_str(), "%d", &timer_mins );

		strpf( s1, "%x", timer_secs );		
		sscanf( s1.c_str(), "%d", &timer_secs );

		time_offset( tt.tm_hour, tt.tm_min, tt.tm_sec, timer_hrs, timer_mins, timer_secs, ihrs, imins, isecs );

		if( o.state == 0 ) 
			{
			ihrs = imins = isecs = 0;	
			}
		
		
		s2 = "";
		strpf( s1, "eta: %02d:", ihrs%24 );
		s2 += s1;
		strpf( s1, "%02d:", imins );
		s2 += s1;
		strpf( s1, "%02d", isecs );
		s2 += s1;
		bx_eta2->copy_label( s2.c_str() );
		bx_eta2->redraw();
		//-----
		}

	}




}











void do_beep( int freq, int delay, int count )
{

string s1;

strpf( s1, " %s -f %d -d %d -r %d", cns_beep_cmd, freq, delay, count );
	 
RunShell( s1 );
}






void cb_bt_start(Fl_Widget *w, void *v)
{
string s1;

int ii = (intptr_t)v;

int secs = get_secs( ii, 0 );											//v1.05
int mins = get_mins( ii, 0 );											//v1.05
int hrs = get_hrs( ii, 0 );												//v1.05

if( ( secs == 0 ) && ( mins == 0 ) && ( hrs == 0 ) )
	{
//	printf(" cb_bt_start() %d\n", ii );
	vtime[ii].time = vtime[ii].interval_time;							//v1.05
	}
else{
	if(	vtime[ii].state == 0 )											//v1.05, stopped?
		{
		if( vtime[ii].time != vtime[ii].interval_time )					//cur time not matching interval time
			{
			strpf( s1, "Note: there is still some countdown time left, will just unpause and continue, hit Reset otherwise.\n" );
			fl_alert( s1.c_str(), 0 );
			}
		}
	}

vtime[ii].state = !vtime[ii].state;

//if( vtime[ii].state ) vtime[ii].time = vtime[ii].interval_time;		//v1.05
}






void cb_bt_reset(Fl_Widget *w, void *v)
{
int ii = (intptr_t)v;

vtime[ii].time = vtime[ii].interval_time;

}







bool do_beep_by_idx( unsigned int idx )
{
string s1;

if( idx >= vtime.size() ) return 0;

if( idx == 0 )
	{
	s1 = fi_beeps0->value();
	int beeps;
	sscanf( s1.c_str(), "%d", &beeps );
	
	if( beeps < 0 ) beeps = 0;
	if( beeps > 60 ) beeps = 60;


	s1 = fi_pitch0->value();
	int pitch;
	sscanf( s1.c_str(), "%d", &pitch );
	
	if( pitch < 100 ) pitch = 100;
	if( pitch > 11000 ) pitch = 11000;

	if( vtime[idx].options & eo_beep ) do_beep( pitch, cn_beep_delay, beeps );
	}


if( idx == 1 )
	{
	s1 = fi_beeps1->value();
	int beeps;
	sscanf( s1.c_str(), "%d", &beeps );
	
	if( beeps < 0 ) beeps = 0;
	if( beeps > 60 ) beeps = 60;


	s1 = fi_pitch1->value();
	int pitch;
	sscanf( s1.c_str(), "%d", &pitch );
	
	if( pitch < 100 ) pitch = 100;
	if( pitch > 11000 ) pitch = 11000;

	if( vtime[idx].options & eo_beep ) do_beep( pitch, cn_beep_delay, beeps );
	}


if( idx == 2 )
	{
	s1 = fi_beeps2->value();
	int beeps;
	sscanf( s1.c_str(), "%d", &beeps );
	
	if( beeps < 0 ) beeps = 0;
	if( beeps > 60 ) beeps = 60;


	s1 = fi_pitch2->value();
	int pitch;
	sscanf( s1.c_str(), "%d", &pitch );
	
	if( pitch < 100 ) pitch = 100;
	if( pitch > 11000 ) pitch = 11000;

	if( vtime[idx].options & eo_beep ) do_beep( pitch, cn_beep_delay, beeps );
	}

return 1;
}






void cb_bt_test(Fl_Widget *w, void *v)
{
int ii = (intptr_t)v;
Fl_Input* wdg = (Fl_Input* )w;

if( ii == 0 ) { RunShell( fi_cmd0->value() );  do_beep_by_idx( 0 ); }	//v1.07
if( ii == 1 ) { RunShell( fi_cmd1->value() );  do_beep_by_idx( 1 ); }	//v1.07
if( ii == 2 ) { RunShell( fi_cmd2->value() );  do_beep_by_idx( 2 ); }	//v1.07

}



void cb_bt_sel(Fl_Widget *w, void *v)
{
int ii = (intptr_t)v;

//Fl_Round_Button* wdg = (Fl_Round_Button* )w;

if( ii == 0 ) 
	{
	bt_sel0->value(1);	
	bt_sel1->value(0);	
	bt_sel2->value(0);	
	}

if( ii == 1 ) 
	{
	bt_sel0->value(0);
	bt_sel1->value(1);	
	bt_sel2->value(0);	
	}
	
if( ii == 2 ) 
	{
	bt_sel0->value(0);
	bt_sel1->value(0);	
	bt_sel2->value(1);	
	}

}





void cb_bt_cycle(Fl_Widget *w, void *v)
{
int ii = (intptr_t)v;

Fl_Check_Button* wdg = (Fl_Check_Button* )w;

st_time_tag o = vtime[ii];

printf( "o.options 1: %d\n",  o.options );

if( wdg->value() ) o.options |= eo_cycle;
else o.options &= ~eo_cycle;

printf( "o.options 2: %d\n",  o.options );

vtime[ii] = o;
}






void cb_ck_beep(Fl_Widget *w, void *v)
{
int ii = (intptr_t)v;

Fl_Check_Button* wdg = (Fl_Check_Button* )w;

st_time_tag o = vtime[ii];

if( wdg->value() ) o.options |= eo_beep;
else o.options &= ~eo_beep;

vtime[ii] = o;
}




void cb_bt_wintop(Fl_Widget *w, void *v)
{
int ii = (intptr_t)v;

Fl_Check_Button* wdg = (Fl_Check_Button* )w;

st_time_tag o = vtime[ii];

if( wdg->value() ) o.options |= eo_win_to_front;
else o.options &= ~eo_win_to_front;

vtime[ii] = o;
}





void cb_cmd_on0(Fl_Widget *w, void *v)
{
int ii = (intptr_t)v;

Fl_Check_Button* wdg = (Fl_Check_Button* )w;

st_time_tag o = vtime[ii];

if( wdg->value() ) o.options |= eo_run_cmd;
else o.options &= ~eo_run_cmd;

vtime[ii] = o;
}


/*
//beep -f $3 -d $4 -r $5
void cb_fi_tim0(Fl_Widget *, void *)
{
printf( "cb_bt_button0() - gonna do shell cmd:  %s -f %d -d %d -r %d\n", cns_beep_cmd, cn_beep_freq, cn_beep_delay, cn_beep_repetition );

string s1;

strpf( s1, " %s -f %d -d %d -r %d", cns_beep_cmd, cn_beep_freq, cn_beep_delay, cn_beep_repetition );
	 
RunShell( s1 );
}
*/



void cb_bt_load(Fl_Widget *w, void *v)
{
string s1, s2;

s1 = slast_filename;

if( my_file_chooser( s2, "Load File?", "*", s1.c_str(), 0, font_num, font_size ) )
	{

	LoadSettings( s2 );
	update_controls( 1 );

	slast_filename = s2;							//load this after 'LoadSettings()' call just above as that call alters 'slast_filename'

    mystr m1 = s2;
    m1.ExtractFilename( dir_seperator[ 0 ], s2 );
    
	printf( "cb_bt_load() - '%s'\n", s2.c_str() );
	}
}





void cb_bt_save(Fl_Widget *w, void *v)
{
string s1, s2;

s1 = slast_filename;

if( my_file_chooser( s2, "Save File?", "*", s1.c_str(), Fl_File_Chooser::CREATE, font_num, font_size ) )
	{
	slast_filename = s2;
	
	SaveSettings( slast_filename );
	
    mystr m1 = s2;
    m1.ExtractFilename( dir_seperator[ 0 ], s2 );

	printf( "cb_bt_save() - '%s'\n", s2.c_str() );
	}
}












void cb_bt_trim(Fl_Widget *w, void *v)
{
int ii = (intptr_t)v;


if( w == bt_trim_01 ) cb_bt_sel( 0, 0 );
if( w == bt_trim01 ) cb_bt_sel( 0, 0 );

if( w == bt_trim_05 ) cb_bt_sel( 0, 0 );
if( w == bt_trim05 ) cb_bt_sel( 0, 0 );

if( w == bt_trim_11 ) cb_bt_sel( 0, (void*)1 );
if( w == bt_trim11 ) cb_bt_sel( 0, (void*)1 );

if( w == bt_trim_15 ) cb_bt_sel( 0, (void*)1 );
if( w == bt_trim15 ) cb_bt_sel( 0, (void*)1 );

if( w == bt_trim_21 ) cb_bt_sel( 0, (void*)2 );
if( w == bt_trim21 ) cb_bt_sel( 0, (void*)2 );

if( w == bt_trim_25 ) cb_bt_sel( 0, (void*)2 );
if( w == bt_trim25 ) cb_bt_sel( 0, (void*)2 );

int idx = get_sel();

int dir = 1;

if( ii < 0 ) dir = -1;

for( int i = 0; i < fabsf(ii*60); i++ )
	{
	add_to_time( idx, 0, dir );											//this call can only handle max of +/-59 secs, so repeatedly call it with 1 or -1
	}



printf( "cb_bt_trim() - ii %d\n" ,ii );

}












void cb_bt_dur(Fl_Widget *w, void *v)
{
int ii = (intptr_t)v;

int idx = get_sel();

bool bminus = 0;

if( wnd_fluid->ctrl_key ) bminus = 1;
if( bt_minus->value() )  bminus = 1;	//v1.04


button_mouse_click* o = (button_mouse_click*) w;

//printf( "cb_bt_dur() - dur %d, right_click %d\n" ,ii ,o->right_button );



if( o->right_button ) bminus = 1;		//v1.06


o->right_button = 0;													//need to clear this flag in callback


for( int i = 0; i < ii; i++ )
	{
	if( bminus )
		{
		add_to_time( idx, 1, -30 );				//adj interval_time
		add_to_time( idx, 1, -30 );
		}
	else{
		add_to_time( idx, 1, 30 );				//adj interval_time
		add_to_time( idx, 1, 30 );
		}
	}
}







void cb_bt_num(Fl_Widget *w, void *v)
{
int ii = (intptr_t)v;



//s1 += ii+0x30;

//fi_tim0->value( s1.c_str() );

int idx = 0;

if( bt_sel0->value() ) idx = 0;
if( bt_sel1->value() ) idx = 1;
if( bt_sel2->value() ) idx = 2;


st_time_tag o = vtime[idx];

if( ii != 10 )								//not a clear?
	{

	o.interval_time <<= 4;
	o.interval_time |= ii;
	}
else{
	o.interval_time = 0;
	}

//o.interval_time = 0x010000;
//o.interval_time = 59;
vtime[idx] = o;

//add_to_time( idx, 0, -1 );

//set_secs( idx, 12 );
//set_mins( idx, 34 );
//set_hrs( idx, 56 );

update_controls( 0 );
}



//v1.04
void cb_bt_minus(Fl_Widget *w, void *v)
{
if( bt_minus->value() ) { bx_ctrl_minus->labelcolor(FL_RED); bx_ctrl_minus->hide(); bx_ctrl_minus->show(); }		//need hide/show instead of redraw() to avoid bizzare embossing effect
else { bx_ctrl_minus->labelcolor(FL_BLACK); bx_ctrl_minus->redraw(); bx_ctrl_minus->hide(); bx_ctrl_minus->show(); }
}






void cb_bt_combo(Fl_Widget *w, void *v)
{
int ii = (intptr_t)v;

if( ii == 0 )					//about
	{
	cb_btAbout( 0, 0 );
	}
}








void make_fluid_ctrls()
{
int find_idx, ox, oy, ow, oh;												//these will each of the wdg dimensions extracted from 'fluid.fl' window
if( !extracted_fluidfl ) 										//one time reading of 'fluid.fl' window and its wdg, gathering dimensions, see: 'fluidfl_find_dimensions()'
	{
	extracted_fluidfl = 1;
	fluidfl_extract_dimensions( (Fl_Group*) wnd_fluid, vfluidfl );
	}


wnd_fluid->clear();

wnd_fluid->begin();

/*
fluidfl_find_dimensions( vfluidfl, "bt_button0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_button0 = new Fl_Button(ox, oy, ow, oh, "button0 beep");
bt_button0->labelsize( 12 );	
bt_button0->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_button0->callback( cb_bt_button0, (void*)0 );
*/



//-------
fluidfl_find_dimensions( vfluidfl, "interval_tim0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_interval_tim0 = new Fl_Input(ox, oy, ow, oh, "Time Interval");
fi_interval_tim0->labelsize( 9 );	
fi_interval_tim0->align( FL_ALIGN_TOP );			//FL_ALIGN_LEFT
//fi_interval_tim0->callback( cb_fi_tim0, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "remaining_tim0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_remaining_tim0 = new Fl_Input(ox, oy, ow, oh, "RemainingTime");
fi_remaining_tim0->labelsize( 9 );	
fi_remaining_tim0->align( FL_ALIGN_TOP );			//FL_ALIGN_LEFT
//fi_remaining_tim0->callback( cb_fi_tim0, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "start0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_start0 = new Fl_Button(ox, oy, ow, oh, "start");
bt_start0->labelsize( 11 );	
bt_start0->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_start0->callback( cb_bt_start, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "reset0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_reset0 = new Fl_Button(ox, oy, ow, oh, "reset");
bt_reset0->labelsize( 11 );	
//bt_reset0->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
bt_reset0->callback( cb_bt_reset, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "cycle0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
ck_cycle0 = new Fl_Check_Button(ox, oy, ow, oh, "cycle");
ck_cycle0->labelsize( 9 );	
//ck_cycle0->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
ck_cycle0->callback( cb_bt_cycle, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "beep0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
ck_beep0 = new Fl_Check_Button(ox, oy, ow, oh, "beep");
ck_beep0->labelsize( 9 );	
//ck_wintop0->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
ck_beep0->callback( cb_ck_beep, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "cmd_on0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
ck_cmd_on0 = new Fl_Check_Button(ox, oy, ow, oh, "cmd");
ck_cmd_on0->labelsize( 9 );	
//ck_wintop0->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
ck_cmd_on0->callback( cb_cmd_on0, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "cmd0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_cmd0 = new Fl_Input(ox, oy, ow, oh, "cmd");
fi_cmd0->labelsize( 12 );	
fi_cmd0->textsize( 10 );	
fi_cmd0->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
//fi_cmd0->callback( cb_fi_tim0, (void*)0 );
fi_cmd0->type(FL_MULTILINE_INPUT);
fi_cmd0->tooltip( "e.g: gnome-calculator &" );

fluidfl_find_dimensions( vfluidfl, "beeps0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_beeps0 = new Fl_Input(ox, oy, ow, oh, "beeps");
fi_beeps0->labelsize( 12 );	
fi_beeps0->textsize( 10 );	
fi_beeps0->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
fi_beeps0->type( FL_INT_INPUT );
//fi_beeps0->callback( cb_fi_tim0, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "pitch0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_pitch0 = new Fl_Input(ox, oy, ow, oh, "pitch");
fi_pitch0->labelsize( 12 );	
fi_pitch0->textsize( 10 );	
fi_pitch0->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
fi_pitch0->type( FL_INT_INPUT );
//fi_pitch0->callback( cb_fi_tim0, (void*)0 );




fluidfl_find_dimensions( vfluidfl, "wintop0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
ck_wintop0 = new Fl_Check_Button(ox, oy, ow, oh, "wintop");
ck_wintop0->labelsize( 9 );	
//ck_wintop0->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
ck_wintop0->callback( cb_bt_wintop, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "test0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_test0 = new Fl_Button(ox, oy, ow, oh, "test");
bt_test0->labelsize( 11 );	
bt_test0->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_test0->callback( cb_bt_test, (void*)0 );

fluidfl_find_dimensions( vfluidfl, "interval_eta0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bx_interval_eta0 = new Fl_Box(ox, oy, ow, oh, "interval_eta0");
bx_interval_eta0->labelsize( 11 );	
bx_interval_eta0->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT

fluidfl_find_dimensions( vfluidfl, "eta0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bx_eta0 = new Fl_Box(ox, oy, ow, oh, "eta0");
bx_eta0->labelsize( 11 );	
bx_eta0->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
//bx_eta0->callback( cb_bt_test, (void*)0 );
//-------



//-------
fluidfl_find_dimensions( vfluidfl, "interval_tim1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_interval_tim1 = new Fl_Input(ox, oy, ow, oh, "Time Interval");
fi_interval_tim1->labelsize( 9 );	
fi_interval_tim1->align( FL_ALIGN_TOP );			//FL_ALIGN_LEFT
//fi_interval_tim1->callback( cb_fi_tim0, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "remaining_tim1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_remaining_tim1 = new Fl_Input(ox, oy, ow, oh, "RemainingTime");
fi_remaining_tim1->labelsize( 9 );	
fi_remaining_tim1->align( FL_ALIGN_TOP );			//FL_ALIGN_LEFT
//fi_remaining_tim1->callback( cb_fi_tim0, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "start1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_start1 = new Fl_Button(ox, oy, ow, oh, "start");
bt_start1->labelsize( 11 );	
bt_start1->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_start1->callback( cb_bt_start, (void*)1 );


fluidfl_find_dimensions( vfluidfl, "reset1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_reset1 = new Fl_Button(ox, oy, ow, oh, "reset");
bt_reset1->labelsize( 11 );	
//bt_reset1->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
bt_reset1->callback( cb_bt_reset, (void*)1 );


fluidfl_find_dimensions( vfluidfl, "cycle1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
ck_cycle1 = new Fl_Check_Button(ox, oy, ow, oh, "cycle");
ck_cycle1->labelsize( 9 );	
//ck_cycle1->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
ck_cycle1->callback( cb_bt_cycle, (void*)1 );


fluidfl_find_dimensions( vfluidfl, "beep1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
ck_beep1 = new Fl_Check_Button(ox, oy, ow, oh, "beep");
ck_beep1->labelsize( 9 );	
//ck_beep1->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
ck_beep1->callback( cb_ck_beep, (void*)1 );

fluidfl_find_dimensions( vfluidfl, "cmd_on1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
ck_cmd_on1 = new Fl_Check_Button(ox, oy, ow, oh, "cmd");
ck_cmd_on1->labelsize( 9 );	
//ck_cmd_on1->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
ck_cmd_on1->callback( cb_cmd_on0, (void*)1 );


fluidfl_find_dimensions( vfluidfl, "cmd1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_cmd1 = new Fl_Input(ox, oy, ow, oh, "cmd");
fi_cmd1->labelsize( 12 );	
fi_cmd1->textsize( 10 );	
fi_cmd1->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
//fi_cmd1->callback( cb_fi_tim0, (void*)1 );
fi_cmd1->type(FL_MULTILINE_INPUT);
fi_cmd1->tooltip( "e.g: gnome-calculator &" );

fluidfl_find_dimensions( vfluidfl, "beeps1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_beeps1 = new Fl_Input(ox, oy, ow, oh, "beeps");
fi_beeps1->labelsize( 12 );	
fi_beeps1->textsize( 10 );	
fi_beeps1->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
fi_beeps1->type( FL_INT_INPUT );
//fi_beeps1->callback( cb_fi_tim0, (void*)1 );


fluidfl_find_dimensions( vfluidfl, "pitch1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_pitch1 = new Fl_Input(ox, oy, ow, oh, "pitch");
fi_pitch1->labelsize( 12 );	
fi_pitch1->textsize( 10 );	
fi_pitch1->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
fi_pitch1->type( FL_INT_INPUT );
//fi_pitch1->callback( cb_fi_tim0, (void*)1 );




fluidfl_find_dimensions( vfluidfl, "wintop1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
ck_wintop1 = new Fl_Check_Button(ox, oy, ow, oh, "wintop");
ck_wintop1->labelsize( 9 );	
//ck_wintop1->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
ck_wintop1->callback( cb_bt_wintop, (void*)1 );


fluidfl_find_dimensions( vfluidfl, "test1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_test1 = new Fl_Button(ox, oy, ow, oh, "test");
bt_test1->labelsize( 11 );	
bt_test1->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_test1->callback( cb_bt_test, (void*)1 );

fluidfl_find_dimensions( vfluidfl, "interval_eta1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bx_interval_eta1 = new Fl_Box(ox, oy, ow, oh, "interval_eta1");
bx_interval_eta1->labelsize( 11 );	
bx_interval_eta1->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT

fluidfl_find_dimensions( vfluidfl, "eta1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bx_eta1 = new Fl_Box(ox, oy, ow, oh, "eta1");
bx_eta1->labelsize( 11 );	
bx_eta1->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
//-------




//-------
fluidfl_find_dimensions( vfluidfl, "interval_tim2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_interval_tim2 = new Fl_Input(ox, oy, ow, oh, "Time Interval");
fi_interval_tim2->labelsize( 9 );	
fi_interval_tim2->align( FL_ALIGN_TOP );			//FL_ALIGN_LEFT
//fi_interval_tim2->callback( cb_fi_tim0, (void*)2 );


fluidfl_find_dimensions( vfluidfl, "remaining_tim2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_remaining_tim2 = new Fl_Input(ox, oy, ow, oh, "RemainingTime");
fi_remaining_tim2->labelsize( 9 );	
fi_remaining_tim2->align( FL_ALIGN_TOP );			//FL_ALIGN_LEFT
//fi_remaining_tim2->callback( cb_fi_tim0, (void*)2 );


fluidfl_find_dimensions( vfluidfl, "start2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_start2 = new Fl_Button(ox, oy, ow, oh, "start");
bt_start2->labelsize( 11 );	
bt_start2->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_start2->callback( cb_bt_start, (void*)2 );


fluidfl_find_dimensions( vfluidfl, "reset2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_reset2 = new Fl_Button(ox, oy, ow, oh, "reset");
bt_reset2->labelsize( 11 );	
//bt_reset2->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
bt_reset2->callback( cb_bt_reset, (void*)2 );


fluidfl_find_dimensions( vfluidfl, "cycle2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
ck_cycle2 = new Fl_Check_Button(ox, oy, ow, oh, "cycle");
ck_cycle2->labelsize( 9 );	
//ck_cycle2->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
ck_cycle2->callback( cb_bt_cycle, (void*)2 );


fluidfl_find_dimensions( vfluidfl, "beep2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
ck_beep2 = new Fl_Check_Button(ox, oy, ow, oh, "beep");
ck_beep2->labelsize( 9 );	
//ck_beep2->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
ck_beep2->callback( cb_ck_beep, (void*)2 );


fluidfl_find_dimensions( vfluidfl, "cmd_on2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
ck_cmd_on2 = new Fl_Check_Button(ox, oy, ow, oh, "cmd");
ck_cmd_on2->labelsize( 9 );	
//ck_cmd_on2->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
ck_cmd_on2->callback( cb_cmd_on0, (void*)2 );


fluidfl_find_dimensions( vfluidfl, "cmd2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_cmd2 = new Fl_Input(ox, oy, ow, oh, "cmd");
fi_cmd2->labelsize( 12 );	
fi_cmd2->textsize( 10 );	
fi_cmd2->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
//fi_cmd2->callback( cb_fi_tim0, (void*)2 );
fi_cmd2->type(FL_MULTILINE_INPUT);
fi_cmd2->tooltip( "e.g: gnome-calculator &" );

fluidfl_find_dimensions( vfluidfl, "beeps2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_beeps2 = new Fl_Input(ox, oy, ow, oh, "beeps");
fi_beeps2->labelsize( 12 );	
fi_beeps2->textsize( 10 );	
fi_beeps2->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
fi_beeps2->type( FL_INT_INPUT );
//fi_beeps1->callback( cb_fi_tim0, (void*)2 );


fluidfl_find_dimensions( vfluidfl, "pitch2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_pitch2 = new Fl_Input(ox, oy, ow, oh, "pitch");
fi_pitch2->labelsize( 12 );	
fi_pitch2->textsize( 10 );	
fi_pitch2->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
fi_pitch2->type( FL_INT_INPUT );
//fi_pitch1->callback( cb_fi_tim0, (void*)2 );




fluidfl_find_dimensions( vfluidfl, "wintop2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
ck_wintop2 = new Fl_Check_Button(ox, oy, ow, oh, "wintop");
ck_wintop2->labelsize( 9 );	
//ck_wintop2->align( FL_ALIGN_LEFT );			//FL_ALIGN_LEFT
ck_wintop2->callback( cb_bt_wintop, (void*)2 );


fluidfl_find_dimensions( vfluidfl, "test2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_test2 = new Fl_Button(ox, oy, ow, oh, "test");
bt_test2->labelsize( 11 );	
bt_test2->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_test2->callback( cb_bt_test, (void*)2 );

fluidfl_find_dimensions( vfluidfl, "interval_eta2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bx_interval_eta2 = new Fl_Box(ox, oy, ow, oh, "interval_eta2");
bx_interval_eta2->labelsize( 11 );	
bx_interval_eta2->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT

fluidfl_find_dimensions( vfluidfl, "eta2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bx_eta2 = new Fl_Box(ox, oy, ow, oh, "eta2");
bx_eta2->labelsize( 11 );	
bx_eta2->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
//-------


fluidfl_find_dimensions( vfluidfl, "sel0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_sel0 = new Fl_Round_Button(ox, oy, ow, oh, "");
bt_sel0->labelsize( 12 );	
bt_sel0->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_sel0->callback( cb_bt_sel, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "sel1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_sel1 = new Fl_Round_Button(ox, oy, ow, oh, "");
bt_sel1->labelsize( 12 );	
bt_sel1->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_sel1->callback( cb_bt_sel, (void*)1 );


fluidfl_find_dimensions( vfluidfl, "sel2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_sel2 = new Fl_Round_Button(ox, oy, ow, oh, "");
bt_sel2->labelsize( 12 );	
bt_sel2->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_sel2->callback( cb_bt_sel, (void*)2 );


fluidfl_find_dimensions( vfluidfl, "num7", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_num7 = new Fl_Button(ox, oy, ow, oh, "7");
bt_num7->labelsize( 11 );	
bt_num7->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_num7->callback( cb_bt_num, (void*)7 );

fluidfl_find_dimensions( vfluidfl, "num8", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_num8 = new Fl_Button(ox, oy, ow, oh, "8");
bt_num8->labelsize( 11 );	
bt_num8->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_num8->callback( cb_bt_num, (void*)8 );

fluidfl_find_dimensions( vfluidfl, "num9", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_num9 = new Fl_Button(ox, oy, ow, oh, "9");
bt_num9->labelsize( 11 );	
bt_num9->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_num9->callback( cb_bt_num, (void*)9 );

fluidfl_find_dimensions( vfluidfl, "num4", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_num4 = new Fl_Button(ox, oy, ow, oh, "4");
bt_num4->labelsize( 11 );	
bt_num4->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_num4->callback( cb_bt_num, (void*)4 );

fluidfl_find_dimensions( vfluidfl, "num5", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_num5 = new Fl_Button(ox, oy, ow, oh, "5");
bt_num5->labelsize( 11 );	
bt_num5->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_num5->callback( cb_bt_num, (void*)5 );

fluidfl_find_dimensions( vfluidfl, "num6", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_num6 = new Fl_Button(ox, oy, ow, oh, "6");
bt_num6->labelsize( 11 );	
bt_num6->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_num6->callback( cb_bt_num, (void*)6 );

fluidfl_find_dimensions( vfluidfl, "num1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_num1 = new Fl_Button(ox, oy, ow, oh, "1");
bt_num1->labelsize( 11 );	
bt_num1->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_num1->callback( cb_bt_num, (void*)1 );

fluidfl_find_dimensions( vfluidfl, "num2", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_num2 = new Fl_Button(ox, oy, ow, oh, "2");
bt_num2->labelsize( 11 );	
bt_num2->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_num2->callback( cb_bt_num, (void*)2 );

fluidfl_find_dimensions( vfluidfl, "num3", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_num3 = new Fl_Button(ox, oy, ow, oh, "3");
bt_num3->labelsize( 11 );	
bt_num3->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_num3->callback( cb_bt_num, (void*)3 );

fluidfl_find_dimensions( vfluidfl, "num0", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_num0 = new Fl_Button(ox, oy, ow, oh, "0");
bt_num0->labelsize( 11 );	
bt_num0->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_num0->callback( cb_bt_num, (void*)0 );

fluidfl_find_dimensions( vfluidfl, "clear", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_num0 = new Fl_Button(ox, oy, ow, oh, "clear");
bt_num0->labelsize( 11 );	
bt_num0->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_num0->callback( cb_bt_num, (void*)10 );

fluidfl_find_dimensions( vfluidfl, "dur1", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_dur1 = new button_mouse_click(ox, oy, ow, oh, "1min");
bt_dur1->labelsize( 11 );	
bt_dur1->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_dur1->callback( cb_bt_dur, (void*)1 );
bt_dur1->tooltip( sz_tooltip0 );

fluidfl_find_dimensions( vfluidfl, "dur5", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_dur5 = new button_mouse_click(ox, oy, ow, oh, "5min");
bt_dur5->labelsize( 11 );	
bt_dur5->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_dur5->callback( cb_bt_dur, (void*)5 );
bt_dur5->tooltip( sz_tooltip0 );

fluidfl_find_dimensions( vfluidfl, "dur10", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_dur10 = new button_mouse_click(ox, oy, ow, oh, "10min");
bt_dur10->labelsize( 11 );	
bt_dur10->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_dur10->callback( cb_bt_dur, (void*)10 );
bt_dur10->tooltip( sz_tooltip0 );

fluidfl_find_dimensions( vfluidfl, "dur15", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_dur15 = new button_mouse_click(ox, oy, ow, oh, "15min");
bt_dur15->labelsize( 11 );	
bt_dur15->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_dur15->callback( cb_bt_dur, (void*)15 );
bt_dur15->tooltip( sz_tooltip0 );

fluidfl_find_dimensions( vfluidfl, "dur20", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_dur20 = new button_mouse_click(ox, oy, ow, oh, "20min");
bt_dur20->labelsize( 11 );	
bt_dur20->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_dur20->callback( cb_bt_dur, (void*)20 );
bt_dur20->tooltip( sz_tooltip0 );

fluidfl_find_dimensions( vfluidfl, "dur30", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_dur30 = new button_mouse_click(ox, oy, ow, oh, "30min");
bt_dur30->labelsize( 11 );	
bt_dur30->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_dur30->callback( cb_bt_dur, (void*)30 );
bt_dur30->tooltip( sz_tooltip0 );

fluidfl_find_dimensions( vfluidfl, "dur45", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_dur45 = new button_mouse_click(ox, oy, ow, oh, "45min");
bt_dur45->labelsize( 11 );	
bt_dur45->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_dur45->callback( cb_bt_dur, (void*)45 );
bt_dur45->tooltip( sz_tooltip0 );

fluidfl_find_dimensions( vfluidfl, "dur60", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_dur15 = new button_mouse_click(ox, oy, ow, oh, "60min");
bt_dur15->labelsize( 11 );	
bt_dur15->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_dur15->callback( cb_bt_dur, (void*)60 );
bt_dur15->tooltip( sz_tooltip0 );


fluidfl_find_dimensions( vfluidfl, "load", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_load = new Fl_Button(ox, oy, ow, oh, "Load");
bt_load->labelsize( 11 );	
bt_load->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_load->callback( cb_bt_load, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "save", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_load = new Fl_Button(ox, oy, ow, oh, "Save");
bt_load->labelsize( 11 );	
bt_load->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_load->callback( cb_bt_save, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "time_now", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
fi_time_now = new Fl_Input(ox, oy, ow, oh, "Time Now");
fi_time_now->labelsize( 9 );	
fi_time_now->align( FL_ALIGN_TOP );			//FL_ALIGN_LEFT
//fi_interval_tim0->callback( cb_fi_tim0, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "ctrl_minus", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bx_ctrl_minus = new Fl_Box(ox, oy, ow, oh, "Ctrl for Minus");
bx_ctrl_minus->labelsize( 11 );	
bx_ctrl_minus->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT


//v1.04
fluidfl_find_dimensions( vfluidfl, "minus", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_minus = new Fl_Button(ox, oy, ow, oh, "-");
bt_minus->labelsize( 11 );	
bt_minus->align( FL_ALIGN_INSIDE );
bt_minus->callback( cb_bt_minus, (void*)0 );
bt_minus->type( FL_TOGGLE_BUTTON );
bt_minus->tooltip( "subtract time quantity" );




fluidfl_find_dimensions( vfluidfl, "about", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_about = new Fl_Button(ox, oy, ow, oh, "about");
bt_about->labelsize( 11 );	
bt_about->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_about->callback( cb_bt_combo, (void*)0 );


fluidfl_find_dimensions( vfluidfl, "-01", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_trim_01 = new button_mouse_click(ox, oy, ow, oh, "-1");
bt_trim_01->labelsize( 11 );	
bt_trim_01->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_trim_01->callback( cb_bt_trim, (void*)-1 );
bt_trim_01->tooltip( sz_tooltip1 );


fluidfl_find_dimensions( vfluidfl, "01", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_trim01 = new button_mouse_click(ox, oy, ow, oh, "+1");
bt_trim01->labelsize( 11 );	
bt_trim01->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_trim01->callback( cb_bt_trim, (void*)1 );
bt_trim01->tooltip( sz_tooltip1 );



fluidfl_find_dimensions( vfluidfl, "-05", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_trim_05 = new button_mouse_click(ox, oy, ow, oh, "-5");
bt_trim_05->labelsize( 11 );	
bt_trim_05->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_trim_05->callback( cb_bt_trim, (void*)-5 );
bt_trim_05->tooltip( sz_tooltip1 );


fluidfl_find_dimensions( vfluidfl, "05", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_trim05 = new button_mouse_click(ox, oy, ow, oh, "+5");
bt_trim05->labelsize( 11 );	
bt_trim05->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_trim05->callback( cb_bt_trim, (void*)5 );
bt_trim05->tooltip( sz_tooltip1 );





fluidfl_find_dimensions( vfluidfl, "-11", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_trim_11 = new button_mouse_click(ox, oy, ow, oh, "-1");
bt_trim_11->labelsize( 11 );	
bt_trim_11->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_trim_11->callback( cb_bt_trim, (void*)-1 );
bt_trim_11->tooltip( sz_tooltip1 );


fluidfl_find_dimensions( vfluidfl, "11", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_trim11 = new button_mouse_click(ox, oy, ow, oh, "+1");
bt_trim11->labelsize( 11 );	
bt_trim11->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_trim11->callback( cb_bt_trim, (void*)1 );
bt_trim11->tooltip( sz_tooltip1 );



fluidfl_find_dimensions( vfluidfl, "-15", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_trim_15 = new button_mouse_click(ox, oy, ow, oh, "-5");
bt_trim_15->labelsize( 11 );	
bt_trim_15->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_trim_15->callback( cb_bt_trim, (void*)-5 );
bt_trim_15->tooltip( sz_tooltip1 );


fluidfl_find_dimensions( vfluidfl, "15", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_trim15 = new button_mouse_click(ox, oy, ow, oh, "+5");
bt_trim15->labelsize( 11 );	
bt_trim15->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_trim15->callback( cb_bt_trim, (void*)5 );
bt_trim15->tooltip( sz_tooltip1 );





fluidfl_find_dimensions( vfluidfl, "-21", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_trim_21 = new button_mouse_click(ox, oy, ow, oh, "-1");
bt_trim_21->labelsize( 11 );	
bt_trim_21->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_trim_21->callback( cb_bt_trim, (void*)-1 );
bt_trim_21->tooltip( sz_tooltip1 );


fluidfl_find_dimensions( vfluidfl, "21", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_trim21 = new button_mouse_click(ox, oy, ow, oh, "+1");
bt_trim21->labelsize( 11 );	
bt_trim21->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_trim21->callback( cb_bt_trim, (void*)1 );
bt_trim21->tooltip( sz_tooltip1 );



fluidfl_find_dimensions( vfluidfl, "-25", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_trim_25 = new button_mouse_click(ox, oy, ow, oh, "-5");
bt_trim_25->labelsize( 11 );	
bt_trim_25->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_trim_25->callback( cb_bt_trim, (void*)-5 );
bt_trim_25->tooltip( sz_tooltip1 );


fluidfl_find_dimensions( vfluidfl, "25", find_idx, ox, oy, ow, oh );		//see 'extract_fluidfl_dimensions()'
bt_trim25 = new button_mouse_click(ox, oy, ow, oh, "+5");
bt_trim25->labelsize( 11 );	
bt_trim25->align( FL_ALIGN_INSIDE );			//FL_ALIGN_LEFT
bt_trim25->callback( cb_bt_trim, (void*)5 );
bt_trim25->tooltip( sz_tooltip1 );


wnd_fluid->end();
}















//----------------------------------------------------------




void update_fonts()
{
//wndMain->redraw();
//fi_unicode->textfont( font_num );
//fi_unicode->textsize( font_size );
//fi_unicode->redraw();

fl_message_font( (Fl_Font) font_num, font_size );

}



























//make sure 'picked' is not the same string as 'pathfilename'
//string 'picked' is loaded with selected path and filename, on 'OK' 
//on 'Cancel' string 'picked' is set to a null string
//returns 1 if 'OK', else 0
//set 'type' to Fl_File_Chooser::CREATE to allow a new filename to be entered

//linux code
#ifndef compile_for_windows
bool my_file_chooser( string &picked, const char* title, const char* pat, const char* start_path_filename, int type, Fl_Font fnt = -1, int fntsze = -1 )
{
picked = "";

//show file chooser
Fl_File_Chooser fc	( 	 start_path_filename,		// directory
						 pat,                       // filter
						 Fl_File_Chooser::SINGLE | type,   // chooser type
						 title						// title
					);

if ( fnt != -1 )fc.textfont( fnt );
if ( fntsze != -1 )fc.textsize( fntsze );

fc.show();

while( fc.shown() )
	{
	Fl::wait();
	}


if( fc.value() == 0 ) return 0;


picked = fc.value();

//windows code
//#ifdef compile_for_windows
//make the slash suit Windows OS
//mystr m1;
//m1 = fc.value();
//m1.FindReplace( picked, "/", "\\",0);
//#endif


return 1;
}
#endif












//windows code
#ifdef compile_for_windows
bool my_file_chooser( string &picked, const char* title, const char* pat, const char* start_path_filename, int type, Fl_Font fnt = -1, int fntsze = -1 )
{
OPENFILENAME ofn;
char szFile[ 8192 ];
string fname;

mystr m1;

m1 = start_path_filename;

m1.ExtractFilename( dir_seperator[ 0 ],  fname );		 //remove path from filename

strncpy( szFile, fname.c_str(), sizeof( szFile ) );		//put supplied fname as default

memset( &ofn, 0, sizeof( ofn ) );

ofn.lStructSize = sizeof ( ofn );
ofn.hwndOwner = NULL ;
ofn.lpstrFile = szFile ;
//ofn.lpstrFile[ 0 ] = '\0';
ofn.nMaxFile = sizeof( szFile );
ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
ofn.nFilterIndex = 1;
ofn.lpstrFileTitle = 0;
ofn.nMaxFileTitle = 0;
ofn.lpstrInitialDir = start_path_filename ;
ofn.lpstrTitle = title;
ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR ;

if( type == Fl_File_Chooser::CREATE )
	{
	if( GetSaveFileName( &ofn ) )
		{
		picked = szFile;
		return 1;
		}
	}
else{
	if( GetOpenFileName( &ofn ) )
		{
		picked = szFile;
		return 1;
		}
	}
return 0;
}
#endif




















//linux code
#ifndef compile_for_windows
//make sure 'picked' is not the same string as 'pathfilename'
//string 'picked' is loaded with selected dir, on 'OK'
//on 'Cancel' string 'picked' is set to a null string
//returns 1 if 'OK', else 0
//set 'type' to  Fl_File_Chooser::CREATE to allow a new directory to be entered
bool my_dir_chooser( string &picked, const char* title, const char* pat, const char* start_path_filename, int type, Fl_Font fnt = -1, int fntsze = -1 )
{
picked = "";
mystr m1;

//show file chooser
Fl_File_Chooser fc	( 	 start_path_filename,				// directory
						 pat,                     		  	// filter
						 Fl_File_Chooser::DIRECTORY | type,	// chooser type
						 title								// title
					);

if ( fnt != -1 )fc.textfont( fnt );
if ( fntsze != -1 )fc.textsize( fntsze );

fc.show();

while( fc.shown() )
	{
	Fl::wait();
	}


if( fc.value() == 0 ) return 0;


picked = fc.value();

//make the slash suit Windows OS
//m1 = fc.value();
//m1.FindReplace( picked, "/", "\\",0);



return 1;
}
#endif





















void cb_btRunAnyway(Fl_Widget *w, void* v)
{
Fl_Widget* wnd=(Fl_Widget*)w;
Fl_Window *win;
win=(Fl_Window*)wnd->parent();

iAppExistDontRun = 0;
win->~Fl_Window();
}






void cb_btDontRunExit(Fl_Widget* w, void* v)
{
Fl_Widget* wnd=(Fl_Widget*)w;
Fl_Window *win;
win=(Fl_Window*)wnd->parent();

win->~Fl_Window();
}









//linux code
#ifndef compile_for_windows 





//v1.32 - handle a 'BadWindow' without causing a crash
int x11_error_handler(Display* display, XErrorEvent* error)
{

char msg[80];
XGetErrorText( display, error->error_code, msg, 80 );
fprintf(stderr, "x11_error_handler() - Error code %s", msg);
return 0;
}





//v1.32 - sets 'XSetErrorHandler()'
//gets its ID, -- fixed memory leak using XFetchName (used XFree) 01-10-10
int FindWindowID(string csName,Display *display, Window &wid)
{
Window root, parent, *children;
unsigned int numWindows = 0;
int ret = 0;
bool vb = 0;

XSetErrorHandler( x11_error_handler );


//*display = XOpenDisplay(NULL);
//gDisp = XOpenDisplay(NULL);



if( !XQueryTree( display, RootWindow( display, 0 ), &root, &parent, &children, &numWindows) )
	{
	return 0;
	}

if( children == 0 ) return 0;


int i = 0;
for(i=0; i < numWindows ; i++)
	{
	Window root2, parent2, *children2;

	unsigned int numWindows2 = 0;

	if( children[i] == 0 ) continue;

		char *name0;
		if( XFetchName(display, children[i], &name0) )
			{
			if(vb) printf( "FindWindowID() - %d/%d: looking for '%s', found '%s'\n", i, numWindows, csName.c_str(), name0 );	

			XFree(name0);
			}
		else{
			if(vb) printf( "FindWindowID() - %d/%d: looking for '%s', could not get name\n", i, numWindows, csName.c_str() );	
//			continue;
			}

	 if( !XQueryTree( display, children[i], &root2, &parent2, &children2, &numWindows2 ) )
		{
		continue;
		}


	if( children2 == 0 ) continue;


	for(int j = 0; j < numWindows2 ; j++)
		{	
		char *name1;
		if( XFetchName( display, children2[j], &name1 ) )
			{
			if(vb) printf( "FindWindowID() [child] - %d/%d: looking for '%s', found '%s'\n", j, numWindows2, csName.c_str(), name1 );

			if(strcmp( csName.c_str(), name1 ) == 0 )
				{
//				XMoveWindow(display, children2[j], -100, -100);
//				XMoveWindow(display, children2[j], -100, -100);
//				XMoveWindow(display, children2[j], -100, -100);
//				XResizeWindow(display, children2[j], 1088, 612+22);
//				XMoveWindow(*display, children2[j], 1100, 22);
				if(vb) printf( "FindWindowID() [child] - %d/%d: MATCH FOUND '%s'\n", j, numWindows2, csName.c_str(), name1  );
				wid = children2[j];
				ret = 1;
//				if(ret)
//					{
//					printf("\n\nTrying to Move %x  %x\n\n",gDisp, gw);
//					XMoveWindow(gDisp, gw, 700, 22);
//					return 1;
//					}
				}
			XFree( name1 );
			}
		else{
			if(vb) printf( "FindWindowID() [child] - %d/%d, could not get name\n", j, numWindows2 );
			}
		
		if (ret) break;
		}


	if(children2) XFree(children2);
	if (ret) break;
	}

if( children ) XFree(children);

return  ret;
}



#endif









//v1.31
void BringWindowToFront(string csTitle)
{

//linux code
#ifndef compile_for_windows

Display *gDisp0;			//v1.31


gDisp0 = XOpenDisplay(NULL);

Window wid;
if(FindWindowID(csTitle,gDisp0,wid))
	{
	XUnmapWindow(gDisp0, wid);
	XMapWindow(gDisp0, wid);
	XFlush(gDisp0);
	}

XCloseDisplay(gDisp0);		//added this to see if valgrind showed improvement - it didn't
#endif


//windows code
#ifdef compile_for_windows
HWND hWnd;
//csAppName.LoadString(IDS_APPNAME);

hWnd = FindWindow( 0, cnsAppName );

if( hWnd )
	{
	::BringWindowToTop( hWnd );
//	::SetForegroundWindow( hWnd );
//	::PostMessage(hWnd,WM_MAXIMIZE,0,0);
	::ShowWindow( hWnd, SW_RESTORE );
	}
#endif

}











//linux code
#ifndef compile_for_windows 

//test if window with csAppName already exists, if so create inital main window with
//two options to either run app, or to exit.
//if no window with same name exists returns 0
//if 'exit' option chosen, exit(0) is called and no return happens
//if 'run anyway' option is chosen, returns 1
int CheckInstanceExists(string csAppName)
{
string csTmp;


Display *gDisp0;			//v1.31
gDisp0 = XOpenDisplay(NULL);

if( gDisp0 == 0 )			//ubuntu addition, incase 'pkexec' is used and access to display was not granted
	{
	printf( "CheckInstanceExists() - failed at call: 'XOpenDisplay()'\n" );
	return 1;				//returning 1 will close app
	}


Window wid;


if(FindWindowID(csAppName,gDisp0,wid))		//a window with same name exists?
	{
	BringWindowToFront( csAppName );

	XCloseDisplay(gDisp0);		//added this to see if valgrind showed improvement - it didn't

	Fl_Window *wndInitial = new Fl_Window(50,50,480,90);
	wndInitial->label("Possible Instance Already Running");
	
	Fl_Box *bxHeading = new Fl_Box(10,10,200, 15, "Another Window With Same Name Was Found.");
	bxHeading->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	strpf(csTmp,"App Name: '%s'",csAppName.c_str()); 
	Fl_Box *bxAppName = new Fl_Box(10,30,200, 15,csTmp.c_str());
	bxAppName->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	Fl_Button *btDontRunExit = new Fl_Button( wndInitial->w() - 130 - 10, 55, 130, 25,"Don't Run App, Exit");
	btDontRunExit->labelsize(12);
	btDontRunExit->callback(cb_btDontRunExit,0);


	Fl_Button *btRunAnyway = new Fl_Button( btDontRunExit->x() - 130 - 10,55,130,25,"Run App Anyway");
	btRunAnyway->labelsize(12);
	btRunAnyway->callback(cb_btRunAnyway,0);

	wndInitial->end();
	wndInitial->show();

	Fl::run();

	return iAppExistDontRun;
	}
else{
	iAppExistDontRun=0;
	}


return iAppExistDontRun;

}

#endif














//windows code
#ifdef compile_for_windows 

//test if window with csAppName already exists, if so create inital main window with
//two options to either run app, or to exit.
//if no window with same name exists returns 0
//if 'exit' option chosen, exit(0) is called and no return happens
//if 'run anyway' option is chosen, returns 1
int CheckInstanceExists( string csAppName )
{
string csTmp;

HWND hWnd;
//csAppName.LoadString(IDS_APPNAME);

hWnd = FindWindow( 0, csAppName.c_str() );

if( hWnd )
	{
	BringWindowToFront( csAppName );
//	::BringWindowToTop( hWnd );
//::SetForegroundWindow( hWnd );
//::PostMessage(hWnd,WM_MAXIMIZE,0,0);
//	::ShowWindow( hWnd, SW_RESTORE );
Sleep(1000);

	Fl_Window *wndInitial = new Fl_Window(50,50,430,90);
	wndInitial->label("Possible Instance Already Running");
	
	Fl_Box *bxHeading = new Fl_Box(10,10,200, 15, "Another Window With Same Name Was Found.");
	bxHeading->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	strpf(csTmp,"App Name: '%s'",csAppName.c_str()); 
	Fl_Box *bxAppName = new Fl_Box(10,30,300, 15,csTmp.c_str());
	bxAppName->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	Fl_Button *btRunAnyway = new Fl_Button(25,55,130,25,"Run App Anyway");
	btRunAnyway->labelsize(12);
	btRunAnyway->callback(cb_btRunAnyway,0);

	Fl_Button *btDontRunExit = new Fl_Button(btRunAnyway->x()+btRunAnyway->w()+15,55,130,25,"Don't Run App, Exit");
	btDontRunExit->labelsize(12);
	btDontRunExit->callback(cb_btDontRunExit,0);

	wndInitial->end();
	wndInitial->show();
	wndInitial->hide();
	wndInitial->show();
//	wndInitial->resizable(wndInitial);
	Fl::run();

	return iAppExistDontRun;
	}
else iAppExistDontRun = 0;

return iAppExistDontRun;
}

#endif














//moded: 07-feb 2017 to ensure no vparams that are null strings are stored
//extract command line details from windows, is called by:  get_path_app_params()

//from GCCmdLine::GetAppName()
//eg: prog.com											//no path
//eg. "c:\dos\edit prog.com"							//with path\prog in quotes
//eg. "c:\dos\edit prog.com" c:\dos\junk.txt			//with path\prog in quotes and path\file
//eg. c\dos\edit.com /p /c		(as in screen-savers)	//path\prog and params no quotes

void extract_cmd_line( string cmdline, string &path, string &appname, vector<string> &vparams )
{
string stmp;
char ch;

path = "";
appname = "";
vparams.clear();
bool in_str = 0;
bool in_quote = 0;
bool beyond_app_name = 0;
//bool app_name_in_quotes = 0;

//cmdline = "c:/dos/edit prog.com";
//cmdline = "\"c:/dos/edit prog.com\" hello 123";
//cmdline = "c:/dos/edit.com hello 123";

int len =  cmdline.length();

if( len == 0 ) return;

for( int i = 0; i < len; i++ )
	{
	ch = cmdline[ i ];
	
	if( ch == '\"' )									//quote?
		{
		if( in_quote )
			{
			in_quote = 0;								//if here found closing quote
			goto got_param;
			}
		else{
			in_quote = 1;
			}
		}
	else{
		if( ch == ' ' )									//space?
			{
			if( !in_quote )				//if not in quote and space must be end of param
				{
				if( in_str ) goto got_param;
				}
			}
		else{
			in_str = 1;
			}

		if( in_str ) stmp += ch;
		}

	continue;

	got_param:

	in_str = 0;
	if ( beyond_app_name == 0 )					//store where approp
		{
		path = stmp;
		beyond_app_name = 1;
		}
	else{
		//store if not just a space
		if( stmp.compare( " " ) != 0 ) vparams.push_back( stmp );
		}

	stmp = "";
	}


//if here end of params reached, store where approp
if ( beyond_app_name == 0 )
	{
	path = stmp;
	}
else{
	if( stmp.length() != 0 ) vparams.push_back( stmp );
	}




appname = path;

len = path.length();
if( len == 0 ) return;

int pos = path.rfind( dir_seperator );

if( pos == string::npos )					//no directory path found?
	{
	path = "";	
	return;
	}

if( ( pos + 1 ) < len ) appname = path.substr( pos + 1,  pos + 1 - len );	//extract appname
path = path.substr( 0,  pos );								//extract path


//windows code
#ifdef compile_for_windows 
#endif

}



















//fixed windows version: v1.20 07-dec-2017, correctly handles paths with spaces that are not quoted (e.g: run from a batch file)
// !! will FAIL if params have a dir seperator in them as in:  D:\New Folder\skeleton.exe "d:\grrr" a "1 2" 3 4
//!!fixed windows version: 07-feb-2017
//find this app's path, app name and cmdline params,
//linux version uses 'argc, argc' for cmdline param extraction
//windows version uses
void get_path_app_params( string dir_sep, int argc, char **argv, string &path_out, string &app_name, vector<string> &vparams )
{
printf( "get_path_app_params()\n" );

string s1, s2, path;
mystr m1;

vparams.clear();

//linux code
#ifndef compile_for_windows

//get the actual path this app lives in
#define MAXPATHLEN 1025   // make this larger if you need to

int length;
char fullpath[MAXPATHLEN];

// /proc/self is a symbolic link to the process-ID subdir
// of /proc, e.g. /proc/4323 when the pid of the process
// of this program is 4323.
//
// Inside /proc/<pid> there is a symbolic link to the
// executable that is running as this <pid>.  This symbolic
// link is called "exe".
//
// So if we read the path where the symlink /proc/self/exe
// points to we have the full path of the executable.



length = readlink("/proc/self/exe", fullpath, sizeof(fullpath));

//printf("readlink: '%s'\n", fullpath );

// Catch some errors:
if (length < 0)
	{
	syslog(LOG_ALERT,"Error resolving symlink /proc/self/exe.\n");
	fprintf(stderr, "Error resolving symlink /proc/self/exe.\n");
	exit(0);
	}

if (length >= MAXPATHLEN)
	{
	syslog(LOG_ALERT, "Path too long. Truncated.\n");
	fprintf(stderr, "Path too long. Truncated.\n");
	exit(0);
	}

//I don't know why, but the string this readlink() function 
//returns is appended with a '@'

fullpath[length] = '\0';      // Strip '@' off the end


//printf("Full path is: %s\n", fullpath);
//syslog(LOG_ALERT,"Full path is: %s\n", fullpath);

m1 = fullpath;
m1.ExtractPath( dir_sep[ 0 ], path_out );

m1 = fullpath;
m1.ExtractFilename( dir_sep[ 0 ], app_name );


for( int i = 1; i < argc; i++ )
	{
	vparams.push_back( argv[ i ] );
	}
//syslog(LOG_ALERT,"Path only is: %s\n", csPathFilename.c_str());

#endif



//windows code
#ifdef compile_for_windows 
UINT i,uiLen;                    //eg. c\dos\edit.com /p /c		(as in screen-savers)
bool bQuotes;
string csCmdLineStr;


//----------------------------
//from GCCmdLine::GetAppName()
//eg: prog.com											//no path
//eg. "c:\dos\edit prog.com"							//with path\prog in quotes
//eg. "c:\dos\edit prog.com" c:\dos\junk.txt			//with path\prog in quotes and path\file
//eg. c\dos\edit.com /p /c		(as in screen-savers)	//path\prog and params no quotes
// !! will FAIL if params have a dir seperator in them as in:  D:\New Folder\skeleton.exe "d:\grrr" a "1 2" 3 4

csCmdLineStr = GetCommandLine();
printf("csCmdLineStr: '%s'\n", csCmdLineStr.c_str() );
//csCmdLineStr = "skeleton.exe abc";
//printf("csCmdLineStr= '%s'\n", csCmdLineStr.c_str() );

char path_appname[ MAX_PATH ];
GetModuleFileName( NULL, path_appname, MAX_PATH );

printf("path_appname= '%s'\n", path_appname );

//----
//added v1.20, now extracts app path that has spaces in it,
//uses the last directory seperator as indicator of end of path and start of filename and its parameters
m1 = path_appname;
string spath;
spath = path_appname;


if( m1.ExtractPath( dir_seperator[ 0 ], s1 ) )
	{
//	printf("wincmd2: '%s'\n", s1.c_str() );
	spath = s1;
	}
else{
	printf("get_path_app_params() - ExtractPath() - failed to extract path from: '%s'\n", spath.c_str() );
	}

m1 = csCmdLineStr;
if( m1.ExtractFilename( dir_seperator[ 0 ], s1 ) )
	{
	csCmdLineStr = s1;
	}
else{
	printf("get_path_app_params() - ExtractFilename() - failed to extract path from: '%s'\n", spath.c_str() );
	}

printf("csCmdLineStr2: '%s'\n", csCmdLineStr.c_str() );

//----



csCmdLineStr += " ";
m1 = csCmdLineStr;
m1.cut_just_past_first_find_and_keep_right( s1, " ", 0 ); 	//strip off app name to just leave params

//reform full command line
//s2 = path_appname;spath	removed v1.20
s2 = "c:";						//added v1.20, add a dummy path that has no spaces so 'extract_cmd_line()' will still extract app name and its params
s2 += dir_seperator[ 0 ];
s2 += csCmdLineStr;				//make time pathname appname and cmdline params
//s2 += s1;						removed v1.20 //make time pathname appname and cmdline params

//printf( "get_path_app_params() - s2: '%s'\n", s2.c_str() );

extract_cmd_line( s2, path_out, app_name, vparams );

path_out = spath;				//added v1.20, ignore original path_out as it might have had spaces
#endif




printf( "get_path_app_params() - path_out: '%s'\n", path_out.c_str() );
printf( "get_path_app_params() - app_name: '%s'\n", app_name.c_str() );


for( int i = 0; i < vparams.size(); i++ )
	{
	printf( "get_path_app_params() - vparams[%d]: '%s'\n", i, vparams[ i ].c_str() );
	}

}




















void LoadSettings(string csIniFilename)
{
string csTmp;
int x,y,cx,cy;
string s1;
mystr m1;

GCProfile p(csIniFilename);
x=p.GetPrivateProfileLONG("Settings","WinX",100);
y=p.GetPrivateProfileLONG("Settings","WinY",100);
//cx=p.GetPrivateProfileLONG("Settings","WinCX",750);
//cy=p.GetPrivateProfileLONG("Settings","WinCY",550);

//wndMain->position( x , y );	
//wndMain->size( cx , cy );	

if( x < 0 ) x = 0;
if( x > 1920 ) x = 1900;

if( y < 0 ) y = 0;
if( y > 1000 ) y = 1000;

wnd_fluid->position( x, y );

vtime[0].interval_time = p.GetPrivateProfileLONG("Settings","interval_time0",0x010000);
vtime[1].interval_time = p.GetPrivateProfileLONG("Settings","interval_time1",0x010000);
vtime[2].interval_time = p.GetPrivateProfileLONG("Settings","interval_time2",0x010000);

vtime[0].options = p.GetPrivateProfileLONG("Settings","options0", eo_cycle | eo_beep | eo_win_to_front | eo_run_cmd );
vtime[1].options = p.GetPrivateProfileLONG("Settings","options1", eo_cycle | eo_beep | eo_win_to_front | eo_run_cmd );
vtime[2].options = p.GetPrivateProfileLONG("Settings","options2", eo_cycle | eo_beep | eo_win_to_front | eo_run_cmd );

p.GetPrivateProfileStr("Settings","last_filename", csIniFilename, &slast_filename );

p.GetPrivateProfileStr("Settings", "cmd0", "",  &s1 );
m1 = s1;
m1.EscToStr();
fi_cmd0->value( m1.szptr() );

p.GetPrivateProfileStr("Settings","beeps0", "2", &s1 );
fi_beeps0->value( s1.c_str() );

p.GetPrivateProfileStr("Settings","pitch0", "4000", &s1 );
fi_pitch0->value( s1.c_str() );


p.GetPrivateProfileStr("Settings", "cmd1", "",  &s1 );
m1 = s1;
m1.EscToStr();
fi_cmd1->value( m1.szptr() );

p.GetPrivateProfileStr("Settings","beeps1", "2", &s1 );
fi_beeps1->value( s1.c_str() );

p.GetPrivateProfileStr("Settings","pitch1", "4000", &s1 );
fi_pitch1->value( s1.c_str() );


p.GetPrivateProfileStr("Settings", "cmd2", "",  &s1 );
m1 = s1;
m1.EscToStr();
fi_cmd2->value( m1.szptr() );

p.GetPrivateProfileStr("Settings","beeps2", "2", &s1 );
fi_beeps2->value( s1.c_str() );

p.GetPrivateProfileStr("Settings","pitch2", "4000", &s1 );
fi_pitch2->value( s1.c_str() );

//vtime[0].cmd = m1.szptr();

}









void SaveSettings(string csIniFilename)
{
string s1;
mystr m1;

GCProfile p(csIniFilename);


//p.WritePrivateProfileLONG("Settings","WinX", wndMain->x()-iBorderWidth);
//p.WritePrivateProfileLONG("Settings","WinY", wndMain->y()-iBorderHeight);
//p.WritePrivateProfileLONG("Settings","WinCX", wndMain->w());
//p.WritePrivateProfileLONG("Settings","WinCY", wndMain->h());



//this uses previously saved window sizing value, hopefully grabbed before window was maximized by user see: dble_wnd::resize()
//remove window border offset when saving window pos settings
p.WritePrivateProfileLONG("Settings","WinX", wnd_fluid->x() - iBorderWidth );
p.WritePrivateProfileLONG("Settings","WinY", wnd_fluid->y() - iBorderHeight);
//p.WritePrivateProfileLONG("Settings","WinCX", wnd_fluid->restore_size_w );
//p.WritePrivateProfileLONG("Settings","WinCY", wnd_fluid->restore_size_h );


p.WritePrivateProfileLONG("Settings","interval_time0", vtime[0].interval_time );
p.WritePrivateProfileLONG("Settings","interval_time1", vtime[1].interval_time );
p.WritePrivateProfileLONG("Settings","interval_time2", vtime[2].interval_time );

p.WritePrivateProfileLONG("Settings","options0", vtime[0].options );
p.WritePrivateProfileLONG("Settings","options1", vtime[1].options );
p.WritePrivateProfileLONG("Settings","options2", vtime[2].options );


p.WritePrivateProfileStr("Settings","last_filename", slast_filename );

m1 = fi_cmd0->value();
m1.StrToEscMostCommon1();
p.WritePrivateProfileStr("Settings","cmd0",  m1.szptr() );

p.WritePrivateProfileStr("Settings","beeps0", fi_beeps0->value() );
p.WritePrivateProfileStr("Settings","pitch0", fi_pitch0->value() );



m1 = fi_cmd1->value();
m1.StrToEscMostCommon1();
p.WritePrivateProfileStr("Settings","cmd1",  m1.szptr() );

p.WritePrivateProfileStr("Settings","beeps1", fi_beeps1->value() );
p.WritePrivateProfileStr("Settings","pitch1", fi_pitch1->value() );



m1 = fi_cmd2->value();
m1.StrToEscMostCommon1();
p.WritePrivateProfileStr("Settings","cmd2",  m1.szptr() );

p.WritePrivateProfileStr("Settings","beeps2", fi_beeps2->value() );
p.WritePrivateProfileStr("Settings","pitch2", fi_pitch2->value() );

}




void DoQuit()
{

SaveSettings(csIniFilename);

exit(0);
}










void cb_wnd_fluid( Fl_Callback *, void* v)
{
Fl_Window* wnd = (Fl_Window*)v;

//wndMain->iconize();
//wndMain->hide();
//wndMain->show();

//do_quit_via_timer = 1;
DoQuit();

}









void cb_btAbout(Fl_Widget *, void *)
{
string s1, st;

Fl_Window *wnd = new Fl_Window( wnd_fluid->x()+20, wnd_fluid->y()+20,600,100 );
wnd->label("About");
Fl_Input *teText = new Fl_Input(10,10,wnd->w()-20,wnd->h()-20,"");
teText->type(FL_MULTILINE_OUTPUT);
teText->textsize(12);

strpf( s1, "%s,  %s,  Built: %s\n", cnsAppWndName, "v1.06", cns_build_date );
st += s1;


strpf( s1, "\nTimer utility with 3 countdown timers, each can be set to execute a bash command." );
st += s1;


teText->value(st.c_str());
wnd->end();

#ifndef compile_for_windows
wnd->set_modal();
#endif

wnd->show();

}












void cb_btQuit(Fl_Widget *, void *)
{
DoQuit();
}








void cb_timer1(void *)
{
string s1;
mystr m1;

Fl::repeat_timeout( 0.25, cb_timer1 );

modulo_sec_cnt++;
if( modulo_sec_cnt >= modulo_sec )									//one sec count
	{
	modulo_sec_cnt = 0;

	for( int i = 0; i < vtime.size(); i++ )
		{
		
		if( vtime[i].state == 1 )
			{
			add_to_time( i, 0, -1 );
			if( check_time_period_ended( i ) )
				{
				vtime[i].state = 0;
				
				if( i == 0 )
					{
					s1 = fi_beeps0->value();
					int beeps;
					sscanf( s1.c_str(), "%d", &beeps );
					
					if( beeps < 0 ) beeps = 0;
					if( beeps > 60 ) beeps = 60;


					s1 = fi_pitch0->value();
					int pitch;
					sscanf( s1.c_str(), "%d", &pitch );
					
					if( pitch < 100 ) pitch = 100;
					if( pitch > 11000 ) pitch = 11000;
				
					if( vtime[i].options & eo_beep ) do_beep( pitch, cn_beep_delay, beeps );
					if( vtime[i].options & eo_run_cmd ) RunShell( fi_cmd0->value() );
					}

				if( i == 1 )
					{
					s1 = fi_beeps1->value();
					int beeps;
					sscanf( s1.c_str(), "%d", &beeps );
					
					if( beeps < 0 ) beeps = 0;
					if( beeps > 60 ) beeps = 60;


					s1 = fi_pitch1->value();
					int pitch;
					sscanf( s1.c_str(), "%d", &pitch );
					
					if( pitch < 100 ) pitch = 100;
					if( pitch > 11000 ) pitch = 11000;
				
					if( vtime[i].options & eo_beep ) do_beep( pitch, cn_beep_delay, beeps );
					if( vtime[i].options & eo_run_cmd ) RunShell( fi_cmd1->value() );
					}


				if( i == 2 )
					{
					s1 = fi_beeps2->value();
					int beeps;
					sscanf( s1.c_str(), "%d", &beeps );
					
					if( beeps < 0 ) beeps = 0;
					if( beeps > 60 ) beeps = 60;


					s1 = fi_pitch2->value();
					int pitch;
					sscanf( s1.c_str(), "%d", &pitch );
					
					if( pitch < 100 ) pitch = 100;
					if( pitch > 11000 ) pitch = 11000;
				
					if( vtime[i].options & eo_beep ) do_beep( pitch, cn_beep_delay, beeps );
					if( vtime[i].options & eo_run_cmd ) RunShell( fi_cmd2->value() );
					}
					
				if( vtime[i].options & eo_win_to_front )
					{
					wnd_fluid->hide();
					wnd_fluid->show();
					}

				if( vtime[i].options & eo_cycle )
					{
					vtime[i].time = vtime[i].interval_time;
					vtime[i].state = 1;	
					}

				}
			}
			
		}
	}

modulo_flash_cnt++;
if( modulo_flash_cnt >= modulo_flash )
	{
	modulo_flash_cnt = 0;
	}

update_controls( 0 );

}













//linux code
#ifndef compile_for_windows

//execute shell cmd
int RunShell(string sin)
{

if ( sin.length() == 0 ) return 0;

//make command to cd working dir to app's dir and execute app (params in "", incase of spaces)
//strpf(csCmd,"cd \"%s\";\"%s\" \"%s\"",csPath.c_str(),sEntry[iEntryNum].csStartFunct.c_str(),csFile.c_str());

pid_t child_pid;

child_pid=fork();		//create a child process	

if(child_pid==-1)		//failed to fork?
	{
	printf("\nRunShell() failed to fork\n");
	return 0;
	}

if(child_pid!=0)		//parent fork? i.e. child pid is avail
	{
	int status;
	printf("\nwaitpid: %d, RunShell start\n",child_pid);	

	while(1)
		{
		waitpid(child_pid,&status,0);		//wait for return val from child so a zombie process is not left in system
		printf("\nwaitpid %d RunShell stop\n",child_pid);
		if(WIFEXITED(status)) break;		//confirm status returned shows the child terminated
		}	
	}
else{					//child fork (0) ?
//	printf("\nRunning Shell: %s\n",csCmd.c_str());
	printf("\nRunShell system cmd started: %s\n",sin.c_str());	
	system(sin.c_str());
	printf("\nRunShell system cmd finished \n");	
	exit(1);
	}
return 1;
}

#endif











//windows code
#ifdef compile_for_windows

//execute shell cmd as a process that can be monitored
int RunShell( string sin )
{
BOOL result;
wstring ws1;

if ( sin.length() == 0 ) return 0;


mystr m1 = sin;

m1.mbcstr_wcstr( ws1 );	//convert utf8 string to windows wchar string array


memset(&processInformation, 0, sizeof(processInformation));


STARTUPINFOW StartInfoW; 							// name structure
memset(&StartInfoW, 0, sizeof(StartInfoW));
StartInfoW.cb = sizeof(StartInfoW);

StartInfoW.wShowWindow = SW_HIDE;

result = CreateProcessW( NULL, (WCHAR*)ws1.c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &StartInfoW, &processInformation);

if ( result == 0)
	{
	
	return 0;
	}

return 1;

}

#endif



































































int main(int argc, char **argv)
{
//-------> !!!!!!! Fl::visual( FL_RGB );								//window might need this for offscreen drawing


//Fl::scheme("plastic");							//NOTE!!!!  for some reason the: 'Fl::scheme("plastic")' scheme causes scroll child objs not to be cleared/redrawn correctly,
													//this leave pixel debris when a scroll obj is scrolled, its children leave pixels behind, the: 'Fl::scheme("gtk+")' scheme does not suffer this
Fl::scheme("plastic");
Fl::scheme("gtk+");

string s, fname, dir_sep;
bool add_ini = 1;									//assume need to add ini extension	

//Fl::set_font( FL_HELVETICA, "Helvetica bold");
//Fl::set_font( FL_COURIER, "");
//Fl::set_font( FL_COURIER, "Courier bold italic");

//fl_font( (Fl_Font) FL_COURIER, 12 );

fname = cnsAppName;							//assume ini file will have same name as app
dir_sep = "";								//assume no path specified, so no / or \ (dos\windows)



//test if window with same name found and ask if still to run this -
// will return 1 if user presses 'Don't Run, Exit' button
if( CheckInstanceExists( cnsAppWndName ) ) return 0;

//linux code
#ifndef compile_for_windows
dir_seperator = "/";									//use unix folder directory seperator
#endif


dir_sep = dir_seperator;



//windows code
//attach a command line console, so printf works
#ifdef compile_for_windows
int hCrt;
FILE *hf;

AllocConsole();
hCrt = _open_osfhandle( (long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
hf = _fdopen( hCrt, "w" );
*stdout = *hf;
setvbuf( stdout, NULL, _IONBF, 0 );
#endif



string app_name;
vector<string> vparams;

get_path_app_params( dir_seperator, argc, argv, app_path, app_name, vparams );







//handle command line params
printf("\n\ntimer_util_linux\n\n");
    printf("~~~~~~~~~~~~~~~~");


printf("\nSpecify no switches to use config with app's filename in app's dir\n");
printf("\nSpecify '--cf ffff' to use config with filename 'ffff'\n");


if( argc == 1 )
	{
	printf( "-- Using app's dir for config storage.\n" );
	}


if( argc == 3 )
	{
	if( strcmp( argv[ 1 ], "--cf" ) == 0 )
		{
		printf( "-- Using specified filename for config storage.\n" );
		fname = argv[ 2 ];
		app_path = "";
		dir_sep = "";								//no directory seperator
		add_ini = 0;								//user spec fname don't add ini ext
		}
	}





if( app_path.length() == 0 )
	{
	csIniFilename = app_path + fname;					//make config file pathfilename
	}
else{
	csIniFilename = app_path + dir_sep + fname;			//make config file pathfilename
	}
if( add_ini ) csIniFilename += ".ini";                  //need ini if no user spcified fname




wnd_fluid = make_fluid_window();
wnd_fluid->callback((Fl_Callback *)cb_wnd_fluid, wnd_fluid);


make_fluid_ctrls();


st_time_tag o;

o.time = 0;
o.interval_time = 0x010000;						//01:00:00, 1 hr
o.state = 0;
o.cycle = 1;
o.options = eo_cycle | eo_beep | eo_win_to_front | eo_run_cmd;

o.beeps = 2;
o.pitch = 4000;
//o.cmd = "gnome-calendar";

vtime.push_back( o );
vtime.push_back( o );
vtime.push_back( o );

LoadSettings(csIniFilename); 

bt_sel0->value(1);

update_controls( 1 );							//update check boxes as well

update_fonts(); //needed this after LoadSettings() so the ini font value is loaded via font_pref_wnd->Load(p);

//fi_unicode_greek->textfont( font_num );	//needed this after LoadSettings() so the ini font value is loaded via font_pref_wnd->Load(p);
//fi_unicode->textfont( font_num );		//needed this after LoadSettings() so the ini font value is loaded via font_pref_wnd->Load(p);





//wndMain->show(argc, argv);

//wnd_fluid->position( 10, 400 );
wnd_fluid->show();

//fl_font( (Fl_Font) font_num, font_size );



Fl::add_timeout( 0.5, cb_timer1 );		//update controls, post queued messages


fl_message_font( (Fl_Font) font_num, font_size );

 
int iAppRet=Fl::run();


return iAppRet;

}














//start a timing timer with internal ns resolution
//the supplied var 'start' will hold an abitrary count in ns used by call time_passed()
void time_start( unsigned long long int &start )
{

//linux code
#ifndef compile_for_windows 

timespec ts_now;

clock_gettime( CLOCK_MONOTONIC, &ts_now );		//initial time read
start = ts_now.tv_nsec;							//get ns count
start += (double)ts_now.tv_sec * 1e9;					//make secs count a ns figure

#endif




//windows code
#ifdef compile_for_windows

LARGE_INTEGER li;
unsigned long long int big;

QueryPerformanceFrequency( &li );		//get performance counter's freq


big = 0;
big |= li.HighPart;
big = big << 32;
big |= li.LowPart;

perf_period = (double)1.0 / (double)big;		//derive performance counter's period

//cslpf("Freq = %I64u\n", big );
//cslpf("Period = %g\n", perf_period );

QueryPerformanceCounter( &li );

start = 0;
start |= li.HighPart;
start = start << 32;
start |= li.LowPart;

start = start * perf_period * 1e9;					//make performance counter a ns figure
#endif

}








//calc secs that have passed since call to time_start()
//calcs are maintained in ns internally and in supplied var 'start'
//returns time that has passed in seconds
double time_passed( unsigned long long int start )
{
unsigned long long int now, time_tot;

//linux code
#ifndef compile_for_windows 

timespec ts_now;

clock_gettime( CLOCK_MONOTONIC, &ts_now );		//read current time
now = ts_now.tv_nsec;							//get ns count
now += (double)ts_now.tv_sec * 1e9;				//make secs count a ns figure

time_tot = now - start;

#endif





//windows code
#ifdef compile_for_windows

LARGE_INTEGER li;

QueryPerformanceCounter( &li );

now = 0;
now |= li.HighPart;
now = now << 32;
now |= li.LowPart;

now = now * perf_period * 1e9;			//conv performance counter figure into ns

time_tot = now - start;
#endif


return (double)time_tot * 1e-9;			//conv internal ns delta to secs
}
















