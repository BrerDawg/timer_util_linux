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


//button_mouse_click.cpp

//v1.01	 2024-04-25		//


#include "button_mouse_click.h"






//----------------------------------------------------------

button_mouse_click::button_mouse_click( int xx, int yy, int wid, int hei, const char *label ) : Fl_Button( xx, yy, wid ,hei, label )
{
left_button = 0;
right_button = 0;
ctrl_key = 0;
shift_key = 0;
}


button_mouse_click::~button_mouse_click()
{

}




int button_mouse_click::handle( int e )
{
bool need_redraw = 0;
bool dont_pass_on = 0;


if ( e & FL_ENTER )
    {
//	inside_button_idx = 0;
    dont_pass_on = 1;		//need this for tooltip
    need_redraw = 1;
// printf("FL_ENTER inside_button_idx %d\n", inside_button_idx );
   }


if ( e & FL_LEAVE )
    {
    dont_pass_on = 1;		//need this for tooltip
    need_redraw = 1;
//	inside_button_idx = -1;
//printf("FL_LEAVE inside_button_idx %d\n", inside_button_idx );
    }

if ( e == FL_FOCUS )
    {
//	check_focus_changed( this );
	dont_pass_on = 1;	
	}

if ( e == FL_PUSH )
	{
	if(Fl::event_button() == 1)
		{
		left_button = 1;
		need_redraw = 1;
		}
		
	if(Fl::event_button() == 3 )
		{
		right_button = 1;
		need_redraw = 1;
		}
		
	dont_pass_on = 0;
	}


if ( e == FL_RELEASE )
	{
	
	if(Fl::event_button() == 1 )
		{
//		left_button = 0;						//this flag NEEDS to be cleared in your callback, and NOT in the 'handle()' function of this class
		need_redraw = 1;
		}

	if(Fl::event_button() == 3 )
		{
//		right_button = 0;						//this flag NEEDS to be cleared in your callback, and NOT in the 'handle()' function of this class
		need_redraw = 1;
		}
	dont_pass_on = 0;
	}



//if ( ( e == FL_KEYDOWN ) || ( e == FL_SHORTCUT ) )					//key pressed?
if ( ( e == FL_KEYDOWN ) )					//key pressed?
	{
	int key = Fl::event_key();
	
	if( ( key == FL_Control_L ) || (  key == FL_Control_R ) ) ctrl_key = 1;
	if( ( key == FL_Shift_L ) || (  key == FL_Shift_R ) ) shift_key = 1;
	
/*
	
	bool b_allow_x_reposition = 1;
	bool b_allow_y_reposition = 1;
	bool b_allow_wid_resize = 1;
	bool b_allow_hei_resize = 1;
	int xx = x();
	int yy = y();
	int ww = w();
	int hh = h();

	int border = 3;
	if( (ctrl_key) && (!shift_key) )
		{
		if( b_allow_x_reposition )
			{
			if( key == FL_Left )
				{
				if( xx > border ) position( xx - 1, yy );
				move_butt_dim_into_edit_box();
				}

			if( key == FL_Right )
				{
				position( xx + 1, yy );
				move_butt_dim_into_edit_box();
				}
			}


		if( b_allow_y_reposition )
			{
			if( key == FL_Up )
				{
				if( yy > border ) position( xx, yy - 1 );
				move_butt_dim_into_edit_box();
				}

			if( key == FL_Down )
				{
				position(  xx, yy + 1 );
				move_butt_dim_into_edit_box();
				}
			}

		}
		
	if( (ctrl_key) && (shift_key) )
		{
		if( b_allow_wid_resize )
			{
			if( key == FL_Left )
				{
				if( ww > 2 ) size( ww - 1, hh );
				move_butt_dim_into_edit_box();
				}

			if( key == FL_Right )
				{
				size( ww + 1, hh );
				move_butt_dim_into_edit_box();
				}
			}

		if( b_allow_hei_resize )
			{
			if( key == FL_Up )
				{
				if( hh > 2 ) size( ww, hh - 1 );
				move_butt_dim_into_edit_box();
				}

			if( key == FL_Down )
				{
				size( ww, hh + 1 );
				move_butt_dim_into_edit_box();
				}
			}
		}
*/
	need_redraw = 1;
	dont_pass_on = 0;
	}


if ( e == FL_KEYUP )												//key release?
	{
	int key = Fl::event_key();
	
	if( ( key == FL_Control_L ) || ( key == FL_Control_R ) ) ctrl_key = 0;
	if( ( key == FL_Shift_L ) || (  key == FL_Shift_R ) ) shift_key = 0;

	need_redraw = 1;
	dont_pass_on = 0;
	}


if ( e == FL_MOUSEWHEEL )
	{
//	mousewheel = Fl::event_dy();
	
	need_redraw = 1;
	dont_pass_on = 0;
	}
	
if ( need_redraw ) redraw();

if( dont_pass_on ) return 1;

return Fl_Button::handle(e);
}

//----------------------------------------------------------


