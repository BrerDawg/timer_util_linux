// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "fluid.h"
fluid_wnd *wnd_fluid; 

Fl_Button *bt_button0=(Fl_Button *)0;

Fl_Input *fi_remaining_tim0=(Fl_Input *)0;

Fl_Button *bt_start0=(Fl_Button *)0;

Fl_Input *fi_cmd0=(Fl_Input *)0;

Fl_Button *bt_test0=(Fl_Button *)0;

Fl_Button *bt_num0=(Fl_Button *)0;

Fl_Button *bt_num7=(Fl_Button *)0;

Fl_Button *bt_num8=(Fl_Button *)0;

Fl_Button *bt_num9=(Fl_Button *)0;

Fl_Button *bt_num4=(Fl_Button *)0;

Fl_Button *bt_num5=(Fl_Button *)0;

Fl_Button *bt_num6=(Fl_Button *)0;

Fl_Button *bt_num1=(Fl_Button *)0;

Fl_Button *bt_num2=(Fl_Button *)0;

Fl_Button *bt_num3=(Fl_Button *)0;

Fl_Button *bt_clr=(Fl_Button *)0;

Fl_Button *bt_reset0=(Fl_Button *)0;

Fl_Input *fi_interval_tim0=(Fl_Input *)0;

Fl_Check_Button *ck_cycle0=(Fl_Check_Button *)0;

Fl_Check_Button *ck_beep0=(Fl_Check_Button *)0;

Fl_Check_Button *ck_wintop0=(Fl_Check_Button *)0;

Fl_Round_Button *bt_sel0=(Fl_Round_Button *)0;

Fl_Round_Button *bt_sel1=(Fl_Round_Button *)0;

Fl_Round_Button *bt_sel2=(Fl_Round_Button *)0;

Fl_Input *fi_beeps0=(Fl_Input *)0;

Fl_Input *fi_pitch0=(Fl_Input *)0;

Fl_Button *bt_load=(Fl_Button *)0;

Fl_Button *bt_save=(Fl_Button *)0;

button_mouse_click *bt_dur15=(button_mouse_click *)0;

button_mouse_click *bt_dur30=(button_mouse_click *)0;

button_mouse_click *bt_dur5=(button_mouse_click *)0;

button_mouse_click *bt_dur1=(button_mouse_click *)0;

button_mouse_click *bt_dur45=(button_mouse_click *)0;

button_mouse_click *bt_dur60=(button_mouse_click *)0;

button_mouse_click *bt_dur10=(button_mouse_click *)0;

Fl_Check_Button *ck_cmd_on0=(Fl_Check_Button *)0;

button_mouse_click *bt_dur20=(button_mouse_click *)0;

Fl_Input *fi_remaining_tim1=(Fl_Input *)0;

Fl_Button *bt_start1=(Fl_Button *)0;

Fl_Input *fi_cmd1=(Fl_Input *)0;

Fl_Button *bt_test1=(Fl_Button *)0;

Fl_Button *bt_reset1=(Fl_Button *)0;

Fl_Input *fi_interval_tim1=(Fl_Input *)0;

Fl_Check_Button *ck_cycle1=(Fl_Check_Button *)0;

Fl_Check_Button *ck_beep1=(Fl_Check_Button *)0;

Fl_Check_Button *ck_wintop1=(Fl_Check_Button *)0;

Fl_Input *fi_beeps1=(Fl_Input *)0;

Fl_Input *fi_pitch1=(Fl_Input *)0;

Fl_Check_Button *ck_cmd_on1=(Fl_Check_Button *)0;

Fl_Input *fi_remaining_tim2=(Fl_Input *)0;

Fl_Button *bt_start2=(Fl_Button *)0;

Fl_Input *fi_cmd2=(Fl_Input *)0;

Fl_Button *bt_test2=(Fl_Button *)0;

Fl_Button *bt_reset2=(Fl_Button *)0;

Fl_Input *fi_interval_tim2=(Fl_Input *)0;

