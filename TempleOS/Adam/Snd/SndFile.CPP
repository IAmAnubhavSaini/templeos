#help_index "Snd/Snd Files"
#define SNDFILE_SAMPLE_RATE	8000
//Header for a ".SND" file
class CFileSND
{ //big endian
  U32 signature;    //0x646e732e
  U32 offset;	   //24
  U32 data_size;
  U32 coding;	   //3=16bit uncompressed
  U32 sample_rate;  //Hz
  U32 channels;	   //1=mono
  I16 body[1];
};

//This is a windows media constraint.
#define MAX_SND_FILE_DATA	0x7FF00


public I64 SndFileCreate(U8 *base_filename,F64 normalized_vol=1.0,F64 averaging=0.0,I64 waveform=WF_SQUARE,
  F64 reverb_delay=0,F64 reverb_intensity=0)
//Use "snd_record" flag to start or stop
//recording, then call this routine.
//Averaging should be a num from 0.0 to 0.999.
//Vol should be from 0.0 to 1.0.
//Set reverb_delay to like 0.3 sec and reverb_intensity to like 0.4.
{
  I64 i,k,cnt,cnt2,level,file_num;
  F64 avg,dt;
  CFileSND *s;
  CSndWaveCtrl *swc=SndWaveCtrlNew(SNDFILE_SAMPLE_RATE,16,1);
  CSndData *d,*d1;
  U8 *name,*name2;

  snd_record=FALSE;

  dt=snd_record_root.last->time_stamp-snd_record_root.next->time_stamp;
  if (!dt) return 0;
  cnt=dt*SNDFILE_SAMPLE_RATE;
  cnt++; //terminator

  name=StrNew(base_filename);
  FileExtRem(name);

  s=CAlloc(offset(CFileSND.body)+cnt*sizeof(I16));
  s->signature=0x646e732e;
  s->offset=EndianU32(offset(CFileSND.body));
  s->coding=EndianU32(3);
  s->sample_rate=EndianU32(SNDFILE_SAMPLE_RATE);
  s->channels=EndianU32(1);

  d=snd_record_root.next;
  k=0;
  while (d->next!=&snd_record_root) {
    d1=d->next;
    dt=d1->time_stamp-d->time_stamp;
    cnt2=dt*SNDFILE_SAMPLE_RATE;
    SndWaveAddBuf(swc,&s->body[k],cnt2,d->freq,waveform,normalized_vol);
    k+=cnt2;
    QueRem(d);
    Free(d);
    d=d1;
  }

  //Average
  if (averaging) {
    avg=0;
    for (i=0;i<cnt-1;i++)
      s->body[i]=avg=LowPass1(averaging,avg,s->body[i],1.0);
    }

    //reverb
  if (reverb_intensity) {
    if (dt=reverb_delay*SNDFILE_SAMPLE_RATE)
      for (i=dt;i<cnt;i++)
	s->body[i]+=reverb_intensity*s->body[i-dt];
      }

      //Get rid of D.C. component
  for (k=0;k<3;k++) {
    level=0;
    for (i=0;i<cnt-1;i++)
      level+=s->body[i];
    level/=cnt-1;
    for (i=0;i<cnt-1;i++)
      s->body[i]=LimitI64(s->body[i]-level,MIN_I16,MAX_I16);
    }

  for (i=0;i<cnt-1;i++)
    s->body[i]=EndianU16(s->body[i]);

  s->body[cnt-1]=0;

  d=snd_record_root.next;
  while (d!=&snd_record_root) {
    d1=d->next;
    QueRem(d);
    Free(d);
    d=d1;
  }

  name2=MAlloc(StrLen(name)+3+1+3+1);
  cnt2=cnt;
  file_num=0;
  while (cnt2>0) {
    i=cnt2;
    if (i>MAX_SND_FILE_DATA)
      i=MAX_SND_FILE_DATA;
    s->data_size=EndianU32(i*sizeof(I16));
    MemCpy(s->body,&s->body[file_num*MAX_SND_FILE_DATA],i*sizeof(I16));
    SPrintF(name2,"%s%03d.SND",name,file_num++);
    FileWrite(name2,s,offset(CFileSND.body)+i*sizeof(I16));
    cnt2-=i;
  }
  Free(s);
  Free(name);
  Free(name2);

  SndWaveCtrlDel(swc);
  return cnt;
}

#help_index ""
