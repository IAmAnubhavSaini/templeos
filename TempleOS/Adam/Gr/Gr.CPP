#help_index "Graphics"
U0 GrInit1()
{
  DefineLstLoad("ST_SPRITE_TYPES","End\0\0Color 8Bit\0\0Point\0"
	"Line\0PolyLine\0PolyPoint\0Width\0Shift\0"
	"BSpline2\0BSpline3\0BSpline2 Closed\0BSpline3 Closed\0"
	"\0\0\0Rect\0Circle\0Flood Fill Not Color\0Flood Fill\0BitMap 8Bit\0"
	"\0Text\0Ellipse\0Polygon\0Planar Symmetry\0"
	"Dither Color 8Bit\0Rotated Rect\0Mesh\0Shiftable Mesh\0"
	"Transform On\0Transform Off\0Arrow\0Text Box\0Text Diamond\0");
}
GrInit1;

#help_index "Graphics/CSprite"
#define GRST_END		0
//Unused
#define GRST_COLOR8		2
//Unused
#define GRST_PT			4
#define GRST_LINE		5
#define GRST_POLYLINE		6
#define GRST_POLYPT		7
#define GRST_WIDTH		8
#define GRST_SHIFT		9
#define GRST_BSPLINE2		10
#define GRST_BSPLINE3		11
#define GRST_BSPLINE2_CLOSED	12
#define GRST_BSPLINE3_CLOSED	13
//Unused
//Unused
//Unused
#define GRST_RECT		17
#define GRST_CIRCLE		18
#define GRST_FLOOD_FILL_NOT	19
#define GRST_FLOOD_FILL		20
#define GRST_BITMAP8		21
//Unused
#define GRST_TEXT		23
#define GRST_ELLIPSE		24
#define GRST_POLYGON		25
#define GRST_PLANAR_SYMMETRY	26
#define GRST_DITHER_COLOR8	27
#define GRST_ROTATED_RECT	28
#define GRST_MESH		29
#define GRST_SHIFTABLE_MESH	30
#define GRST_TRANSFORM_ON	31
#define GRST_TRANSFORM_OFF	32
#define GRST_ARROW		33
#define GRST_TEXT_BOX		34
#define GRST_TEXT_DIAMOND	35
#define GRST_NUM_TYPES		36

class CMeshTri
{
  I32 color; //Give one of 0-7 colors.	Colors 8-15 are 0-7 with intensity bit set.
  I32 nums[3]; //Vertex number
};

public class CSprite
{
//See $LK,"Graphics/CSprite","HI:Graphics/CSprite"$
//See $LK,"::/TempleOS/Adam/Gr/GrSpritePlot.CPP","FL:::/TempleOS/Adam/Gr/GrSpritePlot.CPP,1"$ for how CSprite are stored.
  CSprite *next,*last;

  U0 start;
  U8 type;
  union {
    U8 color8;
    U16 dither_color8;
    I32 x1;
    I32 num;
    I32 width;
    I32 vertex_cnt; //Mesh
  }
  union {
    I32 y1;
    I32 height;
    I32 tri_cnt; //Mesh
  }
  union {
    I32 z1;
    I32 x2;
    U8 st[4];
  }
  union {
    I32 y2;
    I32 shiftable_vertex_cnt //Shiftable Mesh
  }
  union {
    F64 linespeed;
    F64 rot_angle;
    I32 shiftable_tri_cnt //Shiftable Mesh
  }
  I32 sides;
};

#help_index "Graphics"

extern CDC *DCNew(I64 type,I64 width,I64 height,CTask *task=NULL,Bool null_bitmap=FALSE);
extern CDC *DCAlias(CDC *dc=NULL,CTask *task=NULL);
extern U0 DCDel(CDC *dc);
extern I64 GrBlot3(CDC *dc,I64 x1,I64 y1,I64 z1,CDC *img);
extern U8 *GrBitMap4ToBitMap8(U8 *dst,U8 *src,I64 src_size,I64 bkcolor);

#include "GrCfg"
#include "GrAsm"
#include "MathGr"
#include "GrScreen"
#include "GrDC"
#include "GrBitMap"
#include "GrPrimatives"
#include "GrComposites"
#include "GrChar"

#define GE_ABORT	(-1)
#define GE_EXIT		0
#define GE_CONT		1

#include "CSpriteNew"
#include "GrSpritePlot"
#include "CSpriteMesh"
#include "CSpriteBitMap"
#include "CSpriteEd"
#include "CSpriteMain"
#help_index ""

