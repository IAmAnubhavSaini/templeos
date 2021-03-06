//See $LK,"ExtScreenCodes","FA:::/TempleOS/Adam/Gr/GrScreen.CPP,ExtScreenCodes"$ for the format
//of the U32 you pass to PlotWinU8()

//See $LK-A,"::/TempleOS/Demo/ExtendedChars.CPP"$
//and $LK-A,"::/TempleOS/Demo/Graphics/FontEd.CPP"$.
//Hold ALT and press a 3 digit decimal number
//or press <CTRL-ALT-A>.

U0 DrawIt(CTask *task,CDC *)
{
  I64 i;
  for (i=0;i<256;i++)
    PlotWinU8(task,i+BLACK<<12+WHITE<<8,(i&15)*2,(i/16)*2);
}

U0 ScreenCodes()
{
  SettingsPush; //See $LK,"SettingsPush","MN:SettingsPush"$
  DocClear;
  Fs->draw_it=&DrawIt;
  GetChar(,FALSE);
  SettingsPop;
}

ScreenCodes;
