//7 has graphics



$SP,"<1>",1$








U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	"$$FG,GREEN$$Jesus talking...$$FG$$\n";
	Play("2etG3BCqGeDBqC2G3sGAGAeEFsGGGG",
		"It\0is\0\0finished.\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
	NewLine(1);
	PutSprite($IB,"<1>",1$);
	NewLine(7);
	Play("2etG3BCqGeDBqC2G3sGAGAeEFsGGGG",
		"It\0is\0\0finished.\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
	Play("etGCFqDetBEB2G3DBeEAsEGEG2qGG",
		"That\0\0was\0rough!\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
	Play("3etGCFqDetBEB2G3DBeEAsEGEG2qGG",
		"But\0\0I'm\0tough.\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
           R              $   4         0      	��������       $   4         0       