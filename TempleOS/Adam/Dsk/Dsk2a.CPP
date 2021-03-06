#help_index "File/Cmd Line (Typically);Cmd Line (Typically)"

public U8 *DBlk(I64 blk,Bool write=FALSE)
{//Dump Disk Blk.
//If you set write to TRUE, it will be written when you press <ESC>.
//See $LK-A,"::/TempleOS/Demo/Dsk/Raw.CPP"$.
  U8 *buf=MAlloc(BLK_SIZE);

  RBlks(Fs->cur_prt,buf,blk,1);
  DocD(buf,BLK_SIZE);
  if (write) {
    "Edit and press <ESC> to write or <SHIFT-ESC>\n";
    if (View) {
      "Write\n";
      WBlks(Fs->cur_prt,buf,blk,1);
    }
  }
  return buf;
}

public U8 *DCluster(I64 c,Bool write=FALSE,I64 num=0)
{//Dump Disk Cluster.
//If you set write to TRUE, it will be written when you press <ESC>.
//See $LK-A,"::/TempleOS/Demo/Dsk/Raw.CPP"$.
//Do $LK,"Dir","MN:Dir"$("*",TRUE); to get cluster numbers of files.
  U8 *buf=MAlloc(Fs->cur_prt->spc<<BLK_SIZE_BITS);
  c=ClusterNumNext(Fs->cur_prt,c,num);
  RClusters(Fs->cur_prt,buf,c,1);
  "Cluster:%X\n",c;
  DocD(buf,Fs->cur_prt->spc<<BLK_SIZE_BITS);
  if (write) {
    "Edit and press <ESC> to write or <SHIFT-ESC>\n";
    if (View) {
      "Write\n";
      WClusters(Fs->cur_prt,buf,c,1);
    }
  }
  return buf;
}

#help_index "File/Program Routines"

U0 OFFlatten(CDirEntry *tempm,CDirEntry **a,I64 *i)
{
  CDirEntry *tempm1;
  while (tempm) {
    tempm1=tempm->next;
    if (tempm->attr&_ATTR_DIR) {
      OFFlatten(tempm->sub,a,i);
      CDirEntryDel(tempm);
    } else {
      a[*i]=tempm;
      *i=*i+1;
    }
    tempm=tempm1;
  }
}


public I64 CFileCnt(CDirEntry *tempm)
{
  I64 cnt=0;
  while (tempm) {
    if (tempm->attr&_ATTR_DIR)
      cnt+=CFileCnt(tempm->sub);
    else
      cnt++;
    tempm=tempm->next;
  }
  return cnt;
}

I64 OFCompare(CDirEntry *e1,CDirEntry *e2)
{
  return e1->cluster-e2->cluster;
}

public CDirEntry **FilesOrder(CDirEntry *tempm,I64 *_cnt=NULL)
{ //See $LK-A,"::/TempleOS/Demo/Dsk/OrderedIO.CPP"$.
  I64 cnt=CFileCnt(tempm),i=0;
  CDirEntry **a=MAlloc(sizeof(CDirEntry *)*cnt);
  OFFlatten(tempm,a,&i);
  QSortI64(a,cnt,&OFCompare);
  if (_cnt) *_cnt=cnt;
  return a;
}


#help_index "File/Cmd Line (Typically);Cmd Line (Typically)"

public Bool Copy(U8 *src_files_find_mask,U8 *dst_files_find_mask=".")
{
  Bool result=TRUE;
  CFileAccess *fa;
  CDirEntry *tempm,*tempm1;
  U8 *st;
  if (!(tempm=FilesFind(src_files_find_mask)))
    return FALSE;
  if (IsDir(dst_files_find_mask)) {
    if (fa=FileAccessNew(dst_files_find_mask,TRUE)) {
      while (tempm) {
	tempm1=tempm->next;
	if (!(tempm->attr & _ATTR_DIR)) {
	  st=FileNameAbs(tempm->name);
	  if (!CopySingle(tempm->full_name,st))
	    result=FALSE;
	  Free(st);
	}
	CDirEntryDel(tempm);
	tempm=tempm1;
      }
      FileAccessDel(fa);
    }
    return result;
  } else
    return CopySingle(src_files_find_mask,dst_files_find_mask);
}

public Bool Move(U8 *f1,U8 *f2)
{
  if (Copy(f1,f2)) {
    Del(f1);
    return TRUE;
  }
  return FALSE;
}

