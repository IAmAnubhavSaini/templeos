//8 has graphics


$SP,"<1>",1$





U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,w=task->win_pixel_width,h=task->win_pixel_height;
  dc->color=BROWN;
  for (i=3*tM%20;i<w;i+=20)
    GrLine(dc,i,h,w,h-i/4);
  dc->color=BLACK;
  GrLine(dc,0,h-1,w,h-1);
  GrLine(dc,w-1,0,w-1,h);
}

U0 Song()
{
  CTask *task;

  SettingsPush; //See $LK,"SettingsPush","MN:SettingsPush"$
  Fs->text_attr=LTCYAN<<4+BLACK;
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  music_tempo=2.5;
  try {
  task=PopUpViewStr(
    "$LK-A,"BF:Matthew,4:19"$\n"
    "$LK-A,"BF:Luke,5:4"$\n"
    );
  TaskWait(task);
  SetWinHorz(Fs->win_left,Fs->win_right,task);
  SetWinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
  while (!ScanKey) {
	"$$CL$$$$BG,LTCYAN$$";
	Play("3qEsGEGEeF2G3BCqCeDEqBsERER",
		"$$FG$$Lord,\0I\0\0am\0\0feel\0ing\0just\0a\0bit\0un\0in\0spired.\n\0With\0\0\0\0");
	Play("qEsGEGEeF2G3BCqCeDEqBsERER",
		"net\0or\0\0with\0\0reel\0ing\0\0I\0keep\0get\0ting\0mired.\n\0\0\0\0\0");
	Play("qDetRAFqDGetFC2G3sBCBCqDF",
		"$$FG,RED$$Throw\0\0out\0your\0net.\n\0\0\0\0\0\0\0\0\0\0\0");
	PutSprite($IB,"<1>",1$);
	Play("DetRAFqDGetFC2G3sBCBCqDF",
		"Make\0\0you\0a\0bet.\n\0\0\0\0\0\0\0\0\0\0\0");
	NewLine(5);
  }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
                                     "      "   -      -      ;      ;      F      I      F   #   F   #   P      L      T      T      ]      ]      `      `      R      R      B      B      7   	   7   	   6      6      %      %                        U      X      V      Y      
   N      N      N      P      S      T      W      S      P      P      J      J      J      L      P      P      F      E      C      C      C      F      F      F      G      H      E      E      @      A      A      A      C      D      A      >      =      =      <      <      9      6      8      9      :      :      :      :      7      6      4      4      6      6      6      3      2      1      -      -      .      0      -      +      )      )      *      *      %      %      %      Y      U      R      T      W      Y      Z      ]      [       