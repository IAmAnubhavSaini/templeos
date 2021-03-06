//See $LK-A,"::/TempleOS/Demo/Lectures/GraphicsCPULoad.CPP"$
U8 reverse[256],	//The VGA bits are backward

   image[640*480/8];	//We need read-modify write.
			//0xA0000 memory can't be read.
 
U0 MGInit()
{
  I64 i,j;
  MemSet(image,0,sizeof(image));
  MemSet(reverse,0,sizeof(reverse));
  for (i=0;i<256;i++)
    for (j=0;j<8;j++)
      if (Bt(&i,j))
	Bts(&reverse[i],7-j);
}
 
U0 MGUpdate()
{ //Copy image to VGA memory
  //For better performance we could only write what's changed.
  //0xA0000 is slower than normal RAM.
  OutU8(VGA_SC_INDEX,VGA_MAP_MASK);
  OutU8(VGA_SC_DATA,0xF);//All color planes at once -- Black and White
  MemCpy(0xA0000,image,sizeof(image));
}
 
U0 MGPlot(I64 x,I64 y)
{
  if (0<=x<640 && 0<=y<480)
    Bts(image,y*640+x^7);
}
 
U0 MGHLine(I64 x1,I64 x2,I64 y)
{ //Warning!  No clipping
  //For performance, we do as many whole-bytes as possible.
  U8 *ptr;
  I64 i,w,leading,trailing,whole_bytes;
  if (x2<x1) SwapU64(&x1,&x2);
  ptr=image+y*640/8+x1>>3;
  w=x2-x1+1;
  leading =8-x1&7;
  trailing=(x2+1)&7;
  if (leading+trailing>w)
    *ptr|=reverse[(0xFF00>>leading&(0x00FF<<trailing)>>8)];
  else {
    whole_bytes=(w-leading-trailing)>>3;
    if (leading)
      *ptr++|=reverse[(0xFF00>>leading)&0xFF];
    for (i=0;i<whole_bytes;i++)
      *ptr++=0xFF;
    if (trailing)
      *ptr++|=reverse[(0x00FF<<trailing)>>8];
  }
}
 
U0 MGLine(I64 x1,I64 y1,I64 x2,I64 y2)
{ //Warning!  No clipping
  I64 dx=x2-x1,dy=y2-y1;
  x1<<=32; x2<<=32;
  y1<<=32; y2<<=32;
  if (AbsI64(dx)>AbsI64(dy)) {
    dy=dy<<32/AbsI64(dx);
    dx=SignI64(dx)<<32;
    while (x1!=x2) {
      MGPlot(x1.i32[1],y1.i32[1]);
      x1+=dx; y1+=dy;
    }
  } else {
    dx=dx<<32/AbsI64(dy);
    dy=SignI64(dy)<<32;
    while (y1!=y2) {
      MGPlot(x1.i32[1],y1.i32[1]);
      x1+=dx; y1+=dy;
    }
  }
  MGPlot(x1.i32[1],y1.i32[1]);
}
 
U0 MGCircle(I64 x,I64 y,F64 r)
{
  F64 s,c,x1,y1,x2,y2;
  I64 len;
  if (r<0) return;
  x1=r; y1=0;
  c=Cos(1/r);
  s=Sin(1/r);
  len=2*r*�;
  MGPlot(x+x1,y+y1);
  while (len-->=0) {

//m1@a1 * m2@a2     = m1*m2@(a1+a2)

//(x1+y1i)*(x2+y2i) = x1*x2+(x1*y1+x2*y2)i-y1*y2

// me$SY,-3$ti$SY,0$=mCos(t)+imSin(t)

    x2=x1; y2=y1;
    x1=c*x2-s*y2;
    y1=s*x2+c*y2;
    MGPlot(x+x1,y+y1);
  }
}
 
 
U0 MiniGrLibDemo()
{
  I64 i;
  Bool old_preempt;
  MGInit;

  old_preempt=Preempt(OFF);
  for (i=0;i<100;i++)
    MGHLine(200+i,400+i,300+i);
  for (i=0;i<500;i+=10)
    MGLine(i,0,0,480-i);
  for (i=0;i<300;i+=4)
    MGCircle(200,100+i,i);
  MGUpdate;
  BusyWait(1500000);
  Preempt(old_preempt);
/*
We are returning graphics to normal operations under TempleOS.
It is not normal to by-pass the TempleOS graphcis routines.
The TempleOS graphics don't know VGA has changed.
This bit tells TempleOS to update whole screen.
*/
//<CTRL-ALT-F> will flush screen VGA cache.
  VGAFlush;
}
 
MiniGrLibDemo;

//See $LK,"RawPutChar","MN:RawPutChar"$() for text.
//See $LK-A,"::/TempleOS/Demo/Lectures/ScreenMemory.CPP"$ for color.
//See $LK-A,"::/TempleOS/Demo/Lectures/GraphicsCPULoad.CPP"$.
