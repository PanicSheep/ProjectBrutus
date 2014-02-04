#include "flip.h"
//#define SAVE_MEMORY

#ifdef SAVE_MEMORY
/**
	OUTFLANK_0 =   64 Byte
	OUTFLANK_1 =   32 Byte
	OUTFLANK_2 =   64 Byte
	OUTFLANK_3 =   64 Byte
	OUTFLANK_4 =   64 Byte
	OUTFLANK_5 =   64 Byte
	OUTFLANK_6 =   32 Byte
	OUTFLANK_7 =   64 Byte
	----------------------
	              448 Byte

	   FLIPS_0 =   33 Byte
	   FLIPS_1 =   17 Byte
	   FLIPS_2 =  130 Byte
	   FLIPS_3 =  131 Byte
	   FLIPS_4 =  133 Byte
	   FLIPS_5 =  137 Byte
	   FLIPS_6 =   17 Byte
	   FLIPS_7 =   33 Byte
	----------------------
	              631 Byte

	======================
	            1'079 Byte
**/
#else
/**
	OUTFLANK_0 =   64 Byte
	OUTFLANK_1 =   32 Byte
	OUTFLANK_2 =   64 Byte
	OUTFLANK_3 =   64 Byte
	OUTFLANK_4 =   64 Byte
	OUTFLANK_5 =   64 Byte
	OUTFLANK_6 =   32 Byte
	OUTFLANK_7 =   64 Byte
	----------------------
	              448 Byte

	   FLIPS_0 =   33 Byte
	   FLIPS_1 =   17 Byte
	   FLIPS_2 =  130 Byte
	   FLIPS_3 =  131 Byte
	   FLIPS_4 =  133 Byte
	   FLIPS_5 =  137 Byte
	   FLIPS_6 =   17 Byte
	   FLIPS_7 =   33 Byte
	----------------------
	              631 Byte

  STRETCH_A1A8 =  512 Byte
  STRETCH_A8A1 =  512 Byte
  STRETCH_V    =  512 Byte
	----------------------
	            1'536 Byte

	======================
	            2'615 Byte
**/
#endif

namespace Flip
{
	const unsigned char OUTFLANK_0[] = {
		OUTFLANK0<0x00>(), OUTFLANK0<0x01>(), OUTFLANK0<0x02>(), OUTFLANK0<0x03>(), OUTFLANK0<0x04>(), OUTFLANK0<0x05>(), OUTFLANK0<0x06>(), OUTFLANK0<0x07>(),
		OUTFLANK0<0x08>(), OUTFLANK0<0x09>(), OUTFLANK0<0x0A>(), OUTFLANK0<0x0B>(), OUTFLANK0<0x0C>(), OUTFLANK0<0x0D>(), OUTFLANK0<0x0E>(), OUTFLANK0<0x0F>(),
		OUTFLANK0<0x10>(), OUTFLANK0<0x11>(), OUTFLANK0<0x12>(), OUTFLANK0<0x13>(), OUTFLANK0<0x14>(), OUTFLANK0<0x15>(), OUTFLANK0<0x16>(), OUTFLANK0<0x17>(),
		OUTFLANK0<0x18>(), OUTFLANK0<0x19>(), OUTFLANK0<0x1A>(), OUTFLANK0<0x1B>(), OUTFLANK0<0x1C>(), OUTFLANK0<0x1D>(), OUTFLANK0<0x1E>(), OUTFLANK0<0x1F>(),
		OUTFLANK0<0x20>(), OUTFLANK0<0x21>(), OUTFLANK0<0x22>(), OUTFLANK0<0x23>(), OUTFLANK0<0x24>(), OUTFLANK0<0x25>(), OUTFLANK0<0x26>(), OUTFLANK0<0x27>(),
		OUTFLANK0<0x28>(), OUTFLANK0<0x29>(), OUTFLANK0<0x2A>(), OUTFLANK0<0x2B>(), OUTFLANK0<0x2C>(), OUTFLANK0<0x2D>(), OUTFLANK0<0x2E>(), OUTFLANK0<0x2F>(),
		OUTFLANK0<0x30>(), OUTFLANK0<0x31>(), OUTFLANK0<0x32>(), OUTFLANK0<0x33>(), OUTFLANK0<0x34>(), OUTFLANK0<0x35>(), OUTFLANK0<0x36>(), OUTFLANK0<0x37>(),
		OUTFLANK0<0x38>(), OUTFLANK0<0x39>(), OUTFLANK0<0x3A>(), OUTFLANK0<0x3B>(), OUTFLANK0<0x3C>(), OUTFLANK0<0x3D>(), OUTFLANK0<0x3E>(), OUTFLANK0<0x3F>()
	};
	const unsigned char OUTFLANK_1[] = {
		OUTFLANK1<0x00>(), OUTFLANK1<0x01>(), OUTFLANK1<0x02>(), OUTFLANK1<0x03>(), OUTFLANK1<0x04>(), OUTFLANK1<0x05>(), OUTFLANK1<0x06>(), OUTFLANK1<0x07>(),
		OUTFLANK1<0x08>(), OUTFLANK1<0x09>(), OUTFLANK1<0x0A>(), OUTFLANK1<0x0B>(), OUTFLANK1<0x0C>(), OUTFLANK1<0x0D>(), OUTFLANK1<0x0E>(), OUTFLANK1<0x0F>(),
		OUTFLANK1<0x10>(), OUTFLANK1<0x11>(), OUTFLANK1<0x12>(), OUTFLANK1<0x13>(),	OUTFLANK1<0x14>(), OUTFLANK1<0x15>(), OUTFLANK1<0x16>(), OUTFLANK1<0x17>(),
		OUTFLANK1<0x18>(), OUTFLANK1<0x19>(), OUTFLANK1<0x1A>(), OUTFLANK1<0x1B>(), OUTFLANK1<0x1C>(), OUTFLANK1<0x1D>(), OUTFLANK1<0x1E>(), OUTFLANK1<0x1F>()
	};
	const unsigned char OUTFLANK_2[] = {
		OUTFLANK2<0x00>(), OUTFLANK2<0x01>(), OUTFLANK2<0x02>(), OUTFLANK2<0x03>(), OUTFLANK2<0x04>(), OUTFLANK2<0x05>(), OUTFLANK2<0x06>(), OUTFLANK2<0x07>(),
		OUTFLANK2<0x08>(), OUTFLANK2<0x09>(), OUTFLANK2<0x0A>(), OUTFLANK2<0x0B>(), OUTFLANK2<0x0C>(), OUTFLANK2<0x0D>(), OUTFLANK2<0x0E>(), OUTFLANK2<0x0F>(),
		OUTFLANK2<0x10>(), OUTFLANK2<0x11>(), OUTFLANK2<0x12>(), OUTFLANK2<0x13>(), OUTFLANK2<0x14>(), OUTFLANK2<0x15>(), OUTFLANK2<0x16>(), OUTFLANK2<0x17>(),
		OUTFLANK2<0x18>(), OUTFLANK2<0x19>(), OUTFLANK2<0x1A>(), OUTFLANK2<0x1B>(), OUTFLANK2<0x1C>(), OUTFLANK2<0x1D>(), OUTFLANK2<0x1E>(), OUTFLANK2<0x1F>(),
		OUTFLANK2<0x20>(), OUTFLANK2<0x21>(), OUTFLANK2<0x22>(), OUTFLANK2<0x23>(), OUTFLANK2<0x24>(), OUTFLANK2<0x25>(), OUTFLANK2<0x26>(), OUTFLANK2<0x27>(),
		OUTFLANK2<0x28>(), OUTFLANK2<0x29>(), OUTFLANK2<0x2A>(), OUTFLANK2<0x2B>(), OUTFLANK2<0x2C>(), OUTFLANK2<0x2D>(), OUTFLANK2<0x2E>(), OUTFLANK2<0x2F>(),
		OUTFLANK2<0x30>(), OUTFLANK2<0x31>(), OUTFLANK2<0x32>(), OUTFLANK2<0x33>(), OUTFLANK2<0x34>(), OUTFLANK2<0x35>(), OUTFLANK2<0x36>(), OUTFLANK2<0x37>(),
		OUTFLANK2<0x38>(), OUTFLANK2<0x39>(), OUTFLANK2<0x3A>(), OUTFLANK2<0x3B>(), OUTFLANK2<0x3C>(), OUTFLANK2<0x3D>(), OUTFLANK2<0x3E>(), OUTFLANK2<0x3F>()
	};
	const unsigned char OUTFLANK_3[] = {
		OUTFLANK3<0x00>(), OUTFLANK3<0x01>(), OUTFLANK3<0x02>(), OUTFLANK3<0x03>(), OUTFLANK3<0x04>(), OUTFLANK3<0x05>(), OUTFLANK3<0x06>(), OUTFLANK3<0x07>(),
		OUTFLANK3<0x08>(), OUTFLANK3<0x09>(), OUTFLANK3<0x0A>(), OUTFLANK3<0x0B>(), OUTFLANK3<0x0C>(), OUTFLANK3<0x0D>(), OUTFLANK3<0x0E>(), OUTFLANK3<0x0F>(),
		OUTFLANK3<0x10>(), OUTFLANK3<0x11>(), OUTFLANK3<0x12>(), OUTFLANK3<0x13>(), OUTFLANK3<0x14>(), OUTFLANK3<0x15>(), OUTFLANK3<0x16>(), OUTFLANK3<0x17>(),
		OUTFLANK3<0x18>(), OUTFLANK3<0x19>(), OUTFLANK3<0x1A>(), OUTFLANK3<0x1B>(), OUTFLANK3<0x1C>(), OUTFLANK3<0x1D>(), OUTFLANK3<0x1E>(), OUTFLANK3<0x1F>(),
		OUTFLANK3<0x20>(), OUTFLANK3<0x21>(), OUTFLANK3<0x22>(), OUTFLANK3<0x23>(), OUTFLANK3<0x24>(), OUTFLANK3<0x25>(), OUTFLANK3<0x26>(), OUTFLANK3<0x27>(),
		OUTFLANK3<0x28>(), OUTFLANK3<0x29>(), OUTFLANK3<0x2A>(), OUTFLANK3<0x2B>(), OUTFLANK3<0x2C>(), OUTFLANK3<0x2D>(), OUTFLANK3<0x2E>(), OUTFLANK3<0x2F>(),
		OUTFLANK3<0x30>(), OUTFLANK3<0x31>(), OUTFLANK3<0x32>(), OUTFLANK3<0x33>(), OUTFLANK3<0x34>(), OUTFLANK3<0x35>(), OUTFLANK3<0x36>(), OUTFLANK3<0x37>(),
		OUTFLANK3<0x38>(), OUTFLANK3<0x39>(), OUTFLANK3<0x3A>(), OUTFLANK3<0x3B>(), OUTFLANK3<0x3C>(), OUTFLANK3<0x3D>(), OUTFLANK3<0x3E>(), OUTFLANK3<0x3F>()
	};
	const unsigned char OUTFLANK_4[] = {
		OUTFLANK4<0x00>(), OUTFLANK4<0x01>(), OUTFLANK4<0x02>(), OUTFLANK4<0x03>(), OUTFLANK4<0x04>(), OUTFLANK4<0x05>(), OUTFLANK4<0x06>(), OUTFLANK4<0x07>(),
		OUTFLANK4<0x08>(), OUTFLANK4<0x09>(), OUTFLANK4<0x0A>(), OUTFLANK4<0x0B>(), OUTFLANK4<0x0C>(), OUTFLANK4<0x0D>(), OUTFLANK4<0x0E>(), OUTFLANK4<0x0F>(),
		OUTFLANK4<0x10>(), OUTFLANK4<0x11>(), OUTFLANK4<0x12>(), OUTFLANK4<0x13>(), OUTFLANK4<0x14>(), OUTFLANK4<0x15>(), OUTFLANK4<0x16>(), OUTFLANK4<0x17>(),
		OUTFLANK4<0x18>(), OUTFLANK4<0x19>(), OUTFLANK4<0x1A>(), OUTFLANK4<0x1B>(), OUTFLANK4<0x1C>(), OUTFLANK4<0x1D>(), OUTFLANK4<0x1E>(), OUTFLANK4<0x1F>(),
		OUTFLANK4<0x20>(), OUTFLANK4<0x21>(), OUTFLANK4<0x22>(), OUTFLANK4<0x23>(), OUTFLANK4<0x24>(), OUTFLANK4<0x25>(), OUTFLANK4<0x26>(), OUTFLANK4<0x27>(),
		OUTFLANK4<0x28>(), OUTFLANK4<0x29>(), OUTFLANK4<0x2A>(), OUTFLANK4<0x2B>(), OUTFLANK4<0x2C>(), OUTFLANK4<0x2D>(), OUTFLANK4<0x2E>(), OUTFLANK4<0x2F>(),
		OUTFLANK4<0x30>(), OUTFLANK4<0x31>(), OUTFLANK4<0x32>(), OUTFLANK4<0x33>(), OUTFLANK4<0x34>(), OUTFLANK4<0x35>(), OUTFLANK4<0x36>(), OUTFLANK4<0x37>(),
		OUTFLANK4<0x38>(), OUTFLANK4<0x39>(), OUTFLANK4<0x3A>(), OUTFLANK4<0x3B>(), OUTFLANK4<0x3C>(), OUTFLANK4<0x3D>(), OUTFLANK4<0x3E>(), OUTFLANK4<0x3F>()
	};
	const unsigned char OUTFLANK_5[] = {
		OUTFLANK5<0x00>(), OUTFLANK5<0x01>(), OUTFLANK5<0x02>(), OUTFLANK5<0x03>(), OUTFLANK5<0x04>(), OUTFLANK5<0x05>(), OUTFLANK5<0x06>(), OUTFLANK5<0x07>(),
		OUTFLANK5<0x08>(), OUTFLANK5<0x09>(), OUTFLANK5<0x0A>(), OUTFLANK5<0x0B>(), OUTFLANK5<0x0C>(), OUTFLANK5<0x0D>(), OUTFLANK5<0x0E>(), OUTFLANK5<0x0F>(),
		OUTFLANK5<0x10>(), OUTFLANK5<0x11>(), OUTFLANK5<0x12>(), OUTFLANK5<0x13>(), OUTFLANK5<0x14>(), OUTFLANK5<0x15>(), OUTFLANK5<0x16>(), OUTFLANK5<0x17>(),
		OUTFLANK5<0x18>(), OUTFLANK5<0x19>(), OUTFLANK5<0x1A>(), OUTFLANK5<0x1B>(), OUTFLANK5<0x1C>(), OUTFLANK5<0x1D>(), OUTFLANK5<0x1E>(), OUTFLANK5<0x1F>(),
		OUTFLANK5<0x20>(), OUTFLANK5<0x21>(), OUTFLANK5<0x22>(), OUTFLANK5<0x23>(), OUTFLANK5<0x24>(), OUTFLANK5<0x25>(), OUTFLANK5<0x26>(), OUTFLANK5<0x27>(),
		OUTFLANK5<0x28>(), OUTFLANK5<0x29>(), OUTFLANK5<0x2A>(), OUTFLANK5<0x2B>(), OUTFLANK5<0x2C>(), OUTFLANK5<0x2D>(), OUTFLANK5<0x2E>(), OUTFLANK5<0x2F>(),
		OUTFLANK5<0x30>(), OUTFLANK5<0x31>(), OUTFLANK5<0x32>(), OUTFLANK5<0x33>(), OUTFLANK5<0x34>(), OUTFLANK5<0x35>(), OUTFLANK5<0x36>(), OUTFLANK5<0x37>(),
		OUTFLANK5<0x38>(), OUTFLANK5<0x39>(), OUTFLANK5<0x3A>(), OUTFLANK5<0x3B>(), OUTFLANK5<0x3C>(), OUTFLANK5<0x3D>(), OUTFLANK5<0x3E>(), OUTFLANK5<0x3F>()
	};
	const unsigned char OUTFLANK_6[] = {
		OUTFLANK6<0x00>(), OUTFLANK6<0x01>(), OUTFLANK6<0x02>(), OUTFLANK6<0x03>(),	OUTFLANK6<0x04>(), OUTFLANK6<0x05>(), OUTFLANK6<0x06>(), OUTFLANK6<0x07>(), 
		OUTFLANK6<0x08>(), OUTFLANK6<0x09>(), OUTFLANK6<0x0A>(), OUTFLANK6<0x0B>(),	OUTFLANK6<0x0C>(), OUTFLANK6<0x0D>(), OUTFLANK6<0x0E>(), OUTFLANK6<0x0F>(),
		OUTFLANK6<0x10>(), OUTFLANK6<0x11>(), OUTFLANK6<0x12>(), OUTFLANK6<0x13>(),	OUTFLANK6<0x14>(), OUTFLANK6<0x15>(), OUTFLANK6<0x16>(), OUTFLANK6<0x17>(), 
		OUTFLANK6<0x18>(), OUTFLANK6<0x19>(), OUTFLANK6<0x1A>(), OUTFLANK6<0x1B>(),	OUTFLANK6<0x1C>(), OUTFLANK6<0x1D>(), OUTFLANK6<0x1E>(), OUTFLANK6<0x1F>()
	};
	const unsigned char OUTFLANK_7[] = {
		OUTFLANK7<0x00>(), OUTFLANK7<0x01>(), OUTFLANK7<0x02>(), OUTFLANK7<0x03>(), OUTFLANK7<0x04>(), OUTFLANK7<0x05>(), OUTFLANK7<0x06>(), OUTFLANK7<0x07>(),
		OUTFLANK7<0x08>(), OUTFLANK7<0x09>(), OUTFLANK7<0x0A>(), OUTFLANK7<0x0B>(), OUTFLANK7<0x0C>(), OUTFLANK7<0x0D>(), OUTFLANK7<0x0E>(), OUTFLANK7<0x0F>(),
		OUTFLANK7<0x10>(), OUTFLANK7<0x11>(), OUTFLANK7<0x12>(), OUTFLANK7<0x13>(), OUTFLANK7<0x14>(), OUTFLANK7<0x15>(), OUTFLANK7<0x16>(), OUTFLANK7<0x17>(), 
		OUTFLANK7<0x18>(), OUTFLANK7<0x19>(), OUTFLANK7<0x1A>(), OUTFLANK7<0x1B>(), OUTFLANK7<0x1C>(), OUTFLANK7<0x1D>(), OUTFLANK7<0x1E>(), OUTFLANK7<0x1F>(),
		OUTFLANK7<0x20>(), OUTFLANK7<0x21>(), OUTFLANK7<0x22>(), OUTFLANK7<0x23>(), OUTFLANK7<0x24>(), OUTFLANK7<0x25>(), OUTFLANK7<0x26>(), OUTFLANK7<0x27>(), 
		OUTFLANK7<0x28>(), OUTFLANK7<0x29>(), OUTFLANK7<0x2A>(), OUTFLANK7<0x2B>(), OUTFLANK7<0x2C>(), OUTFLANK7<0x2D>(), OUTFLANK7<0x2E>(), OUTFLANK7<0x2F>(),
		OUTFLANK7<0x30>(), OUTFLANK7<0x31>(), OUTFLANK7<0x32>(), OUTFLANK7<0x33>(), OUTFLANK7<0x34>(), OUTFLANK7<0x35>(), OUTFLANK7<0x36>(), OUTFLANK7<0x37>(), 
		OUTFLANK7<0x38>(), OUTFLANK7<0x39>(), OUTFLANK7<0x3A>(), OUTFLANK7<0x3B>(), OUTFLANK7<0x3C>(), OUTFLANK7<0x3D>(), OUTFLANK7<0x3E>(), OUTFLANK7<0x3F>()
	};

