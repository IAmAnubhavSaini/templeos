



$SP,"<1>",1$







$SP,"<2>",2$




U0 DrawGodTalking(CTask *,CDC *dc)
{
  I64 i;
  F64 m,a,x1,y1,x2,y2;
  SpritePlot3(dc,0,70,0,$IB,"<1>",1$);
  SpritePlot3(dc,200,70,0,$IB,"<2>",2$);
  dc->color=win_updates&15;
  for (i=0;i<256;i++) {
    m=Sqr(Rand); a=2*�*Rand;
    P2R(&x1,&y1,m,a);
    m=Sqr(Rand); a=2*�*Rand;
    P2R(&x2,&y2,m,a);
    GrLine(dc,235+30*x1,40+30*y1,235+30*x2,40+30*y2);
  }
}

U0 RandWord()
{// $WW,1$$LK+PU,"Holy Spirit Instructions","FI:::/TempleOS/Apps/HolySpirit/HSNotes.TXT"$
  I64 i;
  if (ws_num_words) {
    i=TimerPickedU32; //$LK,"HolySpirit.CPP.Z","FI:::/TempleOS/Apps/HolySpirit/HolySpirit.CPP.Z"$
    "%s ",ws_sorted_words[i%ws_num_words]->str;
  }
}

U0 RandQuote()
{ // $WW,1$$LK+PU,"Holy Spirit Instructions","FI:::/TempleOS/Apps/HolySpirit/HSNotes.TXT"$
  I64 i,size,l=300;
  U8 *bible_filename=Define("ST_BIBLE_FILE");
  U8 *buf=FileRead(bible_filename,&size);
  if (buf) {
    i=TimerPickedU32%(size-l); //$LK,"HolySpirit.CPP.Z","FI:::/TempleOS/Apps/HolySpirit/HolySpirit.CPP.Z"$
    buf[i+l]=0;
    "" buf+i;
    Free(buf);
  }
}

U0 UpTheMountain()
{
  I64 i;
  Mountain;
  Horeb;

  SettingsPush(Fs,TSF_SAME_SONG); //See $LK,"SettingsPush","MN:SettingsPush"$
  Fs->text_attr=YELLOW<<4+BLUE;
  Fs->draw_it=&DrawGodTalking;
  "$$BG,YELLOW$$$$WW,1$$$$CM,0,30$$God Says...$$FG,RED$$\n";
  if (RandU32&1)
    for (i=0;i<16;i++)
      RandWord;
  else
    RandQuote;
  "$$FG$$\n\nPress $FG,2$<SHIFT-ESC>$FG$.\n";
  View;
  DocClear;
  SettingsPop(Fs,TSF_SAME_SONG);
}
           �                  ����   ����.      .      P   ����P   ����   ����O   ����r   	   N   ����t      Q   ����o   ����p   ����x      x      �   ����q   ����z   ����z   ����}   ����}   ����o   ����o   ����k   ����v   ����v   ����                          -   ����$   ����<   ����&   ����$   ����   ����   ����   ����0   ����#   ����,   ����   ����   ���� 