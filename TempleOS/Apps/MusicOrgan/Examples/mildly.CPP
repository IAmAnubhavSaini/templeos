//7 has graphics


$SP,"",1$










$SP,"",2$















F64 t0=tMBeat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 y,dt=tMBeat-t0;
  if (dt>=24.0)
    SpritePlot3(dc,0,0,0,$IB,"<2>",2$);
  else {
    y=Sin(2*�*(tM%1.0-0.5)/1.0);
    SpritePlot3(dc,0,5.0*y,0,$IB,"<1>",1$);
    SpritePlot3(dc,0,-5.0*y,0,$IB,"<2>",2$);
  }
}

U0 Song()
{
  SettingsPush; //See $LK,"SettingsPush","MN:SettingsPush"$
  Fs->text_attr=CYAN<<4+BLACK;
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  CTask *task=PopUpViewStr(
      "$LK-A,"BF:Mark,4:37"$\n"
      );
  TaskWait(task);
  SetWinHorz(Fs->win_left,Fs->win_right,task);
  SetWinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
  music_tempo=2.5;
  try {
    while (!ScanKey) {
      t0=tMBeat;
      DocClear;
      "$$FG,WHITE$$$$BG,CYAN$$\n";
      Play("3qEsGEGEqDsCCCCqBsDEDEqFG",
	  "Lord,\0there's\0\0a\0\0storm\0u\0\0\0\0pon\0the\0\0\0\0sea.\n\0\0");
      Play("EsGEGEqDsCCCCqBsDEDEqFG",
	  "Lord,\0there's\0\0a\0\0storm\0u\0\0\0\0pon\0the\0\0\0\0sea.\n\0\0");
      Play("GEC2G3etB2G3FGGEDBBeBE",
	  "Re\0lax\0fel\0las.\n\0\0\0\0\0\0\0\0\0\0\0\0");
      Play("qGEC2G3etB2G3FGGEDBBeBE",
	  "(Sea\0be\0came\0glass.)\n\0\0\0\0\0\0\0\0\0\0\0\0");
    }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
           �         �   i   %   �M��y�&I�4I�$I�$I�!I�$)�$I&)�"I�$I�$I�$I�<ϓ4I��y�6I�$i�&��y   i   #   �۶m^]ם���aذmða[6l۶m۶m۶m۶m۶m۶mۼ��   �      $M�$I�$I�$I�$I�$I�$I�$I�$M��y��y�<�&y�$I�$I��H�$I�$I�$J   .     $O��I�<I�$I�&I�$I�$I�$I�$I�$                   ����,      (      (   �   5   �   5   |  K   9   *   8   ;   8   ;   �   J   �   8   �   I   2  D   2  J   7   ;      7      7   ����=      6      J   b   @   a   J   ����`   �  `   d   B      @   5   @   '   1   U   7   u   F   �   =    