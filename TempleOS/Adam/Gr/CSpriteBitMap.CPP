#define GEBM_EXIT		0
#define GEBM_MAIN_MENU		1
#define GEBM_COLOR		2
#define GEBM_DITHER_COLOR	3
#define GEBM_WIDTH		4
#define GEBM_SNAP		5
#define GEBM_PT			6
#define GEBM_LINE		7
#define GEBM_ARROW		8
#define GEBM_RECT		9
#define GEBM_CIRCLE		10
#define GEBM_TEXT		11
#define GEBM_TEXT_BOX		12
#define GEBM_TEXT_DIAMOND	13
#define GEBM_FLOOD_FILL		14
#define GEBM_FLOOD_FILL_NOT	15
#define GEBM_POLYLINE		16
#define GEBM_POLYPT		17
#define GEBM_COPY		18
#define GEBM_DELETE		19
#define GEBM_PASTE		20
#define GEBM_PASTE_TRANSPARENT	21
#define GEBM_FIND_AND_REPLACE	22
#define GEBM_TRIM_TO_EXTENTS	23
#define GEBM_ADD_OUTLINE	24
#define GEBM_ETCH		25
#define GEBM_UNDO		26

U0 GrInit3()
{
  DefineLstLoad("ST_SPRITE_BITMAP_MENU",
	"Exit\0Main Menu\0Color\0Dither Color\0Width\0Snap\0Point\0Line\0Arrow\0"
	"Rect\0Circle\0Text\0Text Box\0Text Diamond\0Flood Fill\0"
	"Flood Fill Not Color\0PolyLine\0PolyPoint\0Copy\0Delete\0Paste\0"
	"Paste Transparent\0Find and Replace\0Trim to Extents\0Add Outline\0"
	"Etch\0Undo\0");
}
GrInit3;

I64 PopUpSpriteBitMap(CColorROPU32 color,I64 width,I64 snap,F64 ip_scaler)
{
  I64 i;
  U8 *st1,*st2,buf[136];
  CDoc *doc=DocNew;

  Color2Text(buf,color);
  if (color&ROPF_DITHER) {
    st1="";
    st2=buf;
  } else {
    st1=buf;
    st2="";
  }

  DocPrintF(doc,"$$FG,PURPLE$$$$TX+CX,\"Sprite BitMap Menu\"$$\n"
	"$$LK+PU+CX,\"Click for Help\",\"FI:::/TempleOS/Doc/SpriteBitMap.TXT.Z\"$$\n"
	"\n$$FG,LTBLUE$$$$MU-UL,\"Color%s\",GEBM_COLOR$$\n"
	"$$MU-UL,\"Dither Color%s\",GEBM_DITHER_COLOR$$\n"
	"$$MU-UL,\"Width%d\",GEBM_WIDTH$$\n"
	"$$MU-UL,\"Snap %d %5.3f\",GEBM_SNAP$$\n"
	"$$MU-UL,\"Find & Replace Color\",GEBM_FIND_AND_REPLACE$$\n"
	"$$MU-UL,\"Trim to Extents\",GEBM_TRIM_TO_EXTENTS$$\n"
	"$$MU-UL,\"Add Outline\",GEBM_ADD_OUTLINE$$\n"
	"$$MU-UL,\"Etch\",GEBM_ETCH$$\n"
	"\n$$MU-UL,\"Point\",GEBM_PT$$\n"
	"$$MU-UL,\"Line\",GEBM_LINE$$\n"
	"$$MU-UL,\"Arrow\",GEBM_ARROW$$\n"
	"$$MU-UL,\"Rect\",GEBM_RECT$$\n"
	"$$MU-UL,\"Circle\",GEBM_CIRCLE$$\n"
	"$$MU-UL,\"Text\",GEBM_TEXT$$\n"
	"$$MU-UL,\"Text Box\",GEBM_TEXT_BOX$$\n"
	"$$MU-UL,\"Text Diamond\",GEBM_TEXT_DIAMOND$$\n"
	"$$MU-UL,\"Flood Fill\",GEBM_FLOOD_FILL$$\n"
	"$$MU-UL,\"Flood Fill Not Color\",GEBM_FLOOD_FILL_NOT$$\n"
	"$$MU-UL,\"PolyLine\",GEBM_POLYLINE$$\n"
	"$$MU-UL,\"PolyPoint\",GEBM_POLYPT$$\n"
	"\n$$MU-UL,\"Copy to Clipboard\",GEBM_COPY$$\n"
	"$$MU-UL,\"Delete to Clipboard\",GEBM_DELETE$$\n"
	"$$MU-UL,\"Paste Clipboard\",GEBM_PASTE$$\n"
	"$$MU-UL,\"Paste Transparent Clipboard\",GEBM_PASTE_TRANSPARENT$$\n"
	"\n$$MU-UL,\"Undo\",GEBM_UNDO$$\n"
	"\n$$FG,PURPLE$$$$MU-UL,\"+] Sprite Main Menu\",GEBM_MAIN_MENU$$$$FG,LTBLUE$$\n"
	"$$MU-UL,\"ExitSprite\",GEBM_EXIT$$\n"
	"$$MU-UL,\"Abort Sprite\",DOCM_CANCEL$$\n",st1,st2,width,snap,ip_scaler);
  DocPutS(doc,"\nRight-Click to get back to this menu.");
  i=PopUpMenu(doc);
  DocDel(doc);
  return i;
}