I64 PutCopyTree(CDirEntry *tempm,I64 src_dir_len,I64 dst_dir_len,U8 *dst_dir)
{
  U8 *st;
  I64 result=1;
  while (tempm) {
    st=MAlloc(StrLen(tempm->full_name)+dst_dir_len+2);
    MemCpy(st,dst_dir,dst_dir_len);
    StrCpy(st+dst_dir_len,tempm->full_name+src_dir_len);
    if (tempm->attr & _ATTR_DIR) {
      MkDir(st);
      result+=PutCopyTree(tempm->sub,src_dir_len,dst_dir_len,dst_dir);
    } else
      if (CopySingle(tempm->full_name,st))
	result++;
    Free(st);
    tempm=tempm->next;
  }
  return result;
}

public I64 CopyTree(U8 *src_files_find_mask,U8 *dst_files_find_mask)
{  //returns count of copied files (not dirs)
  CFileAccess *fa;
  CDirEntry *tempm=NULL;
  I64 result=0,i1,i2;
  U8 *st1,*st2;

  st1=DirNameAbs(src_files_find_mask);
  st2=DirNameAbs(dst_files_find_mask);
  i1=StrLen(st1);
  if (!StrNCmp(st1,st2,i1) && (st2[i1]=='/' || !st2[i1]) ) {
    Free(st1);
    Free(st2);
    return 0;
  }
  Free(st1);
  Free(st2);
  if (fa=FileAccessNew(src_files_find_mask,TRUE)) {
    tempm=SysFilesFind(fa->mask,FUF_RECURSE);
    st1=CurDir;
    FileAccessDel(fa);
    i1=StrLen(st1);
    if (i1==3) i1--;
    if (fa=FileAccessNew(dst_files_find_mask,TRUE,TRUE)) {
      st2=CurDir;
      i2=StrLen(st2);
      if (i2==3) i2--;
      result=PutCopyTree(tempm,i1,i2,st2);
      FileAccessDel(fa);
      Free(st2);
    }
    CDirLstDel(tempm);
    Free(st1);
  }
  return result;
}

I64 DelTreeDirs(CDirEntry *tempm1)
{
  I64 result=0;
  CDirEntry *tempm2;
  while (tempm1) {
    tempm2=tempm1->next;
    if (tempm1->attr & _ATTR_DIR) {
      if (tempm1->sub)
	result+=DelTreeDirs(tempm1->sub);
      result+=SysDel(tempm1->full_name,TRUE);
    }
    CDirEntryDel(tempm1);
    tempm1=tempm2;
  }
  return result;
}

I64 DelTreeFiles(CDirEntry *tempm1)
{
  I64 result=0;
  CDirEntry *tempm2;
  while (tempm1) {
    tempm2=tempm1->next;
    if (tempm1->attr & _ATTR_DIR) {
      if (tempm1->sub)
	result+=DelTreeFiles(tempm1->sub);
    } else
      result+=SysDel(tempm1->full_name,FALSE);
    CDirEntryDel(tempm1);
    tempm1=tempm2;
  }
  return result;
}

public I64 DelTree(U8 *files_find_mask,U8 *fu_flags=NULL)
{
  I64 result=0,fuf_flags=0;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r");
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  if (IsDir(files_find_mask)) {
    result=DelTreeDirs(FilesFind(files_find_mask,fuf_flags));
    result+=SysDel(files_find_mask,TRUE);
    result+=SysDel(files_find_mask,FALSE);
  } else
    result=DelTreeFiles(FilesFind(files_find_mask,fuf_flags));
  return result;
}

U0 TouchFile(U8 *filename,U8 *attr,CDate cdt=0) //Just one file
{
  CPrt *p=Drv2Prt(*filename);
  CDirEntry de;
  U8 *cur_dir=StrNew(filename),
     buf[136];
  if (FileFind(filename,&de,FUF_JUST_FILES)) {
    Free(de.full_name);
    if (!StrCmp(attr,"+?"))
      "%-48ts%s\n",filename,SPrintFFlags(buf,Define("ST_FILE_ATTRS"),de.attr);
    else {
      StrFirstRem(cur_dir,":");
      StrLastRem(cur_dir,"/");
      if (!*cur_dir)
	StrCpy(cur_dir,"/");
      GetFFlags(&de.attr,Define("ST_FILE_ATTRS"),attr);
      if (cdt)
	de.datetime=cdt;
      else
	de.datetime=GetCurTimeDate;
      CDirNew(p,cur_dir,&de,FALSE);
    }
  } else PutDefineErr("ST_FILE_NOT_FOUND");
}

U0 TouchLst(CDirEntry *tempm1,U8 *attr,CDate cdt=0)
{
  CDirEntry *tempm2;
  while (tempm1) {
    tempm2=tempm1->next;
    if (tempm1->attr & _ATTR_DIR) {
      if (tempm1->sub) {
	"%S%s\n","ST_SCANNING_DIR",tempm1->full_name;
	TouchLst(tempm1->sub,attr,cdt);
      }
    } else
      TouchFile(tempm1->full_name,attr,cdt);
    CDirEntryDel(tempm1);
    tempm1=tempm2;
  }
}

