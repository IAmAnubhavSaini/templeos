//8 has graphics
$SP,"<1>",1$











U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  music_tempo=2.5;
  try {
  task=PopUpViewStr(
    "$LK,"Philippians,4:8","BF:Philippians,4:8"$\n"
    );
  TaskWait(task);
  SetWinHorz(Fs->win_left,Fs->win_right,task);
  SetWinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
  "$$WW+H,1$$";
  while (!ScanKey) {
	Play("3eEGqACetCFGeEFqGetGFEqA",
		"Look\0at\0that!\0\0\0\0Dew\0\0\0on\0flow\0\0\0ers.\n\0");
	Play("eEGqACetCFGeEFqGetGFEqA",
		"Look\0at\0that!\0\0\0\0Dew\0\0\0on\0flow\0\0\0ers.\n\0");
	PutSprite($IB,"<1>",1$);
	"$$CM,0,7$$";
	Play("eDEqEAEFCCetCAD",
		"Pret\0ty\0when\0\0they\0catch\0the\0light.\n\0\0\0\0");
	Play("eDEqEAEFCCetCAD",
		"Pret\0ty\0when\0\0they\0catch\0the\0light.\n\0\0\0\0");
	"$$FG,GREEN$$(It's easy to be dark and profound all the time in arrogance."
	"Sometimes, I search for self-evident goodness.)\n$$FG$$";
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
           �     $       "   L�0J�$��$a		   (   $   I�0a�(J�(J�    $      I�ͶM�   %      ��$�
   %      ���$�[�   #      I %   $   #   IEI���FQ��I   #      ���	   ,      $�m
   ,      $�m3   .      %f���   2      ���a�/      1   ����/      2   ����   /      &   .      I$   1      $�e�   3      ��(\=�˒0$   5      &�$v�	   (      [�$   (      $�$��   )      Y�,   '      ��-K     !   3��<I�$I�$I�$I���mۺ�۶m޶m�6 2l۶�$I�$I��f�$IR��IH�$I�ܺy6I�$#J��m��5I�)*۶�ۼ�ݶm۶m۶m�mۆmۖ%I�m�ܒ?ϓ�I�$I�$I�$i��I�$I�$E�$�H��$۶mݶ۶y&y�$I�"ù�k�޶m۶m۶m۶m۶m۶m۶m۶a l	�C���m�f�6I��iI   
      �(Q��$ɟ�����nI��[;Ϯm�"C�w   .      ���$I��(��D�^ݒ"E��(�m�v�$ɶm۶m۶m۶m�m۞=         I%   E   &   $I�<)F�&I�m�6
   1      d���   $      L��	   '      d��   (      &�]{N�(   5      "%Fںu
   9       $I�$         n�m[ 	         LBE   =   %   $I 