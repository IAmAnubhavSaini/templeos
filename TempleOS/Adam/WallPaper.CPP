#help_index "Windows"

U0 WallPrintF(I64 attr=-1,I64 x,I64 y,U8 *fmt,...)
{ //Be careful, this is not fool-proof in x and can overflow.
  U8 buf[256];
  if (0<=y<TEXT_ROWS) {
    if (attr<0)
      attr=BLUE<<4+WHITE;
    else
      attr=BLUE<<4+attr;
    SPrintFJoin(buf,fmt,argc,argv);
    buf[TEXT_COLS-x]=0;
    PlotScreenStr(buf,attr,x,y);
  }
}

I64 last_unused_sysmem=0,last_calc_refresh_cnt=0,
    last_swap_cnter2[MP_MAX_PROCESSORS];
MemSet(last_swap_cnter2,0,sizeof(last_swap_cnter2));
F64 last_refresh_delta_time=1.0;
Bool wallpaper_record_on=FALSE;

class CTaskWallPaperData
{
  I64 allocated_u8s,used_u8s;
  U8 caller_stk[TASK_WALLPAPER_DATA_SIZE-16];
};

U0 WallPaper(CTask *_task)
{
  I64 i,j,l=TEXT_ROWS-1;
  CTask *task;
  CTaskWallPaperData *wpd;
  CHashTable *old_hash=Fs->hash_table;
  CCPU *c;
  Bool old_preempt=Preempt(OFF);
  CDate cdt;
  U8 buf[256],*st;
  static I64 code_heap_unused=0,code_heap_used=0,
	     data_heap_unused=0,data_heap_used=0;
  _task->text_attr=BLUE<<4+WHITE;
  WallPrintF(,0,l--,"Reads:%d Writes:%d lines=%d funs=%d bytes=%010X",
    sys_dsk_reads,sys_dsk_writes,compiled_lines,compiled_funs,compiled_bytes);
  WallPrintF(,0,l--,"progress1=%016X progress2=%016X",progress1,progress2);
  WallPrintF(,0,l--,"progress3=%016X progress4=%016X",progress3,progress4);
  l--;
  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    task=c->seth_task;
    do {
      wpd=&task->wallpaper_data;
      if (!TaskValidate(task)) break;
      if (win_updates&31==15) {
	if (c==Gs)
	  Fs->hash_table=task->hash_table;
	else //precaution
	  Fs->hash_table=old_hash;
	SPrintF(wpd->caller_stk,"%19tP %19tP %19tP %19tP",
	  TaskCaller(task,0),TaskCaller(task,1),
	  TaskCaller(task,2),TaskCaller(task,3));
      }
      WallPrintF(YELLOW,0,l--,wpd->caller_stk);

      j=UnusedStk(task);
      if (j<0)
	StkOverflow(task);
      if (task->user_num)
	SPrintF(buf,"#%02td",task->user_num);
      else
	StrCpy(buf,"");
      if (!(win_updates&63)) {
	wpd->allocated_u8s=TaskAllocatedMem(task);
	wpd->used_u8s	  =TaskUsedMem(task);
      }
      WallPrintF(,0,l--,"%s %08X%010X %010X %010X %-16ts %05X:%02tX %6.2fm",buf,task,j,
	wpd->allocated_u8s,wpd->used_u8s,task->task_title,
	task->task_flags,task->display_flags,
	task->total_time/60.0/time_stamp_freq_initial);
      if (!TaskValidate(task)) break;
      task=task->next_task;
    } while (task!=c->seth_task);
  }
  WallPrintF(,0,l--,"____Task____ UnusedStk_ _AllocMem_ _UsedMem__ __Description___ _Flags__ _Time__");
  l--;
  if (win_updates&63==32) {
    code_heap_unused=UnusedBPMem(sys_code_bp);
    code_heap_used  =sys_code_bp->allocated_u8s;
    if (sys_data_bp) {
      data_heap_unused=UnusedBPMem(sys_data_bp);
      data_heap_used	=sys_data_bp->allocated_u8s;
    }
  }

  if (sys_data_bp) {
    WallPrintF(,0,l--,"Data Heap Unused:%010X Data Heap Used:%010X",data_heap_unused,data_heap_used);
    WallPrintF(,0,l--,"Code Heap Unused:%010X Code Heap Used:%010X",code_heap_unused,code_heap_used);
  } else
    WallPrintF(,0,l--,"Code/Data Heap Unused:%010X Used:%010X",code_heap_unused,code_heap_used);

  l--;
  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    WallPrintF(,0,l--,"%03d %9,d",i,
      ToI64((last_swap_cnter[i]-last_swap_cnter2[i])/last_refresh_delta_time));
    if (win_calc_refresh_cnt!=last_calc_refresh_cnt) {
      last_refresh_delta_time=refresh_delta_time;
      last_swap_cnter2[i]=last_swap_cnter[i];
      last_swap_cnter[i]=c->swap_cnter;
    }
  }

  WallPrintF(,0,l--,"CPUSwaps/s");
  WallPrintF(,0,l--,"Context");

  cdt=GetCurTimeDate;
  if (!(win_updates&63))
    last_unused_sysmem=UnusedSysMem+data_heap_unused;

  WallPrintF(,0,0,"%h2D %h4T Ref:%2tf Mem:%010X CPU",
    cdt,cdt,win_actual_refresh,last_unused_sysmem);
 
  if (wallpaper_record_on)
    WallPrintF(RED,45-4,0,"*");
  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    if (i&1)
      WallPrintF(WHITE,45+i*2,0,"%2tf",100.0*(1.0-c->idle_factor));
    else
      WallPrintF(YELLOW,45+i*2,0,"%2tf",100.0*(1.0-c->idle_factor));
  }

  st=ScanCode2KeyName(sys_last_down_scan_code);
  WallPrintF(YELLOW,80-18,0,"%18ts",st);
  Free(st);

  Fs->hash_table=old_hash;
  last_calc_refresh_cnt=win_calc_refresh_cnt;
  Preempt(old_preempt);
}