	const unsigned char FLIPS_0[] = {
		FLIP0<0x00>(), FLIP0<0x01>(), FLIP0<0x02>(), FLIP0<0x03>(), FLIP0<0x04>(), FLIP0<0x05>(), FLIP0<0x06>(), FLIP0<0x07>(),
		FLIP0<0x08>(), FLIP0<0x09>(), FLIP0<0x0A>(), FLIP0<0x0B>(), FLIP0<0x0C>(), FLIP0<0x0D>(), FLIP0<0x0E>(), FLIP0<0x0F>(),
		FLIP0<0x10>(), FLIP0<0x11>(), FLIP0<0x12>(), FLIP0<0x13>(), FLIP0<0x14>(), FLIP0<0x15>(), FLIP0<0x16>(), FLIP0<0x17>(),
		FLIP0<0x18>(), FLIP0<0x19>(), FLIP0<0x1A>(), FLIP0<0x1B>(), FLIP0<0x1C>(), FLIP0<0x1D>(), FLIP0<0x1E>(), FLIP0<0x1F>(),
		FLIP0<0x20>()
	};
	const unsigned char FLIPS_1[] = {
		FLIP1<0x00>(), FLIP1<0x01>(), FLIP1<0x02>(), FLIP1<0x03>(), 
		FLIP1<0x04>(), FLIP1<0x05>(), FLIP1<0x06>(), FLIP1<0x07>(),
		FLIP1<0x08>(), FLIP1<0x09>(), FLIP1<0x0A>(), FLIP1<0x0B>(),
		FLIP1<0x0C>(), FLIP1<0x0D>(), FLIP1<0x0E>(), FLIP1<0x0F>(),
		FLIP1<0x10>()
	};
	const unsigned char FLIPS_2[] = {
		FLIP2<0x00>(), FLIP2<0x01>(), FLIP2<0x02>(), FLIP2<0x03>(), FLIP2<0x04>(), FLIP2<0x05>(), FLIP2<0x06>(), FLIP2<0x07>(),
		FLIP2<0x08>(), FLIP2<0x09>(), FLIP2<0x0A>(), FLIP2<0x0B>(), FLIP2<0x0C>(), FLIP2<0x0D>(), FLIP2<0x0E>(), FLIP2<0x0F>(),
		FLIP2<0x10>(), FLIP2<0x11>(), FLIP2<0x12>(), FLIP2<0x13>(), FLIP2<0x14>(), FLIP2<0x15>(), FLIP2<0x16>(), FLIP2<0x17>(),
		FLIP2<0x18>(), FLIP2<0x19>(), FLIP2<0x1A>(), FLIP2<0x1B>(), FLIP2<0x1C>(), FLIP2<0x1D>(), FLIP2<0x1E>(), FLIP2<0x1F>(),
		FLIP2<0x20>(), FLIP2<0x21>(), FLIP2<0x22>(), FLIP2<0x23>(), FLIP2<0x24>(), FLIP2<0x25>(), FLIP2<0x26>(), FLIP2<0x27>(), 
		FLIP2<0x28>(), FLIP2<0x29>(), FLIP2<0x2A>(), FLIP2<0x2B>(), FLIP2<0x2C>(), FLIP2<0x2D>(), FLIP2<0x2E>(), FLIP2<0x2F>(),
		FLIP2<0x30>(), FLIP2<0x31>(), FLIP2<0x32>(), FLIP2<0x33>(), FLIP2<0x34>(), FLIP2<0x35>(), FLIP2<0x36>(), FLIP2<0x37>(), 
		FLIP2<0x38>(), FLIP2<0x39>(), FLIP2<0x3A>(), FLIP2<0x3B>(), FLIP2<0x3C>(), FLIP2<0x3D>(), FLIP2<0x3E>(), FLIP2<0x3F>(),
		FLIP2<0x40>(), FLIP2<0x41>(), FLIP2<0x42>(), FLIP2<0x43>(), FLIP2<0x44>(), FLIP2<0x45>(), FLIP2<0x46>(), FLIP2<0x47>(), 
		FLIP2<0x48>(), FLIP2<0x49>(), FLIP2<0x4A>(), FLIP2<0x4B>(), FLIP2<0x4C>(), FLIP2<0x4D>(), FLIP2<0x4E>(), FLIP2<0x4F>(),
		FLIP2<0x50>(), FLIP2<0x51>(), FLIP2<0x52>(), FLIP2<0x53>(), FLIP2<0x54>(), FLIP2<0x55>(), FLIP2<0x56>(), FLIP2<0x57>(), 
		FLIP2<0x58>(), FLIP2<0x59>(), FLIP2<0x5A>(), FLIP2<0x5B>(), FLIP2<0x5C>(), FLIP2<0x5D>(), FLIP2<0x5E>(), FLIP2<0x5F>(),
		FLIP2<0x60>(), FLIP2<0x61>(), FLIP2<0x62>(), FLIP2<0x63>(), FLIP2<0x64>(), FLIP2<0x65>(), FLIP2<0x66>(), FLIP2<0x67>(),
		FLIP2<0x68>(), FLIP2<0x69>(), FLIP2<0x6A>(), FLIP2<0x6B>(), FLIP2<0x6C>(), FLIP2<0x6D>(), FLIP2<0x6E>(), FLIP2<0x6F>(),
		FLIP2<0x70>(), FLIP2<0x71>(), FLIP2<0x72>(), FLIP2<0x73>(), FLIP2<0x74>(), FLIP2<0x75>(), FLIP2<0x76>(), FLIP2<0x77>(), 
		FLIP2<0x78>(), FLIP2<0x79>(), FLIP2<0x7A>(), FLIP2<0x7B>(), FLIP2<0x7C>(), FLIP2<0x7D>(), FLIP2<0x7E>(), FLIP2<0x7F>(),
		FLIP2<0x80>(), FLIP2<0x81>()
	};
	const unsigned char FLIPS_3[] = {
		FLIP3<0x00>(), FLIP3<0x01>(), FLIP3<0x02>(), FLIP3<0x03>(), FLIP3<0x04>(), FLIP3<0x05>(), FLIP3<0x06>(), FLIP3<0x07>(),
		FLIP3<0x08>(), FLIP3<0x09>(), FLIP3<0x0A>(), FLIP3<0x0B>(), FLIP3<0x0C>(), FLIP3<0x0D>(), FLIP3<0x0E>(), FLIP3<0x0F>(),
		FLIP3<0x10>(), FLIP3<0x11>(), FLIP3<0x12>(), FLIP3<0x13>(), FLIP3<0x14>(), FLIP3<0x15>(), FLIP3<0x16>(), FLIP3<0x17>(),
		FLIP3<0x18>(), FLIP3<0x19>(), FLIP3<0x1A>(), FLIP3<0x1B>(), FLIP3<0x1C>(), FLIP3<0x1D>(), FLIP3<0x1E>(), FLIP3<0x1F>(),
		FLIP3<0x20>(), FLIP3<0x21>(), FLIP3<0x22>(), FLIP3<0x23>(), FLIP3<0x24>(), FLIP3<0x25>(), FLIP3<0x26>(), FLIP3<0x27>(),
		FLIP3<0x28>(), FLIP3<0x29>(), FLIP3<0x2A>(), FLIP3<0x2B>(), FLIP3<0x2C>(), FLIP3<0x2D>(), FLIP3<0x2E>(), FLIP3<0x2F>(),
		FLIP3<0x30>(), FLIP3<0x31>(), FLIP3<0x32>(), FLIP3<0x33>(), FLIP3<0x34>(), FLIP3<0x35>(), FLIP3<0x36>(), FLIP3<0x37>(), 
		FLIP3<0x38>(), FLIP3<0x39>(), FLIP3<0x3A>(), FLIP3<0x3B>(), FLIP3<0x3C>(), FLIP3<0x3D>(), FLIP3<0x3E>(), FLIP3<0x3F>(),
		FLIP3<0x40>(), FLIP3<0x41>(), FLIP3<0x42>(), FLIP3<0x43>(), FLIP3<0x44>(), FLIP3<0x45>(), FLIP3<0x46>(), FLIP3<0x47>(),
		FLIP3<0x48>(), FLIP3<0x49>(), FLIP3<0x4A>(), FLIP3<0x4B>(), FLIP3<0x4C>(), FLIP3<0x4D>(), FLIP3<0x4E>(), FLIP3<0x4F>(),
		FLIP3<0x50>(), FLIP3<0x51>(), FLIP3<0x52>(), FLIP3<0x53>(), FLIP3<0x54>(), FLIP3<0x55>(), FLIP3<0x56>(), FLIP3<0x57>(),
		FLIP3<0x58>(), FLIP3<0x59>(), FLIP3<0x5A>(), FLIP3<0x5B>(), FLIP3<0x5C>(), FLIP3<0x5D>(), FLIP3<0x5E>(), FLIP3<0x5F>(),
		FLIP3<0x60>(), FLIP3<0x61>(), FLIP3<0x62>(), FLIP3<0x63>(), FLIP3<0x64>(), FLIP3<0x65>(), FLIP3<0x66>(), FLIP3<0x67>(),
		FLIP3<0x68>(), FLIP3<0x69>(), FLIP3<0x6A>(), FLIP3<0x6B>(), FLIP3<0x6C>(), FLIP3<0x6D>(), FLIP3<0x6E>(), FLIP3<0x6F>(),
		FLIP3<0x70>(), FLIP3<0x71>(), FLIP3<0x72>(), FLIP3<0x73>(), FLIP3<0x74>(), FLIP3<0x75>(), FLIP3<0x76>(), FLIP3<0x77>(),
		FLIP3<0x78>(), FLIP3<0x79>(), FLIP3<0x7A>(), FLIP3<0x7B>(), FLIP3<0x7C>(), FLIP3<0x7D>(), FLIP3<0x7E>(), FLIP3<0x7F>(),
		FLIP3<0x80>(), FLIP3<0x81>(), FLIP3<0x82>()
	};
	const unsigned char FLIPS_4[] = {
		FLIP4<0x00>(), FLIP4<0x01>(), FLIP4<0x02>(), FLIP4<0x03>(), FLIP4<0x04>(), FLIP4<0x05>(), FLIP4<0x06>(), FLIP4<0x07>(),
		FLIP4<0x08>(), FLIP4<0x09>(), FLIP4<0x0A>(), FLIP4<0x0B>(), FLIP4<0x0C>(), FLIP4<0x0D>(), FLIP4<0x0E>(), FLIP4<0x0F>(),
		FLIP4<0x10>(), FLIP4<0x11>(), FLIP4<0x12>(), FLIP4<0x13>(), FLIP4<0x14>(), FLIP4<0x15>(), FLIP4<0x16>(), FLIP4<0x17>(),
		FLIP4<0x18>(), FLIP4<0x19>(), FLIP4<0x1A>(), FLIP4<0x1B>(), FLIP4<0x1C>(), FLIP4<0x1D>(), FLIP4<0x1E>(), FLIP4<0x1F>(),
		FLIP4<0x20>(), FLIP4<0x21>(), FLIP4<0x22>(), FLIP4<0x23>(), FLIP4<0x24>(), FLIP4<0x25>(), FLIP4<0x26>(), FLIP4<0x27>(),
		FLIP4<0x28>(), FLIP4<0x29>(), FLIP4<0x2A>(), FLIP4<0x2B>(), FLIP4<0x2C>(), FLIP4<0x2D>(), FLIP4<0x2E>(), FLIP4<0x2F>(),
		FLIP4<0x30>(), FLIP4<0x31>(), FLIP4<0x32>(), FLIP4<0x33>(), FLIP4<0x34>(), FLIP4<0x35>(), FLIP4<0x36>(), FLIP4<0x37>(),
		FLIP4<0x38>(), FLIP4<0x39>(), FLIP4<0x3A>(), FLIP4<0x3B>(), FLIP4<0x3C>(), FLIP4<0x3D>(), FLIP4<0x3E>(), FLIP4<0x3F>(),
		FLIP4<0x40>(), FLIP4<0x41>(), FLIP4<0x42>(), FLIP4<0x43>(), FLIP4<0x44>(), FLIP4<0x45>(), FLIP4<0x46>(), FLIP4<0x47>(),
		FLIP4<0x48>(), FLIP4<0x49>(), FLIP4<0x4A>(), FLIP4<0x4B>(), FLIP4<0x4C>(), FLIP4<0x4D>(), FLIP4<0x4E>(), FLIP4<0x4F>(),
		FLIP4<0x50>(), FLIP4<0x51>(), FLIP4<0x52>(), FLIP4<0x53>(), FLIP4<0x54>(), FLIP4<0x55>(), FLIP4<0x56>(), FLIP4<0x57>(),
		FLIP4<0x58>(), FLIP4<0x59>(), FLIP4<0x5A>(), FLIP4<0x5B>(), FLIP4<0x5C>(), FLIP4<0x5D>(), FLIP4<0x5E>(), FLIP4<0x5F>(),
		FLIP4<0x60>(), FLIP4<0x61>(), FLIP4<0x62>(), FLIP4<0x63>(), FLIP4<0x64>(), FLIP4<0x65>(), FLIP4<0x66>(), FLIP4<0x67>(),
		FLIP4<0x68>(), FLIP4<0x69>(), FLIP4<0x6A>(), FLIP4<0x6B>(), FLIP4<0x6C>(), FLIP4<0x6D>(), FLIP4<0x6E>(), FLIP4<0x6F>(),
		FLIP4<0x70>(), FLIP4<0x71>(), FLIP4<0x72>(), FLIP4<0x73>(), FLIP4<0x74>(), FLIP4<0x75>(), FLIP4<0x76>(), FLIP4<0x77>(),
		FLIP4<0x78>(), FLIP4<0x79>(), FLIP4<0x7A>(), FLIP4<0x7B>(), FLIP4<0x7C>(), FLIP4<0x7D>(), FLIP4<0x7E>(), FLIP4<0x7F>(),
		FLIP4<0x80>(), FLIP4<0x81>(), FLIP4<0x82>(), FLIP4<0x83>(), FLIP4<0x84>()
	};
	const unsigned char FLIPS_5[] = {
		FLIP5<0x00>(), FLIP5<0x01>(), FLIP5<0x02>(), FLIP5<0x03>(), FLIP5<0x04>(), FLIP5<0x05>(), FLIP5<0x06>(), FLIP5<0x07>(),
		FLIP5<0x08>(), FLIP5<0x09>(), FLIP5<0x0A>(), FLIP5<0x0B>(), FLIP5<0x0C>(), FLIP5<0x0D>(), FLIP5<0x0E>(), FLIP5<0x0F>(),
		FLIP5<0x10>(), FLIP5<0x11>(), FLIP5<0x12>(), FLIP5<0x13>(), FLIP5<0x14>(), FLIP5<0x15>(), FLIP5<0x16>(), FLIP5<0x17>(),
		FLIP5<0x18>(), FLIP5<0x19>(), FLIP5<0x1A>(), FLIP5<0x1B>(), FLIP5<0x1C>(), FLIP5<0x1D>(), FLIP5<0x1E>(), FLIP5<0x1F>(),
		FLIP5<0x20>(), FLIP5<0x21>(), FLIP5<0x22>(), FLIP5<0x23>(), FLIP5<0x24>(), FLIP5<0x25>(), FLIP5<0x26>(), FLIP5<0x27>(),
		FLIP5<0x28>(), FLIP5<0x29>(), FLIP5<0x2A>(), FLIP5<0x2B>(), FLIP5<0x2C>(), FLIP5<0x2D>(), FLIP5<0x2E>(), FLIP5<0x2F>(),
		FLIP5<0x30>(), FLIP5<0x31>(), FLIP5<0x32>(), FLIP5<0x33>(), FLIP5<0x34>(), FLIP5<0x35>(), FLIP5<0x36>(), FLIP5<0x37>(),
		FLIP5<0x38>(), FLIP5<0x39>(), FLIP5<0x3A>(), FLIP5<0x3B>(), FLIP5<0x3C>(), FLIP5<0x3D>(), FLIP5<0x3E>(), FLIP5<0x3F>(),
		FLIP5<0x40>(), FLIP5<0x41>(), FLIP5<0x42>(), FLIP5<0x43>(), FLIP5<0x44>(), FLIP5<0x45>(), FLIP5<0x46>(), FLIP5<0x47>(),
		FLIP5<0x48>(), FLIP5<0x49>(), FLIP5<0x4A>(), FLIP5<0x4B>(), FLIP5<0x4C>(), FLIP5<0x4D>(), FLIP5<0x4E>(), FLIP5<0x4F>(),
		FLIP5<0x50>(), FLIP5<0x51>(), FLIP5<0x52>(), FLIP5<0x53>(), FLIP5<0x54>(), FLIP5<0x55>(), FLIP5<0x56>(), FLIP5<0x57>(),
		FLIP5<0x58>(), FLIP5<0x59>(), FLIP5<0x5A>(), FLIP5<0x5B>(), FLIP5<0x5C>(), FLIP5<0x5D>(), FLIP5<0x5E>(), FLIP5<0x5F>(),
		FLIP5<0x60>(), FLIP5<0x61>(), FLIP5<0x62>(), FLIP5<0x63>(), FLIP5<0x64>(), FLIP5<0x65>(), FLIP5<0x66>(), FLIP5<0x67>(),
		FLIP5<0x68>(), FLIP5<0x69>(), FLIP5<0x6A>(), FLIP5<0x6B>(), FLIP5<0x6C>(), FLIP5<0x6D>(), FLIP5<0x6E>(), FLIP5<0x6F>(),
		FLIP5<0x70>(), FLIP5<0x71>(), FLIP5<0x72>(), FLIP5<0x73>(), FLIP5<0x74>(), FLIP5<0x75>(), FLIP5<0x76>(), FLIP5<0x77>(),
		FLIP5<0x78>(), FLIP5<0x79>(), FLIP5<0x7A>(), FLIP5<0x7B>(), FLIP5<0x7C>(), FLIP5<0x7D>(), FLIP5<0x7E>(), FLIP5<0x7F>(),
		FLIP5<0x80>(), FLIP5<0x81>(), FLIP5<0x82>(), FLIP5<0x83>(), FLIP5<0x84>(), FLIP5<0x85>(), FLIP5<0x86>(), FLIP5<0x87>(),
		FLIP5<0x88>()
	};
	const unsigned char FLIPS_6[] = {
		FLIP6<0x00>(), FLIP6<0x01>(), FLIP6<0x02>(), FLIP6<0x03>(), 
		FLIP6<0x04>(), FLIP6<0x05>(), FLIP6<0x06>(), FLIP6<0x07>(),
		FLIP6<0x08>(), FLIP6<0x09>(), FLIP6<0x0A>(), FLIP6<0x0B>(),
		FLIP6<0x0C>(), FLIP6<0x0D>(), FLIP6<0x0E>(), FLIP6<0x0F>(),
		FLIP6<0x10>()
	};
	const unsigned char FLIPS_7[] = {
		FLIP7<0x00>(), FLIP7<0x01>(), FLIP7<0x02>(), FLIP7<0x03>(), FLIP7<0x04>(), FLIP7<0x05>(), FLIP7<0x06>(), FLIP7<0x07>(),
		FLIP7<0x08>(), FLIP7<0x09>(), FLIP7<0x0A>(), FLIP7<0x0B>(), FLIP7<0x0C>(), FLIP7<0x0D>(), FLIP7<0x0E>(), FLIP7<0x0F>(),
		FLIP7<0x10>(), FLIP7<0x11>(), FLIP7<0x12>(), FLIP7<0x13>(), FLIP7<0x14>(), FLIP7<0x15>(), FLIP7<0x16>(), FLIP7<0x17>(),
		FLIP7<0x18>(), FLIP7<0x19>(), FLIP7<0x1A>(), FLIP7<0x1B>(), FLIP7<0x1C>(), FLIP7<0x1D>(), FLIP7<0x1E>(), FLIP7<0x1F>(),
		FLIP7<0x20>()
	};

