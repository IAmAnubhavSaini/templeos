I64 box_x_min,box_x_max,box_y_min,box_y_max;

class Arrow
{
  Arrow *next,*last;
  F64 x,y,dx,dy;
} root;

   $SP,"<1>",1$

Bool bow_drawn;
F64 bow_x,bow_y,bow_�;

U0 DrawIt(CTask *task,CDC *dc)
{
  F64 �,x,y,dx,dy,
	   str_w,str_h,draw_len;
  Arrow *tempa;
  CD3I32 ctrl[5];
  dc->color=RED;
  GrLineRect4(dc,box_x_min,box_y_min,box_x_max,box_y_max);

  x=LimitI64(ipx-task->win_pixel_left-task->win_scroll_x,box_x_min,box_x_max);
  y=LimitI64(ipy-task->win_pixel_top-task->win_scroll_y, box_y_min,box_y_max);
  dx=bow_x-x;
  dy=bow_y-y;

  if (bow_drawn && (dx|dy))
    bow_�=Arg(dx,dy);
  else {
    bow_x=x;
    bow_y=y;
  }

  draw_len=Sqrt(dx*dx+dy*dy);
  str_w=draw_len/3;
  str_h=Sqrt(60*60-str_w*str_w);

  dc->color=BLACK;
  GrLine(dc,x-str_h/2*Cos(bow_�+�/2)+str_w*Cos(bow_�),
	      y-str_h/2*Sin(bow_�+�/2)+str_w*Sin(bow_�),
	      x,y);
  GrLine(dc,x+str_h/2*Cos(bow_�+�/2)+str_w*Cos(bow_�),
	      y+str_h/2*Sin(bow_�+�/2)+str_w*Sin(bow_�),
	      x,y);

  MemSet(ctrl,0,sizeof(ctrl));
  ctrl[0].x=x-str_h/2*Cos(bow_�+�/2)+str_w*Cos(bow_�);
  ctrl[0].y=y-str_h/2*Sin(bow_�+�/2)+str_w*Sin(bow_�);
  ctrl[1].x=x-0.75*str_h/2*Cos(bow_�+�/2)+draw_len/2*Cos(bow_�)+str_w*Cos(bow_�);
  ctrl[1].y=y-0.75*str_h/2*Sin(bow_�+�/2)+draw_len/2*Sin(bow_�)+str_w*Sin(bow_�);
  ctrl[2].x=x+draw_len/2*Cos(bow_�)+str_w*Cos(bow_�);
  ctrl[2].y=y+draw_len/2*Sin(bow_�)+str_w*Sin(bow_�);
  ctrl[3].x=x+0.75*str_h/2*Cos(bow_�+�/2)+draw_len/2*Cos(bow_�)+str_w*Cos(bow_�);
  ctrl[3].y=y+0.75*str_h/2*Sin(bow_�+�/2)+draw_len/2*Sin(bow_�)+str_w*Sin(bow_�);
  ctrl[4].x=x+str_h/2*Cos(bow_�+�/2)+str_w*Cos(bow_�);
  ctrl[4].y=y+str_h/2*Sin(bow_�+�/2)+str_w*Sin(bow_�);

  dc->color=BROWN;
  dc->pen_width=2;
  Gr2BSpline3(dc,ctrl,5);
  dc->pen_width=1;

  if (bow_drawn)
    SpritePlotRotZ3b(dc,x,y,0,$IB,"<1>",1$,bow_�);

  tempa=root.next;
  while (tempa!=&root) {
    �=Arg(tempa->dx,tempa->dy);
    SpritePlotRotZ3b(dc,tempa->x,tempa->y,0,$IB,"<1>",1$,�);
    tempa=tempa->next;
  }
}

#define ANIMATE_SLEEP_MS	10