U0 GrBMEdPrepPersistentDC(CDC *dc,I64 xx1,I64 yy1,CDC *img)
{
  DCFill(dc);
  GrBlot(dc,xx1,yy1,img);
}

U0 GrBMETrimToExtents(CDC **_img,I64 *_xx1,I64 *_yy1,I64 *_xx2,I64 *_yy2,CColorROPU32 bkcolor)
{
  CDC *img=*_img;
  I64 i,c,
    x1=0,y1=0,x2=img->width-1,y2=img->height-1; //inclusive
  while (y1<y2) {
    for (i=x1;i<=x2;i++) {
      c=GrPeek(img,i,y1);
      if (c!=bkcolor&&c!=COLOR_TRANSPARENT) goto tr_y2;
    }
    y1++;
  }

tr_y2:
  while (y1<y2) {
    for (i=x1;i<=x2;i++) {
      c=GrPeek(img,i,y2);
      if (c!=bkcolor&&c!=COLOR_TRANSPARENT) goto tr_x1;
    }
    y2--;
  }

tr_x1:
  while (x1<x2) {
    for (i=y1;i<=y2;i++) {
      c=GrPeek(img,x1,i);
      if (c!=bkcolor&&c!=COLOR_TRANSPARENT) goto tr_x2;
    }
    x1++;
  }

tr_x2:
  while (x1<x2) {
    for (i=y1;i<=y2;i++) {
      c=GrPeek(img,x2,i);
      if (c!=bkcolor&&c!=COLOR_TRANSPARENT) goto tr_done;
    }
    x2--;
  }

tr_done:
  *_img=DCExtract(img,x1,y1,x2,y2);
  *_xx1+=x1;		    *_yy1+=y1;
  *_xx2+=x2-(img->width-1); *_yy2+=y2-(img->height-1); //not inclusive
  DCDel(img);
}


U0 GrBMEAddOutline(CDC *img,I64 width,CColorROPU32 color,CColorROPU32 bkcolor)
{
  I64 i,j,k,c;
  CColorROPU32 old_color;
  CDC *src;
  if (img->width && img->height) {
    old_color=img->color;
    img->color=color;
    while (width-->0) {
      src=DCExtract(img,0,0,img->width-1,img->height-1);
      for (i=0;i<img->height;i++)
	for (j=0;j<img->width;j++)
	  if (GrPeek(src,j,i)==bkcolor)
	    for (k=0;k<8;k++) {
	      c=GrPeek(src,j+gr_x_offsets[k],i+gr_y_offsets[k]);
	      if (c>=0 && c!=bkcolor) {
		GrPlot(img,j,i);
		break;
	      }
	    }
      DCDel(src);
    }
    img->color=old_color;
  }
}

