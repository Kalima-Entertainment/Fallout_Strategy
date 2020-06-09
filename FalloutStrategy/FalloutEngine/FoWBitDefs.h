#ifndef __FOW_BIT_DEFS__
#define __FOW_BIT_DEFS__

// You don't have to do anything in here! If you are interested in how we store each tile fog state read below, otherwise feel free to go to the next TODO

//Imagine that we divide the tile into 9 subtiles (like in TIC TAC TOE). These first 9 defines store a value for each one of the subtiles
//If we want a certain shape we just add the numbers of the subtiles involved
//Example: If we want to store a diagonal line we can get the subtile on the top left corner, the one on the center and the one on the bottom right and add their values to get a unique value "ID" for that combination
//That is what we do below, we define every shape we want

#define FOW_BIT_NW  (1 << 0)//this is equal to 1* 2^0 
#define FOW_BIT_N   (1 << 1)//this is equal to 1* 2^1
#define FOW_BIT_NE  (1 << 2)//this is equal to 1* 2^2
#define FOW_BIT_W   (1 << 3)//this is equal to 1* 2^3
#define FOW_BIT_C   (1 << 4)//this is equal to 1* 2^4
#define FOW_BIT_E   (1 << 5)//this is equal to 1* 2^5
#define FOW_BIT_SW  (1 << 6)//this is equal to 1* 2^6
#define FOW_BIT_S   (1 << 7)//this is equal to 1* 2^7
#define FOW_BIT_SE  (1 << 8)//this is equal to 1* 2^8

#define fow_NON         0
#define fow_ALL         (FOW_BIT_NW | FOW_BIT_N | FOW_BIT_NE |FOW_BIT_W | FOW_BIT_C | FOW_BIT_E |FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE)

#define NUM_FOW_ENTRIES fow_ALL

//Straight lines
#define fow_EEE         (FOW_BIT_SE | FOW_BIT_E | FOW_BIT_NE)
#define fow_NNN         (FOW_BIT_NE | FOW_BIT_N | FOW_BIT_NW)
#define fow_WWW         (FOW_BIT_NW | FOW_BIT_W | FOW_BIT_SW)
#define fow_SSS         (FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE)

//Corners
#define fow_CNE         (FOW_BIT_E | FOW_BIT_NE | FOW_BIT_N |FOW_BIT_NW | FOW_BIT_C | FOW_BIT_SE)
#define fow_CNW         (FOW_BIT_N | FOW_BIT_NW | FOW_BIT_W |FOW_BIT_SW | FOW_BIT_C | FOW_BIT_NE)
#define fow_CSW         (FOW_BIT_W | FOW_BIT_SW | FOW_BIT_S |FOW_BIT_NW | FOW_BIT_C | FOW_BIT_SE)
#define fow_CSE         (FOW_BIT_S | FOW_BIT_SE | FOW_BIT_E |FOW_BIT_NE | FOW_BIT_C | FOW_BIT_SW)

//Inner Corners (joints)
#define fow_JNE         (FOW_BIT_E | FOW_BIT_NE | FOW_BIT_N)
#define fow_JNW         (FOW_BIT_N | FOW_BIT_NW | FOW_BIT_W)
#define fow_JSW         (FOW_BIT_W | FOW_BIT_SW | FOW_BIT_S)
#define fow_JSE         (FOW_BIT_S | FOW_BIT_SE | FOW_BIT_E)

// max and min radii of the circle mask
#define fow_MIN_CIRCLE_RADIUS 2
#define fow_MAX_CIRCLE_RADIUS 7
#define fow_MAX_CIRCLE_LENGTH ((fow_MAX_CIRCLE_RADIUS*2)+1)

#endif // !__FOW_BIT_DEFS__