U0 AnimateTask(I64)
{
  I64 x,y;
  Arrow *tempa,*tempa1;
  F64 dt,t0=tT;
  while (TRUE) {
    dt=tT-t0;
    t0=tT;
 
    x=LimitI64(ipx-Fs->parent_task->win_pixel_left-Fs->parent_task->win_scroll_x,
       box_x_min,box_x_max)+Fs->parent_task->win_pixel_left+Fs->parent_task->win_scroll_x;
    y=LimitI64(ipy-Fs->parent_task->win_pixel_top-Fs->parent_task->win_scroll_y,
       box_y_min,box_y_max)+Fs->parent_task->win_pixel_top+Fs->parent_task->win_scroll_y;
    if (ipx!=x || ipy!=y)
      SetIP(x,y);

    tempa=root.next;
    while (tempa!=&root) {
      tempa1=tempa->next;
      tempa->x+=tempa->dx*dt;
      tempa->y+=tempa->dy*dt;
      if (!(-Fs->parent_task->win_scroll_x<=tempa->x<Fs->parent_task->win_pixel_width-Fs->parent_task->win_scroll_x) ||
	  !(-Fs->parent_task->win_scroll_y<=tempa->y<Fs->parent_task->win_pixel_height-Fs->parent_task->win_scroll_y)) {
	QueRem(tempa);
	Free(tempa);
      }
      tempa=tempa1;
    }
    WinMgrSync;
  }
}

U0 Init()
{
  I64 w=Fs->win_pixel_width,
      h=Fs->win_pixel_height;
  QueInit(&root);
  bow_drawn=FALSE;
  box_x_min=7*w/16;
  box_y_min=6*h/8;
  box_x_max=9*w/16;
  box_y_max=7*h/8;
  bow_�=-�/2;
  bow_x=(box_x_min+box_x_max)/2;
  bow_y=(box_y_min+box_y_max)/2;
  SetIP(bow_x+Fs->win_pixel_left+Fs->win_scroll_x,
	bow_y+Fs->win_pixel_top+Fs->win_scroll_y);
}

U0 CleanUp()
{
  QueDel(&root,TRUE);
}

U0 Zing()
{
  I64 p1,p2;
  Arrow *tempa;
  MenuPush(
    "File {"
    "  Abort(,CH_SHIFT_ESC);"
    "  Exit(,CH_ESC);"
    "}"
    "Play {"
    "  Restart(,'\n');"
    "}"
  );
  SettingsPush; //See $LK,"SettingsPush","MN:SettingsPush"$
  Preempt(OFF);
  WinBorder;
  WinMax;
  WordStat;

  DocClear;

  Init;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  Fs->win_inhibit=WIF_ALL-WIF_GRAB_SCROLL;
  try {
    while (TRUE) {
      switch (GetMsg(&p1,&p2,1<<MSG_KEY_DOWN|1<<MSG_IP_L_DOWN|1<<MSG_IP_L_UP)) {
	case MSG_KEY_DOWN:
	  switch (p1) {
	    case '\n':
	      CleanUp;
	      Init;
	      break;
	    case CH_ESC:
	    case CH_SHIFT_ESC:
	      goto zi_done;
	  }
	  break;
	case MSG_IP_L_DOWN:
	  bow_x=p1;
	  bow_y=p2;
	  bow_drawn=TRUE;
	  break;
	case MSG_IP_L_UP:
	  if(p1-bow_x || p2-bow_y) {
	    tempa=MAlloc(sizeof(Arrow));
	    tempa->dx=10.0*(bow_x-p1);
	    tempa->dy=10.0*(bow_y-p2);
	    tempa->x=p1;
	    tempa->y=p2;
	    QueIns(tempa,root.last);
	    Noise(50,8000,10000);
	  }
	  bow_drawn=FALSE;
	  break;
      }
    }
zi_done:
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    CatchAll;
  SettingsPop;
  CleanUp;
  MenuPop;
}

Zing;
           �              1        5       2   ����2   ����2      2      5                         ����   ����              ��������������������   ����   ����   ����                     ����   ���� 