U0 GrBMEEtch(CDC *img,I64 width,CColorROPU32 bkcolor)
{
  I64 i,j,k,c;
  CColorROPU32 old_color;
  CDC *src;
  if (img->width && img->height) {
    old_color=img->color;
    img->color=bkcolor;
    while (width-->0) {
      src=DCExtract(img,0,0,img->width-1,img->height-1);
      for (i=0;i<img->height;i++)
	for (j=0;j<img->width;j++)
	  if (GrPeek(src,j,i)!=bkcolor)
	    for (k=0;k<8;k++) {
	      c=GrPeek(src,j+gr_x_offsets[k],i+gr_y_offsets[k]);
	      if (c<0 || c==bkcolor) {
		GrPlot(img,j,i);
		break;
	      }
	    }
      DCDel(src);
    }
    img->color=old_color;
  }
}

I64 SpriteBitMap(CDoc *,CDocEntry *doc_e,CDC *dc,I64 *_xx1,I64 *_yy1,I64 *_xx2,I64 *_yy2,CDC **_img,CColorROPU32 bkcolor,I64 *_ip_snap,F64 *_ip_scaler)
{
  I64 i,j,mode=GEBM_LINE,color=BLACK,width=1,msg_code,p1,p2,x1,y1,x11,y11,x22,y22,ip_snap=1,
	result,xx1=*_xx1,yy1=*_yy1,xx2=*_xx2,yy2=*_yy2,
	old_e_flags1=doc_e->flags1;
  Bool down=FALSE;
  F64 ip_scaler=1.0;
  U8 *st=NULL;
  CDC *img=*_img,
	*clipboard=NULL,*undo=NULL,*dc2;
  SettingsPush; //See $LK,"SettingsPush","MN:SettingsPush"$

  if (_ip_snap) ip_snap=*_ip_snap;
  if (_ip_scaler) ip_scaler=*_ip_scaler;
  SetSnap(&Fs->snap,ip_snap,ip_scaler);

  doc_e->flags1|=DOCEF1_DONT_DRAW;
  goto bm_menu;

  while (TRUE) {
    if (sys_cur_scan_code&SCF_CTRL)//grab scroll update?
      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);

    dc->color=ROPF_DITHER+WHITE<<16+BLACK;
    dc->pen_width=1;
    GrLineRect4(dc,xx1-1,yy1-1,xx2,yy2);//This is done little bit too often.

    while (msg_code=ScanMsg(&p1,&p2,1<<MSG_IP_L_DOWN|1<<MSG_IP_L_UP|1<<MSG_IP_R_DOWN|
	  1<<MSG_IP_MOVE|1<<MSG_KEY_DOWN)) {
      switch (msg_code) {
	case MSG_KEY_DOWN:
	  if (p1==CH_SHIFT_ESC) {
	    result=GE_ABORT;
	    goto bm_key_up_done;
	  } else if (p1==CH_ESC) {
	    result=GE_CONT;
	    goto bm_key_up_done;
	  }
	  break;

	case MSG_IP_R_DOWN:
bm_menu:
	  DCFill(dc);
	  StrCpy(Fs->task_title,"Sprite BitMap Menu");
	  i=PopUpSpriteBitMap(color,width,ip_snap,ip_scaler);
	  if (i>=0)
	    StrCpy(Fs->task_title,DefineSub(i,"ST_SPRITE_BITMAP_MENU"));
	  switch (i) {
	    case DOCM_CANCEL:
	      result=GE_ABORT;
	      goto bm_done;
	    case GEBM_EXIT:
	      result=GE_EXIT;
	      goto bm_done;
	    case GEBM_MAIN_MENU:
	      result=GE_CONT;
	      goto bm_done;

	    case GEBM_COLOR:
	      i=PopUpColor(,,FALSE);
	      if (i>=0) color=i;
	      goto bm_menu;

	    case GEBM_FIND_AND_REPLACE:
	      i=PopUpColor("Find Color\n",,FALSE);
	      if (i>=0) {
		j=PopUpColor("Replace Color\n",,FALSE);
		if (j>=0) {
		  DCColorChg(img,i,j);
		  GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
		}
	      }
	      goto bm_menu;

	    case GEBM_TRIM_TO_EXTENTS:
	      GrBMETrimToExtents(&img,&xx1,&yy1,&xx2,&yy2,bkcolor);
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      goto bm_menu;

	    case GEBM_ADD_OUTLINE:
	      i=PopUpRangeI64(1,16,1,"Outline Width\n");
	      if (i>=0) {
		GrBMEAddOutline(img,i,color,bkcolor);
		GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      }
	      goto bm_menu;

	    case GEBM_ETCH:
	      i=PopUpRangeI64(1,16,1,"Etch Width\n");
	      if (i>=0) {
		GrBMEEtch(img,i,bkcolor);
		GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      }
	      goto bm_menu;

	    case GEBM_DITHER_COLOR:
	      i=PopUpColorDither;
	      if (i>=0) color=i;
	      goto bm_menu;

	    case GEBM_WIDTH:
	      i=PopUpRangeI64(1,16,1,"Pen Width\n");
	      if (i>=0) width=i;
	      goto bm_menu;

	    case GEBM_SNAP:
	      PopUpSnap(&Fs->snap);
	      ip_snap=Fs->snap.x;
	      ip_scaler=Fs->snap.dx;
	      goto bm_menu;

	    case GEBM_UNDO:
	      if (undo) {
		Beep;
		img->color=ROP_EQU;
		GrBlot(img,0,0,undo);
		DCDel(undo);
		undo=NULL;
	      }
	      goto bm_menu;

	    case GEBM_PT:
	    case GEBM_LINE:
	    case GEBM_ARROW:
	    case GEBM_RECT:
	    case GEBM_CIRCLE:
	    case GEBM_FLOOD_FILL:
	    case GEBM_FLOOD_FILL_NOT:
	    case GEBM_POLYPT:
	    case GEBM_POLYLINE:
	    case GEBM_COPY:
	    case GEBM_DELETE:
	    case GEBM_PASTE:
	    case GEBM_PASTE_TRANSPARENT:
	      mode=i;
	      break;

	    case GEBM_TEXT:
	    case GEBM_TEXT_BOX:
	    case GEBM_TEXT_DIAMOND:
	      Free(st);
	      st=PopUpMGetS("Enter text and press <ESC>.\n");
	      if (st && *st)
		mode=i;
	      else
		goto bm_menu;
	      break;

	  }
	  DCDel(undo);
	  undo=DCExtract(img,0,0,img->width-1,img->height-1);
	  undo->bkcolor=bkcolor;
	  WinMgrSync(2,TRUE);	//Let popup close
	  GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	  down=FALSE;
	  break;
	case MSG_IP_L_DOWN:
	  switch (mode) {
	    case GEBM_PT:
	      img->color=color;
	      img->pen_width=width;
	      GrPlot3(img,p1-xx1,p2-yy1,0);
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      break;
	    case GEBM_LINE:
	    case GEBM_ARROW:
	    case GEBM_RECT:
	    case GEBM_CIRCLE:
	    case GEBM_COPY:
	    case GEBM_DELETE:
	      if (down)
		GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      x1=p1; y1=p2;
	      down=TRUE;
	      dc->color=color;
	      dc->pen_width=width;
	      switch (mode) {
		case GEBM_LINE:
		  GrLine3(dc,x1,y1,0,p1,p2,0);
		  break;
		case GEBM_ARROW:
		  GrArrow3(dc,x1,y1,0,p1,p2,0);
		  break;
		case GEBM_RECT:
		  GrRect(dc,x1,y1,1,1);
		  break;
		case GEBM_CIRCLE:
		  GrCircle3(dc,x1,y1,0,1);
		  break;
		case GEBM_COPY:
		case GEBM_DELETE:
		  dc->color=ROPF_DITHER+WHITE<<16+BLACK;
		  dc->pen_width=1;
		  GrLineRect4(dc,x1,y1,x1,y1);
		  break;
	      }
	      break;
	    case GEBM_PASTE:
	    case GEBM_PASTE_TRANSPARENT:
	      if (clipboard) {
		GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
		if (mode==GEBM_PASTE) {
		  clipboard->flags|=DCF_NO_TRANSPARENTS;
		  GrBlot(dc,p1,p2,clipboard);
		  clipboard->flags&=~DCF_NO_TRANSPARENTS;
		} else {
		  dc2=DCCopy(clipboard);
		  DCColorChg(dc2,bkcolor);
		  GrBlot(dc,p1,p2,dc2);
		  DCDel(dc2);
		}
	      }
	      break;
	    case GEBM_TEXT:
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      dc->color=color;
	      GrPutS(dc,p1,p2,st);
	      break;
	    case GEBM_TEXT_BOX:
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      dc->color=color;
	      GrTextBox3(dc,p1,p2,0,st);
	      break;
	    case GEBM_TEXT_DIAMOND:
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      dc->color=color;
	      GrTextDiamond3(dc,p1,p2,0,st);
	      break;
	    case GEBM_FLOOD_FILL:
	      img->color=color;
	      GrFloodFill(img,p1-xx1,p2-yy1);
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      break;
	    case GEBM_FLOOD_FILL_NOT:
	      img->color=color;
	      GrFloodFill(img,p1-xx1,p2-yy1,TRUE);
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      break;
	    case GEBM_POLYLINE:
	      if (!down) {
		x1=p1; y1=p2;
		down=TRUE;
		dc->color=color;
		dc->pen_width=width;
		GrLine3(dc,x1,y1,0,p1,p2,0);
	      }
	      break;
	    case GEBM_POLYPT:
	      x1=p1; y1=p2;
	      down=TRUE;
	      img->color=color;
	      img->pen_width=width;
	      GrLine3(img,x1-xx1,y1-yy1,0,p1-xx1,p2-yy1,0);
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      break;
	  }
	  break;
	case MSG_IP_MOVE:
	  switch (mode) {
	    case GEBM_LINE:
	    case GEBM_ARROW:
	    case GEBM_POLYLINE:
	      if (down) {
		GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
		dc->color=color;
		dc->pen_width=width;
		if (mode==GEBM_ARROW)
		  GrArrow3(dc,x1,y1,0,p1,p2,0);
		else
		  GrLine3(dc,x1,y1,0,p1,p2,0);
	      }
	      break;
	    case GEBM_RECT:
	      if (down) {
		GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
		if (x1<p1) {
		  x11=x1;
		  x22=p1;
		} else {
		  x11=p1;
		  x22=x1;
		}
		if (y1<p2) {
		  y11=y1;
		  y22=p2;
		} else {
		  y11=p2;
		  y22=y1;
		}
		dc->color=color;
		GrRect(dc,x11,y11,x22-x11+1,y22-y11+1);
	      }
	      break;
	    case GEBM_COPY:
	    case GEBM_DELETE:
	      if (down) {
		GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
		if (x1<p1) {
		  x11=x1;
		  x22=p1;
		} else {
		  x11=p1;
		  x22=x1;
		}
		if (y1<p2) {
		  y11=y1;
		  y22=p2;
		} else {
		  y11=p2;
		  y22=y1;
		}
		dc->color=ROPF_DITHER+WHITE<<16+BLACK;
		dc->pen_width=1;
		GrLineRect4(dc,x11,y11,x22,y22);
	      }
	      break;
	    case GEBM_CIRCLE:
	      if (down) {
		GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
		dc->color=color;
		dc->pen_width=width;
		GrCircle3(dc,x1,y1,0,Sqrt(SqrI64(p1-x1)+SqrI64(p2-y1)));
	      }
	      break;
	    case GEBM_PASTE:
	    case GEBM_PASTE_TRANSPARENT:
	      if (clipboard) {
		GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
		if (mode==GEBM_PASTE) {
		  clipboard->flags|=DCF_NO_TRANSPARENTS;
		  GrBlot(dc,p1,p2,clipboard);
		  clipboard->flags&=~DCF_NO_TRANSPARENTS;
		} else {
		  dc2=DCCopy(clipboard);
		  DCColorChg(dc2,bkcolor);
		  GrBlot(dc,p1,p2,dc2);
		  DCDel(dc2);
		}
	      }
	      break;
	    case GEBM_TEXT:
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      dc->color=color;
	      GrPutS(dc,p1,p2,st);
	      break;
	    case GEBM_TEXT_BOX:
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      dc->color=color;
	      GrTextBox3(dc,p1,p2,0,st);
	      break;
	    case GEBM_TEXT_DIAMOND:
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      dc->color=color;
	      GrTextDiamond3(dc,p1,p2,0,st);
	      break;
	    case GEBM_POLYPT:
	      if (down) {
		img->color=color;
		img->pen_width=width;
		GrLine3(img,x1-xx1,y1-yy1,0,p1-xx1,p2-yy1,0);
		GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
		x1=p1; y1=p2;
	      }
	      break;
	  }
	  break;
	case MSG_IP_L_UP:
	  switch (mode) {
	    case GEBM_LINE:
	    case GEBM_ARROW:
	    case GEBM_POLYPT:
	    case GEBM_POLYLINE:
	      img->color=color;
	      img->pen_width=width;
	      if (mode==GEBM_ARROW)
		GrArrow3(img,x1-xx1,y1-yy1,0,p1-xx1,p2-yy1,0);
	      else
		GrLine3(img,x1-xx1,y1-yy1,0,p1-xx1,p2-yy1,0);
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      if (mode==GEBM_POLYLINE) {
		x1=p1; y1=p2;
	      } else
		down=FALSE;
	      break;
	    case GEBM_RECT:
	      img->color=color;
	      if (x1<p1) {
		x11=x1;
		x22=p1;
	      } else {
		x11=p1;
		x22=x1;
	      }
	      if (y1<p2) {
		y11=y1;
		y22=p2;
	      } else {
		y11=p2;
		y22=y1;
	      }
	      GrRect(img,x11-xx1,y11-yy1,x22-x11+1,y22-y11+1);
	      down=FALSE;
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      break;
	    case GEBM_COPY:
	    case GEBM_DELETE:
	      if (x1<p1) {
		x11=x1;
		x22=p1;
	      } else {
		x11=p1;
		x22=x1;
	      }
	      if (y1<p2) {
		y11=y1;
		y22=p2;
	      } else {
		y11=p2;
		y22=y1;
	      }
	      DCDel(clipboard);
	      clipboard=DCExtract(img,x11-xx1,y11-yy1,x22-xx1,y22-yy1);
	      clipboard->bkcolor=bkcolor;
	      if (mode==GEBM_DELETE) {
		img->color=bkcolor;
		GrRect(img,x11-xx1,y11-yy1,x22-x11+1,y22-y11+1);
	      }
	      goto bm_menu;
	    case GEBM_CIRCLE:
	      img->color=color;
	      img->pen_width=width;
	      GrCircle3(img,x1-xx1,y1-yy1,0,Sqrt(SqrI64(p1-x1)+SqrI64(p2-y1)));
	      down=FALSE;
	      GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      break;
	    case GEBM_PASTE:
	    case GEBM_PASTE_TRANSPARENT:
	      if (clipboard) {
		GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
		if (mode==GEBM_PASTE) {
		  clipboard->flags|=DCF_NO_TRANSPARENTS;
		  GrBlot(img,p1-xx1,p2-yy1,clipboard);
		  clipboard->flags&=~DCF_NO_TRANSPARENTS;
		} else {
		  dc2=DCCopy(clipboard);
		  DCColorChg(dc2,bkcolor);
		  GrBlot(img,p1-xx1,p2-yy1,dc2);
		  DCDel(dc2);
		}
		GrBMEdPrepPersistentDC(dc,xx1,yy1,img);
	      }
	      break;
	    case GEBM_TEXT:
	      img->color=color;
	      GrPutS(img,p1-xx1,p2-yy1,st);
	      goto bm_menu;
	    case GEBM_TEXT_BOX:
	      img->color=color;
	      GrTextBox3(img,p1-xx1,p2-yy1,0,st);
	      goto bm_menu;
	    case GEBM_TEXT_DIAMOND:
	      img->color=color;
	      GrTextDiamond3(img,p1-xx1,p2-yy1,0,st);
	      goto bm_menu;
	  }
	  break;
      }
    }
    WinMgrSync;
  }
bm_key_up_done:
  GetMsg(,,1<<MSG_KEY_UP);
bm_done:
  DCDel(clipboard);
  DCDel(undo);
  Free(st);
  DCFill(dc);
  SettingsPop;
  doc_e->flags1=old_e_flags1;
  if (_ip_snap)   *_ip_snap=ip_snap;
  if (_ip_scaler) *_ip_scaler=ip_scaler;
  *_img=img;
  *_xx1=xx1,*_yy1=yy1,*_xx2=xx2,*_yy2=yy2;
  return result;
}
