//8 has words
U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  try {
  task=PopUpViewStr(
    "$LK-A,"BF:Matthew,19:16"$\n"
    );
  TaskWait(task);
  SetWinHorz(Fs->win_left,Fs->win_right,task);
  SetWinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
  "$$WW+H,1$$";
  while (!ScanKey) {
	Play("3eGAetCCCFFDqAeAAFAAFqG",
		"I\0\0have\0\0\0kept\0\0\0\0\0\0the\0\0\0com\0mands.\n\0");
	Play("eGAetCCCFFDqAeAAFAAFqG",
		"I\0\0have\0\0\0kept\0\0\0\0\0\0the\0\0\0com\0mands.\n\0");
	Play("EeEGqEeEEqDBetCCGqC",
		"What\0\0\0else\0\0\0must\0I\0do?\n\0\0\0\0");
	Play("EeEGqEeEEqDBetCCGqC",
		"What\0\0\0else\0\0\0must\0I\0do?\n\0\0\0\0");
	"$$FG,GREEN$$(Christ said sell all and follow me.)\n$$FG$$";
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