Fl_Check_Button *ck_cycle2=(Fl_Check_Button *)0;

Fl_Check_Button *ck_beep2=(Fl_Check_Button *)0;

Fl_Check_Button *ck_wintop2=(Fl_Check_Button *)0;

Fl_Input *fi_beeps2=(Fl_Input *)0;

Fl_Input *fi_pitch2=(Fl_Input *)0;

Fl_Check_Button *ck_cmd_on2=(Fl_Check_Button *)0;

Fl_Input *fi_time_now=(Fl_Input *)0;

Fl_Box *bx_interval_eta0=(Fl_Box *)0;

Fl_Box *bx_eta0=(Fl_Box *)0;

Fl_Box *bx_interval_eta1=(Fl_Box *)0;

Fl_Box *bx_eta1=(Fl_Box *)0;

Fl_Box *bx_interval_eta2=(Fl_Box *)0;

Fl_Box *bx_eta2=(Fl_Box *)0;

Fl_Box *bx_ctrl_minus=(Fl_Box *)0;

Fl_Button *bt_minus=(Fl_Button *)0;

Fl_Button *bt_about=(Fl_Button *)0;

Fl_Button *bt_trim_01=(Fl_Button *)0;

Fl_Button *bt_trim01=(Fl_Button *)0;

Fl_Button *bt_trim05=(Fl_Button *)0;

Fl_Button *bt_trim_05=(Fl_Button *)0;

Fl_Button *bt_trim_11=(Fl_Button *)0;

Fl_Button *bt_trim11=(Fl_Button *)0;

Fl_Button *bt_trim15=(Fl_Button *)0;

Fl_Button *bt_trim_15=(Fl_Button *)0;

Fl_Button *bt_trim_21=(Fl_Button *)0;

Fl_Button *bt_trim21=(Fl_Button *)0;

Fl_Button *bt_trim25=(Fl_Button *)0;

Fl_Button *bt_trim_25=(Fl_Button *)0;