public U0 Touch(U8 *files_find_mask,U8 *attr="+?",U8 *fu_flags=NULL,CDate cdt=0)
{
//Attr "+?" =show
//     "+T" =resident
//To Set Date
//  Touch(filename,"",,datetime);
  I64 fuf_flags=0;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  TouchLst(FilesFind(files_find_mask,fuf_flags),attr,cdt);
}

I64 FFLst(CDirEntry *tempm1)
{
  I64 cnt=0;
  CDirEntry *tempm2;

  while (tempm1) {
    tempm2=tempm1->next;
    if (tempm1->attr & _ATTR_DIR) {
      if (tempm1->sub)
	cnt+=FFLst(tempm1->sub);
    } else {
      PutFileLink(tempm1->full_name);
      NewLine;
      cnt++;
    }
    CDirEntryDel(tempm1);
    tempm1=tempm2;
  }
  return cnt;
}

public I64 FF(U8 *files_find_mask,U8 *fu_flags=NULL)
//Files Find
{
  I64 cnt=0,fuf_flags=0;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r");
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  cnt+=FFLst(FilesFind(files_find_mask,fuf_flags));
  return cnt;
}

public U0 SetDrvTextAttr(U8 drv_let,U8 attr)
{ //This is to color the border text according to the current drive
  CPrt *p;
  try {
    p=Drv2Prt(drv_let);
    p->text_attr=attr;
  } catch
    Fs->catch_except=TRUE;
}

I64 RedSeaFSUnusedDrvSpace(U8 drv_let=0)
{
  CPrt *p=Drv2Prt(drv_let);
  I64 result=0,i,l;
  U8 *bitmap;
  try {
    l=(p->size+p->spc-1)/p->spc;
    i=((l+7)>>3+BLK_SIZE-1)>>BLK_SIZE_BITS;
    bitmap=MAlloc(i<<BLK_SIZE_BITS);
    RBlks(p,bitmap,p->map1,i);
    for (i=0;i<l;i++)
      if (!Bt(bitmap,i))
	result++;
    Free(bitmap);
  } catch
    PrtUnlock(p);
  return result*BLK_SIZE*p->spc;
}

I64 FAT32UnusedDrvSpace(U8 drv_let=0)
{
  CPrt *p=Drv2Prt(drv_let);
  I64 result=0,i,l;
  U32 *bitmap;
  try {
    l=(p->size+p->spc-1)/p->spc;
    i=(l*4+BLK_SIZE-1)>>BLK_SIZE_BITS;
    bitmap=MAlloc(i<<BLK_SIZE_BITS);
    RBlks(p,bitmap,p->map1,i);
    for (i=0;i<l;i++)
      if (!bitmap[i])
	result++;
    Free(bitmap);
  } catch
    PrtUnlock(p);
  return result*BLK_SIZE*p->spc;
}

public I64 UnusedDrvSpace(U8 drv_let=0)
{ //returns size in bytes
  CPrt *p=Drv2Prt(drv_let),*p_old=Fs->cur_prt;
  U8 *old_dir=StrNew(Fs->cur_dir);
  I64 result=0;
  Drv(drv_let);
  switch (p->type) {
    case PT_REDSEA:
      result=RedSeaFSUnusedDrvSpace(drv_let);
      break;
    case PT_FAT32:
      result=FAT32UnusedDrvSpace(drv_let);
      break;
    default:
      PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
  }
  Drv(Prt2Drv(p_old));
  Cd(old_dir);
  Free(old_dir);
  return result;
}

#help_index "File/Program Routines"

I64 TreeSizeFiles(CDirEntry *tempm,I64 round_to)
{
  I64 result=0,i;
  while (tempm) {
    if (round_to)
      i=CeilU64(tempm->size,round_to);
    else
      i=tempm->size;
    if (tempm->attr&_ATTR_DIR)
      i+=TreeSizeFiles(tempm->sub,round_to);
    tempm->user_data=i;
    result+=i;
    tempm=tempm->next;
  }
  return result;
}

public I64 TreeSize(U8 *files_find_mask="/TempleOS/*",U8 *fu_flags=NULL,I64 round_to=0)
{
//Does not include directory size of base directory, but
//does include size of sub directories.
  I64 fuf_flags=0,result=0;
  CDirEntry *tempm1=NULL;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r");
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  if (tempm1=FilesFind(files_find_mask,fuf_flags)) {
    result=TreeSizeFiles(tempm1,round_to);
    CDirLstDel(tempm1);
  }
  return result;
}

#help_index ""