fp_wall_paper=&WallPaper;



$SP,"<1>",1$



U0 DrawTermButton(CDC *dc,CCtrl *c)
{
  if (Blink)
    SpritePlot3(dc,c->left,c->top,0,$IB,"<1>",1$);
}

U0 LeftClickTermButton(CCtrl *,I64,I64,Bool down)
{
  if (down)
    SpawnUser;
}

CCtrl *TermButtonNew()
{
  Bool old_preempt;
  I64 min_x,max_x,min_y,max_y;
  CCtrl *c=ACAlloc(sizeof(CCtrl));

  c->win_task=sys_winmgr_task;
  c->flags=CTRLF_SHOW;
  c->type=CTRLT_GENERIC;
  c->draw_it=&DrawTermButton;
  c->left_click=&LeftClickTermButton;

  SpriteExtents($IB,"<1>",1$,&min_x,&max_x,&min_y,&max_y);
  //min must be zero
  c->left=sys_winmgr_task->win_pixel_width-(max_x-min_x+1);
  c->right=c->left+(max_x-min_x+1)-1;
  c->top=sys_winmgr_task->win_pixel_height-(max_y-min_y+1);
  c->bottom=c->top+(max_y-min_y+1)-1;

  old_preempt=Preempt(OFF);
  QueIns(c,sys_winmgr_task->last_ctrl);
  TaskDerivedValsUpdate(sys_winmgr_task);
  Preempt(old_preempt);

  return c;
}

TermButtonNew;


$SP,"<2>",2$

gr_empty_sprite=$IB,"<2>",2$;

$SP,"<3>",3$

$SP,"<4>",4$

#help_index "Graphics;Input Pointer/Displayed Image"
public U0 DrawGrabInputPtr(CDC *dc,I64 x,I64 y,Bool closed)
{
  dc->flags&=~(DCF_TRANSFORMATION|DCF_SYMMETRY);
  dc->color=ROP_EQU;
  if (closed)
    SpritePlot3(dc,x,y,0,$IB,"<3>",3$);
  else
    SpritePlot3(dc,x,y,0,$IB,"<4>",4$);
}
fp_draw_grab_input_ptr=&DrawGrabInputPtr;

#help_index ""
           7              (      	      $            Term            �            ����    �����  �����    �����  ����    ��     � ��    ��     �    ��      �    ���    ��    �     ��           ���          ��    ����  ��    �����  �    �����                      �      	      ����    ���     �       ��    �                                                        �       �       ��  �       ��  �       ���  �       ���         ���                        &                    ����      ���� 