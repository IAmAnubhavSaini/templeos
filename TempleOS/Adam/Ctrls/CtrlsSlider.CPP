class CMgdSCSlider
{
  CMgdSCSlider *next,*last;
  U8 *name;
  I64 num;
};

class CMgdSC
{
  CMgdSCSlider *next_slider,*last_slider;
  I64 num_sliders,range,spacing,border;
  U8 *glbl_name;
};


CMgdSC *PmtSliderCtrl()
{
  CMgdSCSlider *temps;
  U8 *st,pmt[136];
  CMgdSC *result=CAlloc(sizeof(CMgdSC));

  "$$FG,PURPLE$$Slider Ctrl$$FG$$\n\n";

  result->glbl_name=PmtStr("Glbl Struct Name (ENTER for No Glbl):");
  result->range=PmtI64	("Range   (%4d):",30);
  result->spacing=PmtI64("Spacing (%4d):",20);
  result->border=PmtI64 ("Border  (%4d):",2);

  result->next_slider=result->last_slider=&result->next_slider;
  while (TRUE) {
    SPrintF(pmt,"Slider #%d Name:",result->num_sliders+1);
    st=PmtStr(pmt);
    if (!*st) {
      Free(st);
      break;
    }
    temps=CAlloc(sizeof(CMgdSCSlider));
    temps->name=st;
    QueIns(temps,result->last_slider);
    temps->num=result->num_sliders++;
  }
  return result;
}