	#ifndef SAVE_MEMORY
	const unsigned long long STRETCH_A1A8[64] = {
		_STRETCH_A1A8<0x00>(), _STRETCH_A1A8<0x01>(), _STRETCH_A1A8<0x02>(), _STRETCH_A1A8<0x03>(), _STRETCH_A1A8<0x04>(), _STRETCH_A1A8<0x05>(), _STRETCH_A1A8<0x06>(), _STRETCH_A1A8<0x07>(),	
		_STRETCH_A1A8<0x08>(), _STRETCH_A1A8<0x09>(), _STRETCH_A1A8<0x0A>(), _STRETCH_A1A8<0x0B>(), _STRETCH_A1A8<0x0C>(), _STRETCH_A1A8<0x0D>(), _STRETCH_A1A8<0x0E>(), _STRETCH_A1A8<0x0F>(),
		_STRETCH_A1A8<0x10>(), _STRETCH_A1A8<0x11>(), _STRETCH_A1A8<0x12>(), _STRETCH_A1A8<0x13>(), _STRETCH_A1A8<0x14>(), _STRETCH_A1A8<0x15>(), _STRETCH_A1A8<0x16>(), _STRETCH_A1A8<0x17>(),	
		_STRETCH_A1A8<0x18>(), _STRETCH_A1A8<0x19>(), _STRETCH_A1A8<0x1A>(), _STRETCH_A1A8<0x1B>(), _STRETCH_A1A8<0x1C>(), _STRETCH_A1A8<0x1D>(), _STRETCH_A1A8<0x1E>(), _STRETCH_A1A8<0x1F>(),
		_STRETCH_A1A8<0x20>(), _STRETCH_A1A8<0x21>(), _STRETCH_A1A8<0x22>(), _STRETCH_A1A8<0x23>(), _STRETCH_A1A8<0x24>(), _STRETCH_A1A8<0x25>(), _STRETCH_A1A8<0x26>(), _STRETCH_A1A8<0x27>(),
		_STRETCH_A1A8<0x28>(), _STRETCH_A1A8<0x29>(), _STRETCH_A1A8<0x2A>(), _STRETCH_A1A8<0x2B>(), _STRETCH_A1A8<0x2C>(), _STRETCH_A1A8<0x2D>(), _STRETCH_A1A8<0x2E>(), _STRETCH_A1A8<0x2F>(),
		_STRETCH_A1A8<0x30>(), _STRETCH_A1A8<0x31>(), _STRETCH_A1A8<0x32>(), _STRETCH_A1A8<0x33>(), _STRETCH_A1A8<0x34>(), _STRETCH_A1A8<0x35>(), _STRETCH_A1A8<0x36>(), _STRETCH_A1A8<0x37>(),
		_STRETCH_A1A8<0x38>(), _STRETCH_A1A8<0x39>(), _STRETCH_A1A8<0x3A>(), _STRETCH_A1A8<0x3B>(), _STRETCH_A1A8<0x3C>(), _STRETCH_A1A8<0x3D>(), _STRETCH_A1A8<0x3E>(), _STRETCH_A1A8<0x3F>()
	};
	const unsigned long long STRETCH_A8A1[64] = {
		_STRETCH_A8A1<0x00>(), _STRETCH_A8A1<0x01>(), _STRETCH_A8A1<0x02>(), _STRETCH_A8A1<0x03>(), _STRETCH_A8A1<0x04>(), _STRETCH_A8A1<0x05>(), _STRETCH_A8A1<0x06>(), _STRETCH_A8A1<0x07>(),	
		_STRETCH_A8A1<0x08>(), _STRETCH_A8A1<0x09>(), _STRETCH_A8A1<0x0A>(), _STRETCH_A8A1<0x0B>(), _STRETCH_A8A1<0x0C>(), _STRETCH_A8A1<0x0D>(), _STRETCH_A8A1<0x0E>(), _STRETCH_A8A1<0x0F>(),
		_STRETCH_A8A1<0x10>(), _STRETCH_A8A1<0x11>(), _STRETCH_A8A1<0x12>(), _STRETCH_A8A1<0x13>(), _STRETCH_A8A1<0x14>(), _STRETCH_A8A1<0x15>(), _STRETCH_A8A1<0x16>(), _STRETCH_A8A1<0x17>(),	
		_STRETCH_A8A1<0x18>(), _STRETCH_A8A1<0x19>(), _STRETCH_A8A1<0x1A>(), _STRETCH_A8A1<0x1B>(), _STRETCH_A8A1<0x1C>(), _STRETCH_A8A1<0x1D>(), _STRETCH_A8A1<0x1E>(), _STRETCH_A8A1<0x1F>(),
		_STRETCH_A8A1<0x20>(), _STRETCH_A8A1<0x21>(), _STRETCH_A8A1<0x22>(), _STRETCH_A8A1<0x23>(), _STRETCH_A8A1<0x24>(), _STRETCH_A8A1<0x25>(), _STRETCH_A8A1<0x26>(), _STRETCH_A8A1<0x27>(),
		_STRETCH_A8A1<0x28>(), _STRETCH_A8A1<0x29>(), _STRETCH_A8A1<0x2A>(), _STRETCH_A8A1<0x2B>(), _STRETCH_A8A1<0x2C>(), _STRETCH_A8A1<0x2D>(), _STRETCH_A8A1<0x2E>(), _STRETCH_A8A1<0x2F>(),
		_STRETCH_A8A1<0x30>(), _STRETCH_A8A1<0x31>(), _STRETCH_A8A1<0x32>(), _STRETCH_A8A1<0x33>(), _STRETCH_A8A1<0x34>(), _STRETCH_A8A1<0x35>(), _STRETCH_A8A1<0x36>(), _STRETCH_A8A1<0x37>(),
		_STRETCH_A8A1<0x38>(), _STRETCH_A8A1<0x39>(), _STRETCH_A8A1<0x3A>(), _STRETCH_A8A1<0x3B>(), _STRETCH_A8A1<0x3C>(), _STRETCH_A8A1<0x3D>(), _STRETCH_A8A1<0x3E>(), _STRETCH_A8A1<0x3F>()
	};
	const unsigned long long STRETCH_V[64] = {
		_STRETCH_V<0x00>(), _STRETCH_V<0x01>(), _STRETCH_V<0x02>(), _STRETCH_V<0x03>(), _STRETCH_V<0x04>(), _STRETCH_V<0x05>(), _STRETCH_V<0x06>(), _STRETCH_V<0x07>(),	
		_STRETCH_V<0x08>(), _STRETCH_V<0x09>(), _STRETCH_V<0x0A>(), _STRETCH_V<0x0B>(), _STRETCH_V<0x0C>(), _STRETCH_V<0x0D>(), _STRETCH_V<0x0E>(), _STRETCH_V<0x0F>(),
		_STRETCH_V<0x10>(), _STRETCH_V<0x11>(), _STRETCH_V<0x12>(), _STRETCH_V<0x13>(), _STRETCH_V<0x14>(), _STRETCH_V<0x15>(), _STRETCH_V<0x16>(), _STRETCH_V<0x17>(),	
		_STRETCH_V<0x18>(), _STRETCH_V<0x19>(), _STRETCH_V<0x1A>(), _STRETCH_V<0x1B>(), _STRETCH_V<0x1C>(), _STRETCH_V<0x1D>(), _STRETCH_V<0x1E>(), _STRETCH_V<0x1F>(),
		_STRETCH_V<0x20>(), _STRETCH_V<0x21>(), _STRETCH_V<0x22>(), _STRETCH_V<0x23>(), _STRETCH_V<0x24>(), _STRETCH_V<0x25>(), _STRETCH_V<0x26>(), _STRETCH_V<0x27>(),
		_STRETCH_V<0x28>(), _STRETCH_V<0x29>(), _STRETCH_V<0x2A>(), _STRETCH_V<0x2B>(), _STRETCH_V<0x2C>(), _STRETCH_V<0x2D>(), _STRETCH_V<0x2E>(), _STRETCH_V<0x2F>(),
		_STRETCH_V<0x30>(), _STRETCH_V<0x31>(), _STRETCH_V<0x32>(), _STRETCH_V<0x33>(), _STRETCH_V<0x34>(), _STRETCH_V<0x35>(), _STRETCH_V<0x36>(), _STRETCH_V<0x37>(),
		_STRETCH_V<0x38>(), _STRETCH_V<0x39>(), _STRETCH_V<0x3A>(), _STRETCH_V<0x3B>(), _STRETCH_V<0x3C>(), _STRETCH_V<0x3D>(), _STRETCH_V<0x3E>(), _STRETCH_V<0x3F>()
	};
	#endif

inline unsigned long long A1(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 1) & 0x3FULL;
	const unsigned long long P_h = P >> 2;
	const unsigned char outflank_h = OUTFLANK_0[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_0[outflank_h] << 1;

	const unsigned long long O_v = ((O & 0x0001010101010100ULL) * 0x0102040810204080ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 58;
	const unsigned char outflank_v = OUTFLANK_0[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_0[outflank_v]] << 0;

	const unsigned long long O_dc = ((O & 0x0040201008040200ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned char outflank_dc = OUTFLANK_0[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_0[outflank_dc]] & 0x8040201008040201ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long B1(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 2) & 0x1FULL;
	const unsigned long long P_h = P >> 3;
	const unsigned char outflank_h = OUTFLANK_1[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_1[outflank_h] << 1;

	const unsigned long long O_v = ((O & 0x0002020202020200ULL) * 0x0081020408102040ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 58;
	const unsigned char outflank_v = OUTFLANK_0[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_0[outflank_v]] << 1;

	const unsigned long long O_dc = ((O & 0x0000402010080400ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned long long P_dc = ((P & 0x0080402010080502ULL) * 0x0101010101010101ULL) >> 59;
	const unsigned char outflank_dc = OUTFLANK_1[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_1[outflank_dc]] & 0x0080402010080502ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long C1(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 1) & 0x3FULL;
	const unsigned long long P_h = P >> 0;
	const unsigned char outflank_h = OUTFLANK_2[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_2[outflank_h] << 1;

	const unsigned long long O_v = ((O & 0x0004040404040400ULL) * 0x0040810204081020ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 58;
	const unsigned char outflank_v = OUTFLANK_0[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_0[outflank_v]] << 2;

	const unsigned long long O_dc = ((O & 0x0000004020100A00ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0000804020110A04ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_2[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_2[outflank_dc]] & 0x0000804020110A04ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long D1(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 1) & 0x3FULL;
	const unsigned long long P_h = P >> 0;
	const unsigned char outflank_h = OUTFLANK_3[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_3[outflank_h] << 1;

	const unsigned long long O_v = ((O & 0x0008080808080800ULL) * 0x0020408102040810ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 58;
	const unsigned char outflank_v = OUTFLANK_0[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_0[outflank_v]] << 3;

	const unsigned long long O_dc = ((O & 0x0000000040221400ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0000008041221408ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_3[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_3[outflank_dc]] & 0x0000008041221408ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long E1(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 1) & 0x3FULL;
	const unsigned long long P_h = P >> 0;
	const unsigned char outflank_h = OUTFLANK_4[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_4[outflank_h] << 1;

	const unsigned long long O_v = ((O & 0x0010101010101000ULL) * 0x0010204081020408ULL) >> 57;
	const unsigned long long P_v = ((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 58;
	const unsigned char outflank_v = OUTFLANK_0[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_0[outflank_v]] << 4;

	const unsigned long long O_dc = ((O & 0x0000000002442800ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0000000182442810ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_4[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_4[outflank_dc]] & 0x0000000182442810ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long F1(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 1) & 0x3FULL;
	const unsigned long long P_h = P >> 0;
	const unsigned char outflank_h = OUTFLANK_5[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_5[outflank_h] << 1;

	const unsigned long long O_v = ((O & 0x0020202020202000ULL) * 0x0008102040810204ULL) >> 57;
	const unsigned long long P_v = ((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 58;
	const unsigned char outflank_v = OUTFLANK_0[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_0[outflank_v]] << 5;

	const unsigned long long O_dc = ((O & 0x0000000204085000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0000010204885020ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_5[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_5[outflank_dc]] & 0x0000010204885020ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long G1(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 1) & 0x3FULL;
	const unsigned long long P_h = P >> 0;
	const unsigned char outflank_h = OUTFLANK_6[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_6[outflank_h] << 1;

	const unsigned long long O_v = ((O & 0x0040404040404000ULL) * 0x0004081020408102ULL) >> 57;
	const unsigned long long P_v = ((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 58;
	const unsigned char outflank_v = OUTFLANK_0[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_0[outflank_v]] << 6;

	const unsigned long long O_dc = ((O & 0x0000020408102000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x000102040810A040ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_6[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_6[outflank_dc]] & 0x000102040810A040ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long H1(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 1) & 0x3FULL;
	const unsigned long long P_h = P >> 0;
	const unsigned char outflank_h = OUTFLANK_7[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_7[outflank_h] << 1;

	const unsigned long long O_v = ((O & 0x0080808080808000ULL) * 0x0002040810204081ULL) >> 57;
	const unsigned long long P_v = ((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 58;
	const unsigned char outflank_v = OUTFLANK_0[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_0[outflank_v]] << 7;

	const unsigned long long O_dc = ((O & 0x0002040810204000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_7[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_7[outflank_dc]] & 0x0102040810204080ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long A2(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 9) & 0x3FULL;
	const unsigned long long P_h = P >> 10;
	const unsigned char outflank_h = OUTFLANK_0[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_0[outflank_h] << 9;

	const unsigned long long O_v = ((O & 0x0001010101010100ULL) * 0x0102040810204080ULL) >> 58;
	const unsigned long long P_v = ((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 59;
	const unsigned char outflank_v = OUTFLANK_1[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_1[outflank_v]] << 0;

	const unsigned long long O_dc = ((O & 0x0020100804020000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x4020100804020100ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned char outflank_dc = OUTFLANK_0[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_0[outflank_dc]] & 0x4020100804020100ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long B2(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 10) & 0x1FULL;
	const unsigned long long P_h = P >> 11;
	const unsigned char outflank_h = OUTFLANK_1[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_1[outflank_h] << 9;

	const unsigned long long O_v = ((O & 0x0002020202020200ULL) * 0x0081020408102040ULL) >> 58;
	const unsigned long long P_v = ((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 59;
	const unsigned char outflank_v = OUTFLANK_1[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_1[outflank_v]] << 1;

	const unsigned long long O_dc = ((O & 0x0040201008040200ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned long long P_dc = ((P & 0x8040201008050200ULL) * 0x0101010101010101ULL) >> 59;
	const unsigned char outflank_dc = OUTFLANK_1[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_1[outflank_dc]] & 0x8040201008050200ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long C2(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 9) & 0x3FULL;
	const unsigned long long P_h = P >> 8;
	const unsigned char outflank_h = OUTFLANK_2[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_2[outflank_h] << 9;

	const unsigned long long O_v = ((O & 0x0004040404040400ULL) * 0x0040810204081020ULL) >> 58;
	const unsigned long long P_v = ((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 59;
	const unsigned char outflank_v = OUTFLANK_1[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_1[outflank_v]] << 2;

	const unsigned long long O_dc = ((O & 0x00004020100A0400ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x00804020110A0400ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_2[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_2[outflank_dc]] & 0x00804020110A0400ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long D2(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 9) & 0x3FULL;
	const unsigned long long P_h = P >> 8;
	const unsigned char outflank_h = OUTFLANK_3[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_3[outflank_h] << 9;

	const unsigned long long O_v = ((O & 0x0008080808080800ULL) * 0x0020408102040810ULL) >> 58;
	const unsigned long long P_v = ((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 59;
	const unsigned char outflank_v = OUTFLANK_1[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_1[outflank_v]] << 3;

	const unsigned long long O_dc = ((O & 0x0000004022140800ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0000804122140800ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_3[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_3[outflank_dc]] & 0x0000804122140800ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long E2(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 9) & 0x3FULL;
	const unsigned long long P_h = P >> 8;
	const unsigned char outflank_h = OUTFLANK_4[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_4[outflank_h] << 9;

	const unsigned long long O_v = ((O & 0x0010101010101000ULL) * 0x0010204081020408ULL) >> 58;
	const unsigned long long P_v = ((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 59;
	const unsigned char outflank_v = OUTFLANK_1[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_1[outflank_v]] << 4;

	const unsigned long long O_dc = ((O & 0x0000000244281000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0000018244281000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_4[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_4[outflank_dc]] & 0x0000018244281000ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long F2(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 9) & 0x3FULL;
	const unsigned long long P_h = P >> 8;
	const unsigned char outflank_h = OUTFLANK_5[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_5[outflank_h] << 9;

	const unsigned long long O_v = ((O & 0x0020202020202000ULL) * 0x0008102040810204ULL) >> 58;
	const unsigned long long P_v = ((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 59;
	const unsigned char outflank_v = OUTFLANK_1[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_1[outflank_v]] << 5;

	const unsigned long long O_dc = ((O & 0x0000020408502000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0001020488502000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_5[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_5[outflank_dc]] & 0x0001020488502000ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long G2(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 9) & 0x3FULL;
	const unsigned long long P_h = P >> 8;
	const unsigned char outflank_h = OUTFLANK_6[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_6[outflank_h] << 9;

	const unsigned long long O_v = ((O & 0x0040404040404000ULL) * 0x0004081020408102ULL) >> 58;
	const unsigned long long P_v = ((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 59;
	const unsigned char outflank_v = OUTFLANK_1[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_1[outflank_v]] << 6;

	const unsigned long long O_dc = ((O & 0x0002040810204000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0102040810A04000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_6[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_6[outflank_dc]] & 0x0102040810A04000ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long H2(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 9) & 0x3FULL;
	const unsigned long long P_h = P >> 8;
	const unsigned char outflank_h = OUTFLANK_7[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_7[outflank_h] << 9;

	const unsigned long long O_v = ((O & 0x0080808080808000ULL) * 0x0002040810204081ULL) >> 58;
	const unsigned long long P_v = ((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 59;
	const unsigned char outflank_v = OUTFLANK_1[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_1[outflank_v]] << 7;

	const unsigned long long O_dc = ((O & 0x0004081020400000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0204081020408000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_7[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_7[outflank_dc]] & 0x0204081020408000ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long A3(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 17) & 0x3FULL;
	const unsigned long long P_h = P >> 18;
	const unsigned char outflank_h = OUTFLANK_0[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_0[outflank_h] << 17;

	const unsigned long long O_v = ((O & 0x0001010101010100ULL) * 0x0102040810204080ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_2[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_2[outflank_v]] << 0;

	const unsigned long long O_d = ((O & 0x0010080402000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x2010080402010000ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned char outflank_d = OUTFLANK_0[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_0[outflank_d]] & 0x2010080402010000ULL;

	const unsigned long long O_c = ((O & 0x0000000000000200ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0000000000010204ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned char outflank_c = OUTFLANK_0[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_0[outflank_c]] & 0x0000000000010204ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long B3(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 18) & 0x1FULL;
	const unsigned long long P_h = P >> 19;
	const unsigned char outflank_h = OUTFLANK_1[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_1[outflank_h] << 17;

	const unsigned long long O_v = ((O & 0x0002020202020200ULL) * 0x0081020408102040ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_2[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_2[outflank_v]] << 1;

	const unsigned long long O_d = ((O & 0x0020100804020000ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned long long P_d = ((P & 0x4020100804020100ULL) * 0x0101010101010101ULL) >> 59;
	const unsigned char outflank_d = OUTFLANK_1[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_1[outflank_d]] & 0x4020100804020100ULL;

	const unsigned long long O_c = ((O & 0x0000000000020400ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned long long P_c = ((P & 0x0000000001020408ULL) * 0x0101010101010101ULL) >> 59;
	const unsigned char outflank_c = OUTFLANK_1[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_1[outflank_c]] & 0x0000000001020408ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long C3(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 17) & 0x3FULL;
	const unsigned long long P_h = P >> 16;
	const unsigned char outflank_h = OUTFLANK_2[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_2[outflank_h] << 17;

	const unsigned long long O_v = ((O & 0x0004040404040400ULL) * 0x0040810204081020ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_2[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_2[outflank_v]] << 2;

	const unsigned long long O_d = ((O & 0x0040201008040200ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_2[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_2[outflank_d]] & 0x8040201008040201ULL;

	const unsigned long long O_c = ((O & 0x0000000002040800ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0000000102040810ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_2[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_2[outflank_c]] & 0x0000000102040810ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long D3(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 17) & 0x3FULL;
	const unsigned long long P_h = P >> 16;
	const unsigned char outflank_h = OUTFLANK_3[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_3[outflank_h] << 17;

	const unsigned long long O_v = ((O & 0x0008080808080800ULL) * 0x0020408102040810ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_2[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_2[outflank_v]] << 3;

	const unsigned long long O_d = ((O & 0x0000402010080400ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0080402010080402ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_3[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_3[outflank_d]] & 0x0080402010080402ULL;

	const unsigned long long O_c = ((O & 0x0000000204081000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0000010204081020ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_3[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_3[outflank_c]] & 0x0000010204081020ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long E3(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 17) & 0x3FULL;
	const unsigned long long P_h = P >> 16;
	const unsigned char outflank_h = OUTFLANK_4[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_4[outflank_h] << 17;

	const unsigned long long O_v = ((O & 0x0010101010101000ULL) * 0x0010204081020408ULL) >> 57;
	const unsigned long long P_v = ((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_2[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_2[outflank_v]] << 4;

	const unsigned long long O_d = ((O & 0x0000004020100800ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0000804020100804ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_4[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_4[outflank_d]] & 0x0000804020100804ULL;

	const unsigned long long O_c = ((O & 0x0000020408102000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0001020408102040ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_4[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_4[outflank_c]] & 0x0001020408102040ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long F3(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 17) & 0x3FULL;
	const unsigned long long P_h = P >> 16;
	const unsigned char outflank_h = OUTFLANK_5[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_5[outflank_h] << 17;

	const unsigned long long O_v = ((O & 0x0020202020202000ULL) * 0x0008102040810204ULL) >> 57;
	const unsigned long long P_v = ((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_2[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_2[outflank_v]] << 5;

	const unsigned long long O_d = ((O & 0x0000000040201000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0000008040201008ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_5[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_5[outflank_d]] & 0x0000008040201008ULL;

	const unsigned long long O_c = ((O & 0x0002040810204000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_5[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_5[outflank_c]] & 0x0102040810204080ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long G3(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 17) & 0x3FULL;
	const unsigned long long P_h = P >> 16;
	const unsigned char outflank_h = OUTFLANK_6[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_6[outflank_h] << 17;

	const unsigned long long O_v = ((O & 0x0040404040404000ULL) * 0x0004081020408102ULL) >> 57;
	const unsigned long long P_v = ((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_2[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_2[outflank_v]] << 6;

	const unsigned long long O_d = ((O & 0x0000000000402000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0000000080402010ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_6[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_6[outflank_d]] & 0x0000000080402010ULL;

	const unsigned long long O_c = ((O & 0x0004081020400000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0204081020408000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_6[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_6[outflank_c]] & 0x0204081020408000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long H3(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 17) & 0x3FULL;
	const unsigned long long P_h = P >> 16;
	const unsigned char outflank_h = OUTFLANK_7[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_7[outflank_h] << 17;

	const unsigned long long O_v = ((O & 0x0080808080808000ULL) * 0x0002040810204081ULL) >> 57;
	const unsigned long long P_v = ((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_2[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_2[outflank_v]] << 7;

	const unsigned long long O_d = ((O & 0x0000000000004000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0000000000804020ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_7[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_7[outflank_d]] & 0x0000000000804020ULL;

	const unsigned long long O_c = ((O & 0x0008102040000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0408102040800000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_7[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_7[outflank_c]] & 0x0408102040800000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long A4(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 25) & 0x3FULL;
	const unsigned long long P_h = P >> 26;
	const unsigned char outflank_h = OUTFLANK_0[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_0[outflank_h] << 25;

	const unsigned long long O_v = ((O & 0x0001010101010100ULL) * 0x0102040810204080ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_3[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_3[outflank_v]] << 0;

	const unsigned long long O_d = ((O & 0x0008040200000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x1008040201000000ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned char outflank_d = OUTFLANK_0[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_0[outflank_d]] & 0x1008040201000000ULL;

	const unsigned long long O_c = ((O & 0x0000000000020400ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0000000001020408ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned char outflank_c = OUTFLANK_0[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_0[outflank_c]] & 0x0000000001020408ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long B4(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 26) & 0x1FULL;
	const unsigned long long P_h = P >> 27;
	const unsigned char outflank_h = OUTFLANK_1[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_1[outflank_h] << 25;

	const unsigned long long O_v = ((O & 0x0002020202020200ULL) * 0x0081020408102040ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_3[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_3[outflank_v]] << 1;

	const unsigned long long O_d = ((O & 0x0010080402000000ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned long long P_d = ((P & 0x2010080402010000ULL) * 0x0101010101010101ULL) >> 59;
	const unsigned char outflank_d = OUTFLANK_1[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_1[outflank_d]] & 0x2010080402010000ULL;

	const unsigned long long O_c = ((O & 0x0000000002040800ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned long long P_c = ((P & 0x0000000102040810ULL) * 0x0101010101010101ULL) >> 59;
	const unsigned char outflank_c = OUTFLANK_1[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_1[outflank_c]] & 0x0000000102040810ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long C4(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 25) & 0x3FULL;
	const unsigned long long P_h = P >> 24;
	const unsigned char outflank_h = OUTFLANK_2[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_2[outflank_h] << 25;

	const unsigned long long O_v = ((O & 0x0004040404040400ULL) * 0x0040810204081020ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_3[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_3[outflank_v]] << 2;

	const unsigned long long O_d = ((O & 0x0020100804020000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x4020100804020100ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_2[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_2[outflank_d]] & 0x4020100804020100ULL;

	const unsigned long long O_c = ((O & 0x0000000204081000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0000010204081020ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_2[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_2[outflank_c]] & 0x0000010204081020ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long D4(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 25) & 0x3FULL;
	const unsigned long long P_h = P >> 24;
	const unsigned char outflank_h = OUTFLANK_3[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_3[outflank_h] << 25;

	const unsigned long long O_v = ((O & 0x0008080808080800ULL) * 0x0020408102040810ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_3[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_3[outflank_v]] << 3;

	const unsigned long long O_d = ((O & 0x0040201008040200ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_3[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_3[outflank_d]] & 0x8040201008040201ULL;

	const unsigned long long O_c = ((O & 0x0000020408102000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0001020408102040ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_3[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_3[outflank_c]] & 0x0001020408102040ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long E4(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 25) & 0x3FULL;
	const unsigned long long P_h = P >> 24;
	const unsigned char outflank_h = OUTFLANK_4[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_4[outflank_h] << 25;

	const unsigned long long O_v = ((O & 0x0010101010101000ULL) * 0x0010204081020408ULL) >> 57;
	const unsigned long long P_v = ((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_3[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_3[outflank_v]] << 4;

	const unsigned long long O_d = ((O & 0x0000402010080400ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0080402010080402ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_4[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_4[outflank_d]] & 0x0080402010080402ULL;

	const unsigned long long O_c = ((O & 0x0002040810204000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_4[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_4[outflank_c]] & 0x0102040810204080ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long F4(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 25) & 0x3FULL;
	const unsigned long long P_h = P >> 24;
	const unsigned char outflank_h = OUTFLANK_5[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_5[outflank_h] << 25;

	const unsigned long long O_v = ((O & 0x0020202020202000ULL) * 0x0008102040810204ULL) >> 57;
	const unsigned long long P_v = ((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_3[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_3[outflank_v]] << 5;

	const unsigned long long O_d = ((O & 0x0000004020100800ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0000804020100804ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_5[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_5[outflank_d]] & 0x0000804020100804ULL;

	const unsigned long long O_c = ((O & 0x0004081020400000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0204081020408000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_5[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_5[outflank_c]] & 0x0204081020408000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long G4(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 25) & 0x3FULL;
	const unsigned long long P_h = P >> 24;
	const unsigned char outflank_h = OUTFLANK_6[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_6[outflank_h] << 25;

	const unsigned long long O_v = ((O & 0x0040404040404000ULL) * 0x0004081020408102ULL) >> 57;
	const unsigned long long P_v = ((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_3[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_3[outflank_v]] << 6;

	const unsigned long long O_d = ((O & 0x0000000040201000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0000008040201008ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_6[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_6[outflank_d]] & 0x0000008040201008ULL;

	const unsigned long long O_c = ((O & 0x0008102040000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0408102040800000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_6[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_6[outflank_c]] & 0x0408102040800000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long H4(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 25) & 0x3FULL;
	const unsigned long long P_h = P >> 24;
	const unsigned char outflank_h = OUTFLANK_7[O_h] & P_h;
	const unsigned long long flips_h = FLIPS_7[outflank_h] << 25;

	const unsigned long long O_v = ((O & 0x0080808080808000ULL) * 0x0002040810204081ULL) >> 57;
	const unsigned long long P_v = ((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_3[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_3[outflank_v]] << 7;

	const unsigned long long O_d = ((O & 0x0000000000402000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0000000080402010ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_7[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_7[outflank_d]] & 0x0000000080402010ULL;

	const unsigned long long O_c = ((O & 0x0010204000000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0810204080000000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_7[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_7[outflank_c]] & 0x0810204080000000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long A5(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 33) & 0x3FULL;
	const unsigned long long P_h = P >> 34;
	const unsigned char outflank_h = OUTFLANK_0[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_0[outflank_h]) << 33;

	const unsigned long long O_v = ((O & 0x0001010101010100ULL) * 0x0102040810204080ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_4[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_4[outflank_v]] << 0;

	const unsigned long long O_d = ((O & 0x0004020000000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0804020100000000ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned char outflank_d = OUTFLANK_0[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_0[outflank_d]] & 0x0804020100000000ULL;

	const unsigned long long O_c = ((O & 0x0000000002040800ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0000000102040810ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned char outflank_c = OUTFLANK_0[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_0[outflank_c]] & 0x0000000102040810ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long B5(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 34) & 0x1FULL;
	const unsigned long long P_h = P >> 35;
	const unsigned char outflank_h = OUTFLANK_1[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_1[outflank_h]) << 33;

	const unsigned long long O_v = ((O & 0x0002020202020200ULL) * 0x0081020408102040ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_4[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_4[outflank_v]] << 1;

	const unsigned long long O_d = ((O & 0x0008040200000000ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned long long P_d = ((P & 0x1008040201000000ULL) * 0x0101010101010101ULL) >> 59;
	const unsigned char outflank_d = OUTFLANK_1[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_1[outflank_d]] & 0x1008040201000000ULL;

	const unsigned long long O_c = ((O & 0x0000000204081000ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned long long P_c = ((P & 0x0000010204081020ULL) * 0x0101010101010101ULL) >> 59;
	const unsigned char outflank_c = OUTFLANK_1[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_1[outflank_c]] & 0x0000010204081020ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long C5(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 33) & 0x3FULL;
	const unsigned long long P_h = P >> 32;
	const unsigned char outflank_h = OUTFLANK_2[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_2[outflank_h]) << 33;

	const unsigned long long O_v = ((O & 0x0004040404040400ULL) * 0x0040810204081020ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_4[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_4[outflank_v]] << 2;

	const unsigned long long O_d = ((O & 0x0010080402000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x2010080402010000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_2[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_2[outflank_d]] & 0x2010080402010000ULL;

	const unsigned long long O_c = ((O & 0x0000020408102000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0001020408102040ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_2[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_2[outflank_c]] & 0x0001020408102040ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long D5(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 33) & 0x3FULL;
	const unsigned long long P_h = P >> 32;
	const unsigned char outflank_h = OUTFLANK_3[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_3[outflank_h]) << 33;

	const unsigned long long O_v = ((O & 0x0008080808080800ULL) * 0x0020408102040810ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_4[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_4[outflank_v]] << 3;

	const unsigned long long O_d = ((O & 0x0020100804020000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x4020100804020100ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_3[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_3[outflank_d]] & 0x4020100804020100ULL;

	const unsigned long long O_c = ((O & 0x0002040810204000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_3[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_3[outflank_c]] & 0x0102040810204080ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long E5(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 33) & 0x3FULL;
	const unsigned long long P_h = P >> 32;
	const unsigned char outflank_h = OUTFLANK_4[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_4[outflank_h]) << 33;

	const unsigned long long O_v = ((O & 0x0010101010101000ULL) * 0x0010204081020408ULL) >> 57;
	const unsigned long long P_v = ((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_4[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_4[outflank_v]] << 4;

	const unsigned long long O_d = ((O & 0x0040201008040200ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_4[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_4[outflank_d]] & 0x8040201008040201ULL;

	const unsigned long long O_c = ((O & 0x0004081020400000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0204081020408000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_4[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_4[outflank_c]] & 0x0204081020408000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long F5(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 33) & 0x3FULL;
	const unsigned long long P_h = P >> 32;
	const unsigned char outflank_h = OUTFLANK_5[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_5[outflank_h]) << 33;

	const unsigned long long O_v = ((O & 0x0020202020202000ULL) * 0x0008102040810204ULL) >> 57;
	const unsigned long long P_v = ((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_4[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_4[outflank_v]] << 5;

	const unsigned long long O_d = ((O & 0x0000402010080400ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0080402010080402ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_5[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_5[outflank_d]] & 0x0080402010080402ULL;

	const unsigned long long O_c = ((O & 0x0008102040000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0408102040800000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_5[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_5[outflank_c]] & 0x0408102040800000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long G5(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 33) & 0x3FULL;
	const unsigned long long P_h = P >> 32;
	const unsigned char outflank_h = OUTFLANK_6[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_6[outflank_h]) << 33;

	const unsigned long long O_v = ((O & 0x0040404040404000ULL) * 0x0004081020408102ULL) >> 57;
	const unsigned long long P_v = ((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_4[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_4[outflank_v]] << 6;

	const unsigned long long O_d = ((O & 0x0000004020100800ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0000804020100804ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_6[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_6[outflank_d]] & 0x0000804020100804ULL;

	const unsigned long long O_c = ((O & 0x0010204000000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0810204080000000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_6[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_6[outflank_c]] & 0x0810204080000000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long H5(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 33) & 0x3FULL;
	const unsigned long long P_h = P >> 32;
	const unsigned char outflank_h = OUTFLANK_7[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_7[outflank_h]) << 33;

	const unsigned long long O_v = ((O & 0x0080808080808000ULL) * 0x0002040810204081ULL) >> 57;
	const unsigned long long P_v = ((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_4[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_4[outflank_v]] << 7;

	const unsigned long long O_d = ((O & 0x0000000040201000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0000008040201008ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_7[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_7[outflank_d]] & 0x0000008040201008ULL;

	const unsigned long long O_c = ((O & 0x0020400000000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x1020408000000000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_7[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_7[outflank_c]] & 0x1020408000000000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long A6(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 41) & 0x3FULL;
	const unsigned long long P_h = P >> 42;
	const unsigned char outflank_h = OUTFLANK_0[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_0[outflank_h]) << 41;

	const unsigned long long O_v = ((O & 0x0001010101010100ULL) * 0x0102040810204080ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_5[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_5[outflank_v]] << 0;

	const unsigned long long O_d = ((O & 0x0002000000000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0402010000000000ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned char outflank_d = OUTFLANK_0[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_0[outflank_d]] & 0x0402010000000000ULL;

	const unsigned long long O_c = ((O & 0x0000000204081000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0000010204081020ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned char outflank_c = OUTFLANK_0[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_0[outflank_c]] & 0x0000010204081020ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long B6(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 42) & 0x1FULL;
	const unsigned long long P_h = P >> 43;
	const unsigned char outflank_h = OUTFLANK_1[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_1[outflank_h]) << 41;

	const unsigned long long O_v = ((O & 0x0002020202020200ULL) * 0x0081020408102040ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_5[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_5[outflank_v]] << 1;

	const unsigned long long O_d = ((O & 0x0004020000000000ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned long long P_d = ((P & 0x0804020100000000ULL) * 0x0101010101010101ULL) >> 59;
	const unsigned char outflank_d = OUTFLANK_1[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_1[outflank_d]] & 0x0804020100000000ULL;

	const unsigned long long O_c = ((O & 0x0000020408102000ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned long long P_c = ((P & 0x0001020408102040ULL) * 0x0101010101010101ULL) >> 59;
	const unsigned char outflank_c = OUTFLANK_1[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_1[outflank_c]] & 0x0001020408102040ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long C6(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 41) & 0x3FULL;
	const unsigned long long P_h = P >> 40;
	const unsigned char outflank_h = OUTFLANK_2[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_2[outflank_h]) << 41;

	const unsigned long long O_v = ((O & 0x0004040404040400ULL) * 0x0040810204081020ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_5[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_5[outflank_v]] << 2;

	const unsigned long long O_d = ((O & 0x0008040200000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x1008040201000000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_2[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_2[outflank_d]] & 0x1008040201000000ULL;

	const unsigned long long O_c = ((O & 0x0002040810204000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_2[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_2[outflank_c]] & 0x0102040810204080ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long D6(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 41) & 0x3FULL;
	const unsigned long long P_h = P >> 40;
	const unsigned char outflank_h = OUTFLANK_3[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_3[outflank_h]) << 41;

	const unsigned long long O_v = ((O & 0x0008080808080800ULL) * 0x0020408102040810ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_5[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_5[outflank_v]] << 3;

	const unsigned long long O_d = ((O & 0x0010080402000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x2010080402010000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_3[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_3[outflank_d]] & 0x2010080402010000ULL;

	const unsigned long long O_c = ((O & 0x0004081020400000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0204081020408000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_3[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_3[outflank_c]] & 0x0204081020408000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long E6(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 41) & 0x3FULL;
	const unsigned long long P_h = P >> 40;
	const unsigned char outflank_h = OUTFLANK_4[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_4[outflank_h]) << 41;

	const unsigned long long O_v = ((O & 0x0010101010101000ULL) * 0x0010204081020408ULL) >> 57;
	const unsigned long long P_v = ((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_5[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_5[outflank_v]] << 4;

	const unsigned long long O_d = ((O & 0x0020100804020000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x4020100804020100ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_4[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_4[outflank_d]] & 0x4020100804020100ULL;

	const unsigned long long O_c = ((O & 0x0008102040000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0408102040800000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_4[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_4[outflank_c]] & 0x0408102040800000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long F6(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 41) & 0x3FULL;
	const unsigned long long P_h = P >> 40;
	const unsigned char outflank_h = OUTFLANK_5[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_5[outflank_h]) << 41;

	const unsigned long long O_v = ((O & 0x0020202020202000ULL) * 0x0008102040810204ULL) >> 57;
	const unsigned long long P_v = ((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_5[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_5[outflank_v]] << 5;

	const unsigned long long O_d = ((O & 0x0040201008040200ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_5[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_5[outflank_d]] & 0x8040201008040201ULL;

	const unsigned long long O_c = ((O & 0x0010204000000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x0810204080000000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_5[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_5[outflank_c]] & 0x0810204080000000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long G6(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 41) & 0x3FULL;
	const unsigned long long P_h = P >> 40;
	const unsigned char outflank_h = OUTFLANK_6[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_6[outflank_h]) << 41;

	const unsigned long long O_v = ((O & 0x0040404040404000ULL) * 0x0004081020408102ULL) >> 57;
	const unsigned long long P_v = ((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_5[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_5[outflank_v]] << 6;

	const unsigned long long O_d = ((O & 0x0000402010080400ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0080402010080402ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_6[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_6[outflank_d]] & 0x0080402010080402ULL;

	const unsigned long long O_c = ((O & 0x0020400000000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x1020408000000000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_6[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_6[outflank_c]] & 0x1020408000000000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long H6(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 41) & 0x3FULL;
	const unsigned long long P_h = P >> 40;
	const unsigned char outflank_h = OUTFLANK_7[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_7[outflank_h]) << 41;

	const unsigned long long O_v = ((O & 0x0080808080808000ULL) * 0x0002040810204081ULL) >> 57;
	const unsigned long long P_v = ((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_5[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_5[outflank_v]] << 7;

	const unsigned long long O_d = ((O & 0x0000004020100800ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_d = ((P & 0x0000804020100804ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_d = OUTFLANK_7[O_d] & P_d;
	const unsigned long long flips_d = STRETCH_V[FLIPS_7[outflank_d]] & 0x0000804020100804ULL;

	const unsigned long long O_c = ((O & 0x0040000000000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_c = ((P & 0x2040800000000000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_c = OUTFLANK_7[O_c] & P_c;
	const unsigned long long flips_c = STRETCH_V[FLIPS_7[outflank_c]] & 0x2040800000000000ULL;

	return flips_h | flips_v | flips_d | flips_c;
}
inline unsigned long long A7(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 49) & 0x3FULL;
	const unsigned long long P_h = P >> 50;
	const unsigned char outflank_h = OUTFLANK_0[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_0[outflank_h]) << 49;

	const unsigned long long O_v = ((O & 0x0001010101010100ULL) * 0x0102040810204080ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_6[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_6[outflank_v]] << 0;

	const unsigned long long O_dc = ((O & 0x0000020408102000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0001020408102040ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned char outflank_dc = OUTFLANK_0[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_0[outflank_dc]] & 0x0001020408102040ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long B7(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 50) & 0x1FULL;
	const unsigned long long P_h = P >> 51;
	const unsigned char outflank_h = OUTFLANK_1[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_1[outflank_h]) << 49;

	const unsigned long long O_v = ((O & 0x0002020202020200ULL) * 0x0081020408102040ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_6[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_6[outflank_v]] << 1;

	const unsigned long long O_dc = ((O & 0x0002040810204000ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned long long P_dc = ((P & 0x0002050810204080ULL) * 0x0101010101010101ULL) >> 59;
	const unsigned char outflank_dc = OUTFLANK_1[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_1[outflank_dc]] & 0x0002050810204080ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long C7(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 49) & 0x3FULL;
	const unsigned long long P_h = P >> 48;
	const unsigned char outflank_h = OUTFLANK_2[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_2[outflank_h]) << 49;

	const unsigned long long O_v = ((O & 0x0004040404040400ULL) * 0x0040810204081020ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_6[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_6[outflank_v]] << 2;

	const unsigned long long O_dc = ((O & 0x00040A1020400000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x00040A1120408000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_2[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_2[outflank_dc]] & 0x00040A1120408000ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long D7(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 49) & 0x3FULL;
	const unsigned long long P_h = P >> 48;
	const unsigned char outflank_h = OUTFLANK_3[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_3[outflank_h]) << 49;

	const unsigned long long O_v = ((O & 0x0008080808080800ULL) * 0x0020408102040810ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_6[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_6[outflank_v]] << 3;

	const unsigned long long O_dc = ((O & 0x0008142240000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0008142241800000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_3[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_3[outflank_dc]] & 0x0008142241800000ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long E7(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 49) & 0x3FULL;
	const unsigned long long P_h = P >> 48;
	const unsigned char outflank_h = OUTFLANK_4[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_4[outflank_h]) << 49;

	const unsigned long long O_v = ((O & 0x0010101010101000ULL) * 0x0010204081020408ULL) >> 57;
	const unsigned long long P_v = ((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_6[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_6[outflank_v]] << 4;

	const unsigned long long O_dc = ((O & 0x0010284402000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0010284482010000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_4[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_4[outflank_dc]] & 0x0010284482010000ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long F7(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 49) & 0x3FULL;
	const unsigned long long P_h = P >> 48;
	const unsigned char outflank_h = OUTFLANK_5[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_5[outflank_h]) << 49;

	const unsigned long long O_v = ((O & 0x0020202020202000ULL) * 0x0008102040810204ULL) >> 57;
	const unsigned long long P_v = ((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_6[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_6[outflank_v]] << 5;

	const unsigned long long O_dc = ((O & 0x0020500804020000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0020508804020100ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_5[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_5[outflank_dc]] & 0x0020508804020100ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long G7(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 49) & 0x3FULL;
	const unsigned long long P_h = P >> 48;
	const unsigned char outflank_h = OUTFLANK_6[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_6[outflank_h]) << 49;

	const unsigned long long O_v = ((O & 0x0040404040404000ULL) * 0x0004081020408102ULL) >> 57;
	const unsigned long long P_v = ((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_6[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_6[outflank_v]] << 6;

	const unsigned long long O_dc = ((O & 0x0040201008040200ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0040A01008040201ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_6[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_6[outflank_dc]] & 0x0040A01008040201ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long H7(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 49) & 0x3FULL;
	const unsigned long long P_h = P >> 48;
	const unsigned char outflank_h = OUTFLANK_7[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_7[outflank_h]) << 49;

	const unsigned long long O_v = ((O & 0x0080808080808000ULL) * 0x0002040810204081ULL) >> 57;
	const unsigned long long P_v = ((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_6[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_6[outflank_v]] << 7;

	const unsigned long long O_dc = ((O & 0x0000402010080400ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0080402010080402ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_7[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_7[outflank_dc]] & 0x0080402010080402ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long A8(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 57) & 0x3FULL;
	const unsigned long long P_h = P >> 58;
	const unsigned char outflank_h = OUTFLANK_0[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_0[outflank_h]) << 57;

	const unsigned long long O_v = ((O & 0x0001010101010100ULL) * 0x0102040810204080ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_7[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_7[outflank_v]] << 0;

	const unsigned long long O_dc = ((O & 0x0002040810204000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned char outflank_dc = OUTFLANK_0[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_0[outflank_dc]] & 0x0102040810204080ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long B8(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 58) & 0x1FULL;
	const unsigned long long P_h = P >> 59;
	const unsigned char outflank_h = OUTFLANK_1[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_1[outflank_h]) << 57;

	const unsigned long long O_v = ((O & 0x0002020202020200ULL) * 0x0081020408102040ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_7[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_7[outflank_v]] << 1;

	const unsigned long long O_dc = ((O & 0x0004081020400000ULL) * 0x0101010101010101ULL) >> 58;
	const unsigned long long P_dc = ((P & 0x0205081020408000ULL) * 0x0101010101010101ULL) >> 59;
	const unsigned char outflank_dc = OUTFLANK_1[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_1[outflank_dc]] & 0x0205081020408000ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long C8(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 57) & 0x3FULL;
	const unsigned long long P_h = P >> 56;
	const unsigned char outflank_h = OUTFLANK_2[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_2[outflank_h]) << 57;

	const unsigned long long O_v = ((O & 0x0004040404040400ULL) * 0x0040810204081020ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_7[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_7[outflank_v]] << 2;

	const unsigned long long O_dc = ((O & 0x000A102040000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x040A112040800000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_2[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_2[outflank_dc]] & 0x040A112040800000ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long D8(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 57) & 0x3FULL;
	const unsigned long long P_h = P >> 56;
	const unsigned char outflank_h = OUTFLANK_3[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_3[outflank_h]) << 57;

	const unsigned long long O_v = ((O & 0x0008080808080800ULL) * 0x0020408102040810ULL) >> 57;
	const unsigned long long P_v = ((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_7[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_7[outflank_v]] << 3;

	const unsigned long long O_dc = ((O & 0x0014224000000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x0814224180000000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_3[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_3[outflank_dc]] & 0x0814224180000000ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long E8(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 57) & 0x3FULL;
	const unsigned long long P_h = P >> 56;
	const unsigned char outflank_h = OUTFLANK_4[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_4[outflank_h]) << 57;

	const unsigned long long O_v = ((O & 0x0010101010101000ULL) * 0x0010204081020408ULL) >> 57;
	const unsigned long long P_v = ((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_7[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_7[outflank_v]] << 4;

	const unsigned long long O_dc = ((O & 0x0028440200000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x1028448201000000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_4[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_4[outflank_dc]] & 0x1028448201000000ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long F8(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 57) & 0x3FULL;
	const unsigned long long P_h = P >> 56;
	const unsigned char outflank_h = OUTFLANK_5[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_5[outflank_h]) << 57;

	const unsigned long long O_v = ((O & 0x0020202020202000ULL) * 0x0008102040810204ULL) >> 57;
	const unsigned long long P_v = ((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_7[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_7[outflank_v]] << 5;

	const unsigned long long O_dc = ((O & 0x0050080402000000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x2050880402010000ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_5[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_5[outflank_dc]] & 0x2050880402010000ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long G8(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 57) & 0x3FULL;
	const unsigned long long P_h = P >> 56;
	const unsigned char outflank_h = OUTFLANK_6[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_6[outflank_h]) << 57;

	const unsigned long long O_v = ((O & 0x0040404040404000ULL) * 0x0004081020408102ULL) >> 57;
	const unsigned long long P_v = ((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_7[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_7[outflank_v]] << 6;

	const unsigned long long O_dc = ((O & 0x0020100804020000ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x40A0100804020100ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_6[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_6[outflank_dc]] & 0x40A0100804020100ULL;

	return flips_h | flips_v | flips_dc;
}
inline unsigned long long H8(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long O_h = (O >> 57) & 0x3FULL;
	const unsigned long long P_h = P >> 56;
	const unsigned char outflank_h = OUTFLANK_7[O_h] & P_h;
	const unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_7[outflank_h]) << 57;

	const unsigned long long O_v = ((O & 0x0080808080808000ULL) * 0x0002040810204081ULL) >> 57;
	const unsigned long long P_v = ((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56;
	const unsigned char outflank_v = OUTFLANK_7[O_v] & P_v;
	const unsigned long long flips_v = STRETCH_A1A8[FLIPS_7[outflank_v]] << 7;

	const unsigned long long O_dc = ((O & 0x0040201008040200ULL) * 0x0101010101010101ULL) >> 57;
	const unsigned long long P_dc = ((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56;
	const unsigned char outflank_dc = OUTFLANK_7[O_dc] & P_dc;
	const unsigned long long flips_dc = STRETCH_V[FLIPS_7[outflank_dc]] & 0x8040201008040201ULL;

	return flips_h | flips_v | flips_dc;
}

	unsigned long long (*flip_fp[])(const unsigned long long, const unsigned long long) = {
		//A1, B1, C1, D1, E1, F1, G1, H1,
		//A2, B2, C2, D2, E2, F2, G2, H2,
		//A3, B3, C3, D3, E3, F3, G3, H3,
		//A4, B4, C4, D4, E4, F4, G4, H4,
		//A5, B5, C5, D5, E5, F5, G5, H5,
		//A6, B6, C6, D6, E6, F6, G6, H6,
		//A7, B7, C7, D7, E7, F7, G7, H7,
		//A8, B8, C8, D8, E8, F8, G8, H8
		ZZ<0,0>, ZZ<1,0>, ZZ<2,0>, ZZ<3,0>, ZZ<4,0>, ZZ<5,0>, ZZ<6,0>, ZZ<7,0>,
		ZZ<0,1>, ZZ<1,1>, ZZ<2,1>, ZZ<3,1>, ZZ<4,1>, ZZ<5,1>, ZZ<6,1>, ZZ<7,1>,
		AA<0,2>, BB<1,2>, XX<2,2>, XX<3,2>, XX<4,2>, XX<5,2>, GH<6,2>, GH<7,2>,
		AA<0,3>, BB<1,3>, XX<2,3>, XX<3,3>, XX<4,3>, XX<5,3>, GH<6,3>, GH<7,3>,
		AA<0,4>, BB<1,4>, XX<2,4>, XX<3,4>, XX<4,4>, XX<5,4>, GH<6,4>, GH<7,4>,
		AA<0,5>, BB<1,5>, XX<2,5>, XX<3,5>, XX<4,5>, XX<5,5>, GH<6,5>, GH<7,5>,
		ZZ<0,6>, ZZ<1,6>, ZZ<2,6>, ZZ<3,6>, ZZ<4,6>, ZZ<5,6>, ZZ<6,6>, ZZ<7,6>,
		ZZ<0,7>, ZZ<1,7>, ZZ<2,7>, ZZ<3,7>, ZZ<4,7>, ZZ<5,7>, ZZ<6,7>, ZZ<7,7>
	};

	void PrintArrays()
	{
		printf("const unsigned char OUTFLANK_0[64] = {\n");
		for (int i = 0; i < 64; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", OUTFLANK_0[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");
	
		printf("const unsigned char OUTFLANK_1[64] = {\n");
		for (int i = 0; i < 64; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", OUTFLANK_1[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");
	
		printf("const unsigned char OUTFLANK_2[64] = {\n");
		for (int i = 0; i < 64; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", OUTFLANK_2[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");
	
		printf("const unsigned char OUTFLANK_3[64] = {\n");
		for (int i = 0; i < 64; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", OUTFLANK_3[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");
	
		printf("const unsigned char OUTFLANK_4[64] = {\n");
		for (int i = 0; i < 64; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", OUTFLANK_4[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");
	
		printf("const unsigned char OUTFLANK_5[64] = {\n");
		for (int i = 0; i < 64; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", OUTFLANK_5[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");
	
		printf("const unsigned char OUTFLANK_6[64] = {\n");
		for (int i = 0; i < 64; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", OUTFLANK_6[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");
	
		printf("const unsigned char OUTFLANK_7[64] = {\n");
		for (int i = 0; i < 64; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", OUTFLANK_7[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");


		printf("\n");


		printf("const unsigned char FLIPS_0[33] = {\n");
		for (int i = 0; i < 33; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", FLIPS_0[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");

		printf("const unsigned char FLIPS_1[17] = {\n");
		for (int i = 0; i < 17; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", FLIPS_1[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");

		printf("const unsigned char FLIPS_2[130] = {\n");
		for (int i = 0; i < 130; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", FLIPS_2[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");

		printf("const unsigned char FLIPS_3[131] = {\n");
		for (int i = 0; i < 131; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", FLIPS_3[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");

		printf("const unsigned char FLIPS_4[133] = {\n");
		for (int i = 0; i < 133; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", FLIPS_4[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");

		printf("const unsigned char FLIPS_5[137] = {\n");
		for (int i = 0; i < 137; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", FLIPS_5[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");

		printf("const unsigned char FLIPS_6[17] = {\n");
		for (int i = 0; i < 17; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", FLIPS_6[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");

		printf("const unsigned char FLIPS_7[33] = {\n");
		for (int i = 0; i < 33; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%02X, ", FLIPS_7[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");

#ifndef SAVE_MEMORY
		printf("\n");
	

		printf("const unsigned long long STRETCH_A1A8[64] = {\n");
		for (int i = 0; i < 64; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%016llX, ", STRETCH_A1A8[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");

		printf("const unsigned long long STRETCH_V[i] = {\n");
		for (int i = 0; i < 64; ++i)
		{
			if (i % 8 == 0)
				printf("\t");
			printf("0x%016llX, ", STRETCH_V[i]);
			if (i % 8 == 7)
				printf("\n");
		}
		printf("};\n");
#endif
	}
}