fluid_wnd* make_fluid_window() {
  fluid_wnd* w;
  { fluid_wnd* o = new fluid_wnd(0, 0, 600, 295, " ~~oO=====   timer_util_linux   =====Oo~~");
    w = o; if (w) {/* empty */}
    o->box(FL_FLAT_BOX);
    o->color(FL_BACKGROUND_COLOR);
    o->selection_color(FL_BACKGROUND_COLOR);
    o->labeltype(FL_NO_LABEL);
    o->labelfont(0);
    o->labelsize(14);
    o->labelcolor(FL_FOREGROUND_COLOR);
    o->align(Fl_Align(FL_ALIGN_TOP));
    o->when(FL_WHEN_RELEASE);
    { bt_button0 = new Fl_Button(290, 230, 115, 20, "bt_button0");
    } // Fl_Button* bt_button0
    { fi_remaining_tim0 = new Fl_Input(115, 20, 75, 20, "remaining_tim0");
      fi_remaining_tim0->labelsize(9);
      fi_remaining_tim0->textsize(11);
      fi_remaining_tim0->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Input* fi_remaining_tim0
    { bt_start0 = new Fl_Button(200, 20, 40, 20, "start0");
      bt_start0->labelsize(9);
    } // Fl_Button* bt_start0
    { fi_cmd0 = new Fl_Input(395, 1, 155, 34, "cmd0");
      fi_cmd0->labelsize(12);
      fi_cmd0->textsize(11);
    } // Fl_Input* fi_cmd0
    { bt_test0 = new Fl_Button(555, 1, 40, 20, "test0");
      bt_test0->labelsize(9);
    } // Fl_Button* bt_test0
    { bt_num0 = new Fl_Button(31, 270, 20, 20, "num0");
      bt_num0->labelsize(9);
    } // Fl_Button* bt_num0
    { bt_num7 = new Fl_Button(31, 210, 20, 20, "num7");
      bt_num7->labelsize(9);
    } // Fl_Button* bt_num7
    { bt_num8 = new Fl_Button(51, 210, 20, 20, "num8");
      bt_num8->labelsize(9);
    } // Fl_Button* bt_num8
    { bt_num9 = new Fl_Button(71, 210, 20, 20, "num9");
      bt_num9->labelsize(9);
    } // Fl_Button* bt_num9
    { bt_num4 = new Fl_Button(31, 230, 20, 20, "num4");
      bt_num4->labelsize(9);
    } // Fl_Button* bt_num4
    { bt_num5 = new Fl_Button(51, 230, 20, 20, "num5");
      bt_num5->labelsize(9);
    } // Fl_Button* bt_num5
    { bt_num6 = new Fl_Button(71, 230, 20, 20, "num6");
      bt_num6->labelsize(9);
    } // Fl_Button* bt_num6
    { bt_num1 = new Fl_Button(31, 250, 20, 20, "num1");
      bt_num1->labelsize(9);
    } // Fl_Button* bt_num1
    { bt_num2 = new Fl_Button(51, 250, 20, 20, "num2");
      bt_num2->labelsize(9);
    } // Fl_Button* bt_num2
    { bt_num3 = new Fl_Button(71, 250, 20, 20, "num3");
      bt_num3->labelsize(9);
    } // Fl_Button* bt_num3
    { bt_clr = new Fl_Button(51, 270, 40, 20, "clear");
      bt_clr->labelsize(9);
    } // Fl_Button* bt_clr
    { bt_reset0 = new Fl_Button(245, 20, 40, 20, "reset0");
      bt_reset0->labelsize(9);
    } // Fl_Button* bt_reset0
    { fi_interval_tim0 = new Fl_Input(30, 20, 75, 20, "interval_tim0");
      fi_interval_tim0->labelsize(9);
      fi_interval_tim0->textsize(11);
      fi_interval_tim0->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Input* fi_interval_tim0
    { ck_cycle0 = new Fl_Check_Button(290, 5, 15, 15, "cycle0");
      ck_cycle0->down_box(FL_DOWN_BOX);
      ck_cycle0->labelsize(9);
    } // Fl_Check_Button* ck_cycle0
    { ck_beep0 = new Fl_Check_Button(290, 20, 15, 15, "beep0");
      ck_beep0->down_box(FL_DOWN_BOX);
      ck_beep0->labelsize(9);
    } // Fl_Check_Button* ck_beep0
    { ck_wintop0 = new Fl_Check_Button(290, 35, 15, 15, "wintop0");
      ck_wintop0->down_box(FL_DOWN_BOX);
      ck_wintop0->labelsize(9);
    } // Fl_Check_Button* ck_wintop0
    { bt_sel0 = new Fl_Round_Button(5, 17, 20, 25, "sel0");
      bt_sel0->down_box(FL_ROUND_DOWN_BOX);
    } // Fl_Round_Button* bt_sel0
    { bt_sel1 = new Fl_Round_Button(5, 95, 20, 25, "sel1");
      bt_sel1->down_box(FL_ROUND_DOWN_BOX);
    } // Fl_Round_Button* bt_sel1
    { bt_sel2 = new Fl_Round_Button(5, 170, 20, 25, "sel2");
      bt_sel2->down_box(FL_ROUND_DOWN_BOX);
    } // Fl_Round_Button* bt_sel2
    { fi_beeps0 = new Fl_Input(395, 35, 30, 18, "beeps0");
      fi_beeps0->labelsize(12);
      fi_beeps0->textsize(11);
    } // Fl_Input* fi_beeps0
    { fi_pitch0 = new Fl_Input(470, 35, 55, 18, "pitch0");
      fi_pitch0->labelsize(12);
      fi_pitch0->textsize(11);
    } // Fl_Input* fi_pitch0
    { bt_load = new Fl_Button(505, 270, 40, 20, "load");
      bt_load->labelsize(9);
    } // Fl_Button* bt_load
    { bt_save = new Fl_Button(555, 270, 40, 20, "save");
      bt_save->labelsize(9);
    } // Fl_Button* bt_save
    { bt_dur15 = new button_mouse_click(100, 250, 45, 20, "dur15");
      bt_dur15->box(FL_UP_BOX);
      bt_dur15->color(FL_BACKGROUND_COLOR);
      bt_dur15->selection_color(FL_BACKGROUND_COLOR);
      bt_dur15->labeltype(FL_NORMAL_LABEL);
      bt_dur15->labelfont(0);
      bt_dur15->labelsize(9);
      bt_dur15->labelcolor(FL_FOREGROUND_COLOR);
      bt_dur15->align(Fl_Align(FL_ALIGN_CENTER));
      bt_dur15->when(FL_WHEN_RELEASE);
    } // button_mouse_click* bt_dur15
    { bt_dur30 = new button_mouse_click(100, 230, 45, 20, "dur30");
      bt_dur30->box(FL_UP_BOX);
      bt_dur30->color(FL_BACKGROUND_COLOR);
      bt_dur30->selection_color(FL_BACKGROUND_COLOR);
      bt_dur30->labeltype(FL_NORMAL_LABEL);
      bt_dur30->labelfont(0);
      bt_dur30->labelsize(9);
      bt_dur30->labelcolor(FL_FOREGROUND_COLOR);
      bt_dur30->align(Fl_Align(FL_ALIGN_CENTER));
      bt_dur30->when(FL_WHEN_RELEASE);
    } // button_mouse_click* bt_dur30
    { bt_dur5 = new button_mouse_click(100, 270, 45, 20, "dur5");
      bt_dur5->box(FL_UP_BOX);
      bt_dur5->color(FL_BACKGROUND_COLOR);
      bt_dur5->selection_color(FL_BACKGROUND_COLOR);
      bt_dur5->labeltype(FL_NORMAL_LABEL);
      bt_dur5->labelfont(0);
      bt_dur5->labelsize(9);
      bt_dur5->labelcolor(FL_FOREGROUND_COLOR);
      bt_dur5->align(Fl_Align(FL_ALIGN_CENTER));
      bt_dur5->when(FL_WHEN_RELEASE);
    } // button_mouse_click* bt_dur5
    { bt_dur1 = new button_mouse_click(145, 270, 45, 20, "dur1");
      bt_dur1->box(FL_UP_BOX);
      bt_dur1->color(FL_BACKGROUND_COLOR);
      bt_dur1->selection_color(FL_BACKGROUND_COLOR);
      bt_dur1->labeltype(FL_NORMAL_LABEL);
      bt_dur1->labelfont(0);
      bt_dur1->labelsize(9);
      bt_dur1->labelcolor(FL_FOREGROUND_COLOR);
      bt_dur1->align(Fl_Align(FL_ALIGN_CENTER));
      bt_dur1->when(FL_WHEN_RELEASE);
    } // button_mouse_click* bt_dur1
    { bt_dur45 = new button_mouse_click(146, 210, 45, 20, "dur45");
      bt_dur45->box(FL_UP_BOX);
      bt_dur45->color(FL_BACKGROUND_COLOR);
      bt_dur45->selection_color(FL_BACKGROUND_COLOR);
      bt_dur45->labeltype(FL_NORMAL_LABEL);
      bt_dur45->labelfont(0);
      bt_dur45->labelsize(9);
      bt_dur45->labelcolor(FL_FOREGROUND_COLOR);
      bt_dur45->align(Fl_Align(FL_ALIGN_CENTER));
      bt_dur45->when(FL_WHEN_RELEASE);
    } // button_mouse_click* bt_dur45
    { bt_dur60 = new button_mouse_click(100, 210, 45, 20, "dur60");
      bt_dur60->tooltip("right click to subtract");
      bt_dur60->box(FL_UP_BOX);
      bt_dur60->color(FL_BACKGROUND_COLOR);
      bt_dur60->selection_color(FL_BACKGROUND_COLOR);
      bt_dur60->labeltype(FL_NORMAL_LABEL);
      bt_dur60->labelfont(0);
      bt_dur60->labelsize(9);
      bt_dur60->labelcolor(FL_FOREGROUND_COLOR);
      bt_dur60->align(Fl_Align(FL_ALIGN_CENTER));
      bt_dur60->when(FL_WHEN_RELEASE);
    } // button_mouse_click* bt_dur60
    { bt_dur10 = new button_mouse_click(145, 250, 45, 20, "dur10");
      bt_dur10->box(FL_UP_BOX);
      bt_dur10->color(FL_BACKGROUND_COLOR);
      bt_dur10->selection_color(FL_BACKGROUND_COLOR);
      bt_dur10->labeltype(FL_NORMAL_LABEL);
      bt_dur10->labelfont(0);
      bt_dur10->labelsize(9);
      bt_dur10->labelcolor(FL_FOREGROUND_COLOR);
      bt_dur10->align(Fl_Align(FL_ALIGN_CENTER));
      bt_dur10->when(FL_WHEN_RELEASE);
    } // button_mouse_click* bt_dur10
    { ck_cmd_on0 = new Fl_Check_Button(290, 49, 15, 15, "cmd_on0");
      ck_cmd_on0->down_box(FL_DOWN_BOX);
      ck_cmd_on0->labelsize(9);
    } // Fl_Check_Button* ck_cmd_on0
    { bt_dur20 = new button_mouse_click(146, 230, 45, 20, "dur20");
      bt_dur20->box(FL_UP_BOX);
      bt_dur20->color(FL_BACKGROUND_COLOR);
      bt_dur20->selection_color(FL_BACKGROUND_COLOR);
      bt_dur20->labeltype(FL_NORMAL_LABEL);
      bt_dur20->labelfont(0);
      bt_dur20->labelsize(9);
      bt_dur20->labelcolor(FL_FOREGROUND_COLOR);
      bt_dur20->align(Fl_Align(FL_ALIGN_CENTER));
      bt_dur20->when(FL_WHEN_RELEASE);
    } // button_mouse_click* bt_dur20
    { fi_remaining_tim1 = new Fl_Input(115, 96, 75, 20, "remaining_tim1");
      fi_remaining_tim1->labelsize(9);
      fi_remaining_tim1->textsize(11);
      fi_remaining_tim1->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Input* fi_remaining_tim1
    { bt_start1 = new Fl_Button(200, 96, 40, 20, "start1");
      bt_start1->labelsize(9);
    } // Fl_Button* bt_start1
    { fi_cmd1 = new Fl_Input(395, 77, 155, 34, "cmd1");
      fi_cmd1->labelsize(12);
      fi_cmd1->textsize(11);
    } // Fl_Input* fi_cmd1
    { bt_test1 = new Fl_Button(555, 77, 40, 20, "test1");
      bt_test1->labelsize(9);
    } // Fl_Button* bt_test1
    { bt_reset1 = new Fl_Button(245, 96, 40, 20, "reset1");
      bt_reset1->labelsize(9);
    } // Fl_Button* bt_reset1
    { fi_interval_tim1 = new Fl_Input(30, 96, 75, 20, "interval_tim1");
      fi_interval_tim1->labelsize(9);
      fi_interval_tim1->textsize(11);
      fi_interval_tim1->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Input* fi_interval_tim1
    { ck_cycle1 = new Fl_Check_Button(290, 81, 15, 15, "cycle1");
      ck_cycle1->down_box(FL_DOWN_BOX);
      ck_cycle1->labelsize(9);
    } // Fl_Check_Button* ck_cycle1
    { ck_beep1 = new Fl_Check_Button(290, 96, 15, 15, "beep1");
      ck_beep1->down_box(FL_DOWN_BOX);
      ck_beep1->labelsize(9);
    } // Fl_Check_Button* ck_beep1
    { ck_wintop1 = new Fl_Check_Button(290, 111, 15, 15, "wintop1");
      ck_wintop1->down_box(FL_DOWN_BOX);
      ck_wintop1->labelsize(9);
    } // Fl_Check_Button* ck_wintop1
    { fi_beeps1 = new Fl_Input(395, 111, 30, 18, "beeps1");
      fi_beeps1->labelsize(12);
      fi_beeps1->textsize(11);
    } // Fl_Input* fi_beeps1
    { fi_pitch1 = new Fl_Input(470, 111, 55, 18, "pitch1");
      fi_pitch1->labelsize(12);
      fi_pitch1->textsize(11);
    } // Fl_Input* fi_pitch1
    { ck_cmd_on1 = new Fl_Check_Button(290, 125, 15, 15, "cmd_on1");
      ck_cmd_on1->down_box(FL_DOWN_BOX);
      ck_cmd_on1->labelsize(9);
    } // Fl_Check_Button* ck_cmd_on1
    { fi_remaining_tim2 = new Fl_Input(115, 171, 75, 20, "remaining_tim2");
      fi_remaining_tim2->labelsize(9);
      fi_remaining_tim2->textsize(11);
      fi_remaining_tim2->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Input* fi_remaining_tim2
    { bt_start2 = new Fl_Button(200, 171, 40, 20, "start2");
      bt_start2->labelsize(9);
    } // Fl_Button* bt_start2
    { fi_cmd2 = new Fl_Input(395, 152, 155, 34, "cmd2");
      fi_cmd2->labelsize(12);
      fi_cmd2->textsize(11);
    } // Fl_Input* fi_cmd2
    { bt_test2 = new Fl_Button(555, 152, 40, 20, "test2");
      bt_test2->labelsize(9);
    } // Fl_Button* bt_test2
    { bt_reset2 = new Fl_Button(245, 171, 40, 20, "reset2");
      bt_reset2->labelsize(9);
    } // Fl_Button* bt_reset2
    { fi_interval_tim2 = new Fl_Input(30, 171, 75, 20, "interval_tim2");
      fi_interval_tim2->labelsize(9);
      fi_interval_tim2->textsize(11);
      fi_interval_tim2->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Input* fi_interval_tim2
    { ck_cycle2 = new Fl_Check_Button(290, 156, 15, 15, "cycle2");
      ck_cycle2->down_box(FL_DOWN_BOX);
      ck_cycle2->labelsize(9);
    } // Fl_Check_Button* ck_cycle2
    { ck_beep2 = new Fl_Check_Button(290, 171, 15, 15, "beep2");
      ck_beep2->down_box(FL_DOWN_BOX);
      ck_beep2->labelsize(9);
    } // Fl_Check_Button* ck_beep2
    { ck_wintop2 = new Fl_Check_Button(290, 186, 15, 15, "wintop2");
      ck_wintop2->down_box(FL_DOWN_BOX);
      ck_wintop2->labelsize(9);
    } // Fl_Check_Button* ck_wintop2
    { fi_beeps2 = new Fl_Input(395, 186, 30, 18, "beeps2");
      fi_beeps2->labelsize(12);
      fi_beeps2->textsize(11);
    } // Fl_Input* fi_beeps2
    { fi_pitch2 = new Fl_Input(470, 186, 55, 18, "pitch2");
      fi_pitch2->labelsize(12);
      fi_pitch2->textsize(11);
    } // Fl_Input* fi_pitch2
    { ck_cmd_on2 = new Fl_Check_Button(290, 200, 15, 15, "cmd_on2");
      ck_cmd_on2->down_box(FL_DOWN_BOX);
      ck_cmd_on2->labelsize(9);
    } // Fl_Check_Button* ck_cmd_on2
    { fi_time_now = new Fl_Input(315, 270, 75, 20, "time_now");
      fi_time_now->labelsize(9);
      fi_time_now->textsize(11);
      fi_time_now->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Input* fi_time_now
    { bx_interval_eta0 = new Fl_Box(30, 40, 75, 20, "interval_eta0");
      bx_interval_eta0->tooltip("real time that time out will occur at");
      bx_interval_eta0->labelsize(10);
    } // Fl_Box* bx_interval_eta0
    { bx_eta0 = new Fl_Box(115, 40, 75, 20, "eta0");
      bx_eta0->tooltip("real time that time out will occur at");
      bx_eta0->labelsize(10);
    } // Fl_Box* bx_eta0
    { bx_interval_eta1 = new Fl_Box(30, 115, 75, 20, "interval_eta1");
      bx_interval_eta1->tooltip("real time that time out will occur at");
      bx_interval_eta1->labelsize(10);
    } // Fl_Box* bx_interval_eta1
    { bx_eta1 = new Fl_Box(115, 115, 75, 20, "eta1");
      bx_eta1->tooltip("real time that time out will occur at");
      bx_eta1->labelsize(10);
    } // Fl_Box* bx_eta1
    { bx_interval_eta2 = new Fl_Box(30, 190, 75, 20, "interval_eta2");
      bx_interval_eta2->tooltip("real time that time out will occur at");
      bx_interval_eta2->labelsize(10);
    } // Fl_Box* bx_interval_eta2
    { bx_eta2 = new Fl_Box(115, 190, 75, 20, "eta2");
      bx_eta2->tooltip("real time that time out will occur at");
      bx_eta2->labelsize(10);
    } // Fl_Box* bx_eta2
    { bx_ctrl_minus = new Fl_Box(195, 270, 80, 20, "ctrl_minus");
      bx_ctrl_minus->tooltip("real time that time out will occur at");
      bx_ctrl_minus->labelsize(10);
    } // Fl_Box* bx_ctrl_minus
    { bt_minus = new Fl_Button(278, 270, 25, 20, "minus");
      bt_minus->labelsize(9);
    } // Fl_Button* bt_minus
    { bt_about = new Fl_Button(430, 270, 40, 20, "about");
      bt_about->labelsize(9);
    } // Fl_Button* bt_about
    { bt_trim_01 = new Fl_Button(225, 45, 15, 15, "-01");
      bt_trim_01->labelsize(8);
    } // Fl_Button* bt_trim_01
    { bt_trim01 = new Fl_Button(245, 45, 15, 15, "01");
      bt_trim01->labelsize(8);
    } // Fl_Button* bt_trim01
    { bt_trim05 = new Fl_Button(265, 45, 15, 15, "05");
      bt_trim05->labelsize(8);
    } // Fl_Button* bt_trim05
    { bt_trim_05 = new Fl_Button(205, 45, 15, 15, "-05");
      bt_trim_05->labelsize(8);
    } // Fl_Button* bt_trim_05
    { bt_trim_11 = new Fl_Button(225, 120, 15, 15, "-11");
      bt_trim_11->labelsize(8);
    } // Fl_Button* bt_trim_11
    { bt_trim11 = new Fl_Button(245, 120, 15, 15, "11");
      bt_trim11->labelsize(8);
    } // Fl_Button* bt_trim11
    { bt_trim15 = new Fl_Button(265, 120, 15, 15, "15");
      bt_trim15->labelsize(8);
    } // Fl_Button* bt_trim15
    { bt_trim_15 = new Fl_Button(205, 120, 15, 15, "-15");
      bt_trim_15->labelsize(8);
    } // Fl_Button* bt_trim_15
    { bt_trim_21 = new Fl_Button(225, 195, 15, 15, "-21");
      bt_trim_21->labelsize(8);
    } // Fl_Button* bt_trim_21
    { bt_trim21 = new Fl_Button(245, 195, 15, 15, "21");
      bt_trim21->labelsize(8);
    } // Fl_Button* bt_trim21
    { bt_trim25 = new Fl_Button(265, 195, 15, 15, "25");
      bt_trim25->labelsize(8);
    } // Fl_Button* bt_trim25
    { bt_trim_25 = new Fl_Button(205, 195, 15, 15, "-25");
      bt_trim_25->labelsize(8);
    } // Fl_Button* bt_trim_25
    o->end();
  } // fluid_wnd* o
  return w;
}