U0 MgdSliderCtrl(CDoc *doc)
{
  CTask *task;
  CMgdSC *m=NULL;
  CMgdSCSlider *temps;
  I64 i;

  DocUnlock(doc);
  DocRecalc(doc);
  DocCaptureUndo(doc,TRUE);

  task=SpawnUser("CMgdSC **_m=0x%X;*_m=PmtSliderCtrl;\n",&m);
  while (!m)
    Yield;

  DocPrintF(doc,
"#define SLIDER_RANGE	%d
#define SLIDER_SPACING %d
#define SLIDER_BORDER  %d

class CSliderState
{
",m->range,m->spacing,m->border);


  temps=m->next_slider;
  while (temps!=&m->next_slider) {
    DocPrintF(doc,"I64 %s;\n",temps->name);
    temps=temps->next;
  }
  if (*m->glbl_name)
    DocPrintF(doc,"} %s;\n",m->glbl_name);
  else
    DocPutS(doc,"};\n");


  DocPrintF(doc,
"
U0 DrawSliderCtrl(CDC *dc,CCtrl *c)
{
CSliderState *s=c->state;

dc->color=LTRED;
GrRect(dc, c->left,c->top,%d*SLIDER_SPACING+%d,SLIDER_SPACING*2+SLIDER_RANGE);
dc->color=BLUE;
GrRect(dc, c->left+SLIDER_BORDER,c->top+SLIDER_BORDER,
%d*SLIDER_SPACING+%d-2*SLIDER_BORDER,SLIDER_SPACING*2+SLIDER_RANGE-2*SLIDER_BORDER);
dc->color=BLACK;
",m->num_sliders+1,m->num_sliders,m->num_sliders+1,m->num_sliders);

  for (i=0;i<m->num_sliders;i++)
    DocPrintF(doc,
"GrLine(dc,c->left+%d*SLIDER_SPACING+%d,c->top+SLIDER_SPACING,
	      c->left+%d*SLIDER_SPACING+%d,c->top+SLIDER_SPACING+SLIDER_RANGE-1);
",i+1,i,i+1,i);

  DocPutS(doc,"dc->color=LTRED;\n");

  temps=m->next_slider;
  while (temps!=&m->next_slider) {
    DocPrintF(doc,
"GrPrintF(dc,c->left+%d*SLIDER_SPACING+%d-FONT_WIDTH/2,
c->top+SLIDER_SPACING+SLIDER_RANGE+3,
\"%%d\",s->%s*10/SLIDER_RANGE);\n",
      temps->num+1,temps->num,temps->name);
    temps=temps->next;
  }

  temps=m->next_slider;
  while (temps!=&m->next_slider) {
    DocPrintF(doc,"GrRect(dc,c->left+%d*SLIDER_SPACING+%d-3,c->top+SLIDER_SPACING+SLIDER_RANGE-1-s->%s-2,7,5);\n",
      temps->num+1,temps->num,temps->name);
    temps=temps->next;
  }
  DocPutS(doc,"dc->color=YELLOW;\n");

  temps=m->next_slider;
  while (temps!=&m->next_slider) {
    DocPrintF(doc,"GrRect(dc,c->left+%d*SLIDER_SPACING+%d-2,c->top+SLIDER_SPACING+SLIDER_RANGE-1-s->%s-1,5,3);\n",
      temps->num+1,temps->num,temps->name);
    temps=temps->next;
  }
  DocPrintF(doc,
"}

U0 UpdateDerivedSliderCtrl(CCtrl *c)
{
  CSliderState *s=c->state;
  c->left=c->win_task->win_pixel_width/2-(SLIDER_SPACING*3+2)/2;
  c->right=c->left+%d*SLIDER_SPACING+%d;
  c->top=c->win_task->win_pixel_height/2-(SLIDER_SPACING*2+SLIDER_RANGE)/2;
  c->bottom=c->top+SLIDER_SPACING*2+SLIDER_RANGE;
",m->num_sliders+1,m->num_sliders);

  temps=m->next_slider;
  while (temps!=&m->next_slider) {
    DocPrintF(doc,"s->%s=LimitI64(s->%s,0,SLIDER_RANGE-1);\n",temps->name,temps->name);
    temps=temps->next;
  }
  DocPutS(doc,
"}

U0 LeftClickSlider(CCtrl *c,I64 x,I64 y,Bool down)
{
nounusedwarn down;
CSliderState *s=c->state;
");

  temps=m->next_slider;
  while (temps!=&m->next_slider) {
    DocPutS(doc,"");
    if (temps!=m->next_slider)
      DocPutS(doc,"else");
    if (temps->next==&m->next_slider)
      DocPutS(doc,"\n");
    else
      DocPrintF(doc,"if (x<c->left+%d*SLIDER_SPACING+%d+SLIDER_SPACING/2)\n",
	temps->num+1,temps->num);
    DocPrintF(doc,"s->%s=SLIDER_RANGE-1-(y-(c->top+SLIDER_SPACING));\n",
       temps->name);
    temps=temps->next;
  }
 
  DocPutS(doc,
"if (c->update_derived_vals)
(*c->update_derived_vals)(c);
}

CCtrl *SliderNew()
{
Bool old_preempt;
CCtrl *c=CAlloc(sizeof(CCtrl));
c->win_task=Fs;
c->flags=CTRLF_SHOW|CTRLF_CAPTURE_LEFT_IP;
c->type=CTRLT_GENERIC;
");
  if (*m->glbl_name)
    DocPrintF(doc,"c->state=&%s;\n"
		"MemSet(&%s,0,sizeof(CSliderState));\n",m->glbl_name,m->glbl_name);
  else
    DocPutS(doc,	"c->state=CAlloc(sizeof(CSliderState));\n");
  DocPutS(doc,
"c->draw_it=&DrawSliderCtrl;
c->left_click=&LeftClickSlider;
c->update_derived_vals=&UpdateDerivedSliderCtrl;
old_preempt=Preempt(OFF);
QueIns(c,Fs->last_ctrl);
TaskDerivedValsUpdate;
Preempt(old_preempt);
return c;
}

U0 SliderDel(CCtrl *c)
{
Bool old_preempt=Preempt(OFF);
QueRem(c);
Preempt(old_preempt);
");
  if (!*m->glbl_name)
    DocPutS(doc,"Free(c->state);\n");
  DocPutS(doc,
"Free(c);
}
");

  Kill(task);  //This frees memory.

  AcctOneTimePopUp(ARf_MANAGE_SLIDER,
    "Modify the code to your heart's content.\n"
    "The code is no longer managed.\n");

  DocLock(doc);
}
