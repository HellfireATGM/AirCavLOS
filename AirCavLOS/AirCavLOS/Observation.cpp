#include "StdAfx.h"
#include "Observation.h"


int	Standard_Optics_Clear[OBS_MAX][TER_MAX_2] =
{
//  c   b   d   w   t   l    h    c   b   d   w   t   l   h 
//  l   r   e   o   o   t    v    l   r   e   o   o   t   v  
//  e   o   f   o   w        y    e   o   f   o   w       y 
//  a   k   i   d   n   l         a   k   i   d   n   l      
//  r   e   l   s       o    l    r   e   l   s       o   l  
//      n   a           w    o        n   a           w   o
//          d                w            d               w
//          e                             e
//----------- DAY ------------    ----------- NIGHT ---------

  { 30, 5,  4,  4,  4,  0,   0,   2,  1,  1,  1,  1,  0,  0  }, /* OBS_VEHICLE */
  { 10, 2,  2,  2,  2,  0,   0,   1,  0,  0,  0,  0,  0,  0  }, /* OBS_INFANTRY */
  { 30, 5,  5,  5,  5,  30,  0,   4,  2,  2,  2,  2,  10, 0  }, /* OBS_LIGHT_HELO */
  { 35, 10, 10, 8,  8,  0,   40,  6,  5,  5,  4,  4,  0,  15 }, /* OBS_HEAVY_HELO */
  { 40, 40, 40, 40, 40, 40,  40,  30, 30, 30, 30, 30, 30, 30 }  /* OBS_FIRING */
};

int	Standard_Optics_Rain_Snow_Lt_Fog[OBS_MAX][TER_MAX_2] =
{
//  c   b   d   w   t   l    h    c   b   d   w   t   l   h 
//  l   r   e   o   o   t    v    l   r   e   o   o   t   v  
//  e   o   f   o   w        y    e   o   f   o   w       y 
//  a   k   i   d   n   l         a   k   i   d   n   l      
//  r   e   l   s       o    l    r   e   l   s       o   l  
//      n   a           w    o        n   a           w   o
//          d                w            d               w
//          e                             e
//----------- DAY ------------    ----------- NIGHT ---------

  { 12, 3,  2,  2,  2,  0,   0,   2,  1,  1,  1,  1,  0,  0  }, /* OBS_VEHICLE */
  {  2, 2,  2,  2,  2,  0,   0,   0,  0,  0,  0,  0,  0,  0  }, /* OBS_INFANTRY */
  { 30, 5,  5,  5,  5,  20,  0,   4,  1,  1,  1,  1,  12, 0  }, /* OBS_LIGHT_HELO */
  { 35, 10, 10, 10, 8,  0,   25,  6,  5,  5,  4,  4,  0,  18 }, /* OBS_HEAVY_HELO */
  { 25, 25, 25, 25, 25, 25,  25,  15, 15, 15, 15, 15, 15, 15 }  /* OBS_FIRING */
};

int	Standard_Optics_Overcast[OBS_MAX][TER_MAX_2] =
{
//  c   b   d   w   t   l    h    c   b   d   w   t   l   h 
//  l   r   e   o   o   t    v    l   r   e   o   o   t   v  
//  e   o   f   o   w        y    e   o   f   o   w       y 
//  a   k   i   d   n   l         a   k   i   d   n   l      
//  r   e   l   s       o    l    r   e   l   s       o   l  
//      n   a           w    o        n   a           w   o
//          d                w            d               w
//          e                             e
//----------- DAY ------------    ----------- NIGHT ---------

  { 12, 3,  2,  2,  2,  0,   0,   2,  1,  1,  1,  1,  0,  0  }, /* OBS_VEHICLE */
  {  2, 2,  2,  2,  2,  0,   0,   0,  0,  0,  0,  0,  0,  0  }, /* OBS_INFANTRY */
  { 30, 5,  5,  5,  5,  20,  0,   4,  1,  1,  1,  1,  12, 0  }, /* OBS_LIGHT_HELO */
  { 35, 10, 10, 10, 8,  0,   25,  6,  5,  5,  4,  4,  0,  18 }, /* OBS_HEAVY_HELO */
  { 35, 35, 35, 35, 35, 35,  35,  20, 20, 20, 20, 20, 20, 20 }  /* OBS_FIRING */
};

int	Standard_Optics_Heavy_Fog[OBS_MAX][TER_MAX_2] =
{
//  c   b   d   w   t   l    h    c   b   d   w   t   l   h 
//  l   r   e   o   o   t    v    l   r   e   o   o   t   v  
//  e   o   f   o   w        y    e   o   f   o   w       y 
//  a   k   i   d   n   l         a   k   i   d   n   l      
//  r   e   l   s       o    l    r   e   l   s       o   l  
//      n   a           w    o        n   a           w   o
//          d                w            d               w
//          e                             e
//----------- DAY ------------    ----------- NIGHT ---------

  { 12, 3,  2,  2,  2,  0,   0,   2,  1,  1,  1,  1,  0,  0  }, /* OBS_VEHICLE */
  {  2, 2,  2,  2,  2,  0,   0,   0,  0,  0,  0,  0,  0,  0  }, /* OBS_INFANTRY */
  { 30, 5,  5,  5,  5,  20,  0,   4,  1,  1,  1,  1,  12, 0  }, /* OBS_LIGHT_HELO */
  { 35, 10, 10, 10, 8,  0,  25,   6,  5,  5,  4,  4,  0,  18 }, /* OBS_HEAVY_HELO */
  {  6,  6,  6,  6,  6,  6,  6,   6,  6,  6,  6,  6,  6,  6  }  /* OBS_FIRING */
};

int	Blue_Thermal_Imager_Clear[OBS_MAX][TER_MAX] =
{
//  c    b   d   w   t   l    h
//  l    r   e   o   o   t    v
//  e    o   f   o   w        y
//  a    k   i   d   n   l     
//  r    e   l   s       o    l
//       n   a           w    o
//           d                w
//           e                 

  { 20, 10,  8,  8,  8,  0,   0 }, /* OBS_VEHICLE */
  { 6,   3,  3,  3,  3,  0,   0 }, /* OBS_INFANTRY */
  { 20, 10,  8,  8,  8,  30,  0 }, /* OBS_LIGHT_HELO */
  { 35, 15, 15, 12, 12,  0,  40 }, /* OBS_HEAVY_HELO */
  { 25, 25, 25, 25, 25, 25,  25 }  /* OBS_FIRING */
};

int	Blue_Thermal_Imager_Rain_Snow_Lt_Fog[OBS_MAX][TER_MAX] =
{
//  c    b   d   w   t   l    h
//  l    r   e   o   o   t    v
//  e    o   f   o   w        y
//  a    k   i   d   n   l     
//  r    e   l   s       o    l
//       n   a           w    o
//           d                w
//           e                 

  { 10,  8,  6,  6,  6,  0,   0 }, /* OBS_VEHICLE */
  { 4,   2,  2,  2,  2,  0,   0 }, /* OBS_INFANTRY */
  { 10,  8,  6,  6,  6,  15,  0 }, /* OBS_LIGHT_HELO */
  { 18, 10, 10, 14, 14,  0,  20 }, /* OBS_HEAVY_HELO */
  { 15, 15, 15, 15, 15, 15,  15 }  /* OBS_FIRING */
};

int	Blue_Thermal_Imager_Overcast[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 15,  9,  7,  7,  7,  0,   0 }, /* OBS_VEHICLE */
  { 4,   2,  2,  2,  2,  0,   0 }, /* OBS_INFANTRY */
  { 15,  9,  7,  7,  7,  15,  0 }, /* OBS_LIGHT_HELO */
  { 18, 10, 10, 14, 14,  0,  20 }, /* OBS_HEAVY_HELO */
  { 18, 18, 18, 18, 18, 18,  18 }  /* OBS_FIRING */
};

int	Blue_Thermal_Imager_Heavy_Fog[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 3,   2,  2,  2,  2,  0,  0 }, /* OBS_VEHICLE */
  { 1,   0,  0,  0,  0,  0,  0 }, /* OBS_INFANTRY */
  { 3,   2,  2,  2,  2,  4,  0 }, /* OBS_LIGHT_HELO */
  { 7,   4,  4,  6,  6,  0,  6 }, /* OBS_HEAVY_HELO */
  { 5,   5,  5,  5,  5,  5,  5 }  /* OBS_FIRING */
};

int	Red_Thermal_Imager_Clear[OBS_MAX][TER_MAX_2] =
{
//  c   b   d   w   t   l    h    c   b   d   w   t   l   h 
//  l   r   e   o   o   t    v    l   r   e   o   o   t   v  
//  e   o   f   o   w        y    e   o   f   o   w       y 
//  a   k   i   d   n   l         a   k   i   d   n   l      
//  r   e   l   s       o    l    r   e   l   s       o   l  
//      n   a           w    o        n   a           w   o
//          d                w            d               w
//          e                             e
//----------- DAY ------------   ----------- NIGHT ----------

  { 10, 6,  4,  4,  4,  0,   0,   7,  6,  4,  4,  4,  0,  0 }, /* OBS_VEHICLE */
  { 4,  2,  2,  2,  2,  0,   0,   4,  2,  2,  2,  2,  0,  0 }, /* OBS_INFANTRY */
  { 10, 6,  4,  4,  4, 12,   0,   7,  6,  4,  4,  4,  9,  0 }, /* OBS_LIGHT_HELO */
  { 12, 8,  8,  5,  5,  0,  15,   9,  8,  8,  5,  5,  0, 12 }, /* OBS_HEAVY_HELO */
  { 10, 10, 10, 10, 10, 10, 10,  10, 10, 10, 10, 10, 10, 10 }  /* OBS_FIRING */
};

int	Red_Thermal_Imager_Rain_Snow_Lt_Fog[OBS_MAX][TER_MAX_2] =
{
//  c   b   d   w   t   l    h    c   b   d   w   t   l   h 
//  l   r   e   o   o   t    v    l   r   e   o   o   t   v  
//  e   o   f   o   w        y    e   o   f   o   w       y 
//  a   k   i   d   n   l         a   k   i   d   n   l      
//  r   e   l   s       o    l    r   e   l   s       o   l  
//      n   a           w    o        n   a           w   o
//          d                w            d               w
//          e                             e
//----------- DAY ------------   ----------- NIGHT ----------

  { 6,  4,  2,  2,  2,  0,   0,   6,  4,  2,  2,  2,  0,  0 }, /* OBS_VEHICLE */
  { 3,  1,  1,  1,  1,  0,   0,   3,  1,  1,  1,  1,  0,  0 }, /* OBS_INFANTRY */
  { 6,  4,  2,  2,  2,  8,   0,   6,  4,  2,  2,  2,  8,  0 }, /* OBS_LIGHT_HELO */
  { 8,  6,  6,  3,  3,  0,  10,   8,  6,  6,  3,  3,  0, 10 }, /* OBS_HEAVY_HELO */
  { 7,  7,  7,  7,  7,  7,   7,   7,  7,  7,  7,  7,  7,  7 }  /* OBS_FIRING */
};

int	Red_Thermal_Imager_Overcast[OBS_MAX][TER_MAX_2] =
{
//  c   b   d   w   t   l    h    c   b   d   w   t   l   h 
//  l   r   e   o   o   t    v    l   r   e   o   o   t   v  
//  e   o   f   o   w        y    e   o   f   o   w       y 
//  a   k   i   d   n   l         a   k   i   d   n   l      
//  r   e   l   s       o    l    r   e   l   s       o   l  
//      n   a           w    o        n   a           w   o
//          d                w            d               w
//          e                             e
//----------- DAY ------------   ----------- NIGHT ----------

  { 7,  6,  4,  4,  4,  0,   0,   7,  6,  4,  4,  4,  0,  0 }, /* OBS_VEHICLE */
  { 4,  2,  2,  2,  2,  0,   0,   4,  2,  2,  2,  2,  0,  0 }, /* OBS_INFANTRY */
  { 7,  6,  4,  4,  4, 12,   0,   7,  6,  4,  4,  4,  9,  0 }, /* OBS_LIGHT_HELO */
  { 9,  8,  8,  5,  5,  0,  12,   9,  8,  8,  5,  5,  0, 12 }, /* OBS_HEAVY_HELO */
  { 10, 10, 10, 10, 10, 10, 10,  10, 10, 10, 10, 10, 10, 10 }  /* OBS_FIRING */
};

int	Red_Thermal_Imager_Heavy_Fog[OBS_MAX][TER_MAX_2] =
{
//  c   b   d   w   t   l    h    c   b   d   w   t   l   h 
//  l   r   e   o   o   t    v    l   r   e   o   o   t   v  
//  e   o   f   o   w        y    e   o   f   o   w       y 
//  a   k   i   d   n   l         a   k   i   d   n   l      
//  r   e   l   s       o    l    r   e   l   s       o   l  
//      n   a           w    o        n   a           w   o
//          d                w            d               w
//          e                             e
//----------- DAY ------------   ----------- NIGHT ----------

  { 3,  2,  1,  1,  1,  0,   0,   3,  2,  1,  1,  1,  0,  0 }, /* OBS_VEHICLE */
  { 1,  0,  0,  0,  0,  0,   0,   1,  0,  0,  0,  0,  0,  0 }, /* OBS_INFANTRY */
  { 3,  2,  2,  2,  2,  4,   0,   3,  4,  2,  2,  2,  4,  0 }, /* OBS_LIGHT_HELO */
  { 5,  4,  4,  3,  3,  0,   6,   5,  4,  4,  3,  3,  0,  6 }, /* OBS_HEAVY_HELO */
  { 2,  2,  2,  2,  2,  2,   2,   2,  2,  2,  2,  2,  2,  2 }  /* OBS_FIRING */
};

int	Ambient_Light_Enhancer_Clear[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 8,  4,  4,  4,  4,  0,   0 }, /* OBS_VEHICLE */
  { 5,  3,  3,  3,  3,  0,   0 }, /* OBS_INFANTRY */
  { 8,  4,  4,  4,  4,  12,  0 }, /* OBS_LIGHT_HELO */
  { 10, 6,  6,  6,  6,  0,  16 }, /* OBS_HEAVY_HELO */
  { 15,15, 15, 15, 15,  15, 15 }  /* OBS_FIRING */
};

int	Ambient_Light_Enhancer_Rain_Snow_Lt_Fog[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 2,  3,  3,  3,  3,  0,   0 }, /* OBS_VEHICLE */
  { 2,  1,  1,  1,  1,  0,   0 }, /* OBS_INFANTRY */
  { 2,  3,  3,  3,  3,  4,   0 }, /* OBS_LIGHT_HELO */
  { 6,  3,  4,  4,  5,  0,   6 }, /* OBS_HEAVY_HELO */
  { 6,  6,  6,  6,  6,  6,   6 }  /* OBS_FIRING */
};

int	Ambient_Light_Enhancer_Overcast[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 1,  3,  3,  3,  3,  0,   0 }, /* OBS_VEHICLE */
  { 2,  1,  1,  1,  1,  0,   0 }, /* OBS_INFANTRY */
  { 1,  3,  3,  3,  3,  4,   0 }, /* OBS_LIGHT_HELO */
  { 2,  5,  5,  5,  5,  0,   5 }, /* OBS_HEAVY_HELO */
  { 6,  6,  6,  6,  6,  6,   6 }  /* OBS_FIRING */
};

int	Ambient_Light_Enhancer_Heavy_Fog[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 0,  0,  0,  0,  0,  0,  0 }, /* OBS_VEHICLE */
  { 0,  0,  0,  0,  0,  0,  0 }, /* OBS_INFANTRY */
  { 0,  0,  0,  0,  0,  0,  0 }, /* OBS_LIGHT_HELO */
  { 0,  0,  0,  0,  0,  0,  0 }, /* OBS_HEAVY_HELO */
  { 0,  0,  0,  0,  0,  0,  0 }  /* OBS_FIRING */
};

int	Infrared_Searchlight_Clear[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 12, 4,  3,  3,  3,  0,   0 }, /* OBS_VEHICLE */
  { 10, 2,  2,  2,  2,  0,   0 }, /* OBS_INFANTRY */
  { 12, 4,  4,  4,  4,  12,  0 }, /* OBS_LIGHT_HELO */
  { 12, 6,  6,  6,  6,  0,  12 }, /* OBS_HEAVY_HELO */
  { 15,15, 15, 15, 15,  15, 15 }  /* OBS_FIRING */
};

int	Infrared_Searchlight_Rain_Snow_Lt_Fog[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 6,  3,  2,  2,  2,  0,   0 }, /* OBS_VEHICLE */
  { 2,  1,  1,  1,  1,  0,   0 }, /* OBS_INFANTRY */
  { 2,  3,  3,  3,  3,  4,   0 }, /* OBS_LIGHT_HELO */
  { 6,  3,  4,  4,  5,  0,   6 }, /* OBS_HEAVY_HELO */
  { 6,  6,  6,  6,  6,  6,   6 }  /* OBS_FIRING */
};

int	Infrared_Searchlight_Overcast[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 12, 4,  3,  3,  3,  0,   0 }, /* OBS_VEHICLE */
  { 10, 2,  2,  2,  2,  0,   0 }, /* OBS_INFANTRY */
  { 12, 4,  4,  4,  4, 12,   0 }, /* OBS_LIGHT_HELO */
  { 12, 5,  6,  6,  6,  0,  12 }, /* OBS_HEAVY_HELO */
  { 15,15, 15, 15, 15,  15, 15 }  /* OBS_FIRING */
};

int	Infrared_Searchlight_Heavy_Fog[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 2,  1,  1,  1,  1,  0,  0 }, /* OBS_VEHICLE */
  { 0,  0,  0,  0,  0,  0,  0 }, /* OBS_INFANTRY */
  { 2,  1,  1,  1,  1,  2,  0 }, /* OBS_LIGHT_HELO */
  { 3,  2,  2,  2,  2,  0,  4 }, /* OBS_HEAVY_HELO */
  { 6,  6,  6,  6,  6,  6,  6 }  /* OBS_FIRING */
};

int	Whitelight_Searchlight_Clear[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 15, 4,  3,  3,  3,  0,   0 }, /* OBS_VEHICLE */
  { 10, 2,  2,  2,  2,  0,   0 }, /* OBS_INFANTRY */
  { 15,15, 15, 15, 15,  15,  0 }, /* OBS_LIGHT_HELO */
  { 20,15, 15, 15, 15,  0,  15 }, /* OBS_HEAVY_HELO */
  { 15,15, 15, 15, 15,  15, 15 }  /* OBS_FIRING */
};

int	Whitelight_Searchlight_Rain_Snow_Lt_Fog[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 8,  3,  2,  2,  2,  0,   0 }, /* OBS_VEHICLE */
  { 2,  1,  1,  1,  1,  0,   0 }, /* OBS_INFANTRY */
  { 8,  8,  8,  8,  8,  10,  0 }, /* OBS_LIGHT_HELO */
  { 10,10, 10, 10, 10,  0,  12 }, /* OBS_HEAVY_HELO */
  { 15,15, 15, 15, 15,  15, 15 }  /* OBS_FIRING */
};

int	Whitelight_Searchlight_Overcast[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 15, 4,  3,  3,  3,  0,   0 }, /* OBS_VEHICLE */
  { 10, 2,  2,  2,  2,  0,   0 }, /* OBS_INFANTRY */
  { 15,15, 15, 15, 15,  15,  0 }, /* OBS_LIGHT_HELO */
  { 15,15, 15, 15, 15,  0,  15 }, /* OBS_HEAVY_HELO */
  { 15,15, 15, 15, 15,  15, 15 }  /* OBS_FIRING */
};

int	Whitelight_Searchlight_Heavy_Fog[OBS_MAX][TER_MAX] =
{
//  c   b   d   w   t   l    h
//  l   r   e   o   o   t    v
//  e   o   f   o   w        y
//  a   k   i   d   n   l     
//  r   e   l   s       o    l
//      n   a           w    o
//          d                w
//          e                 

  { 2,  1,  1,  1,  1,  0,   0 }, /* OBS_VEHICLE */
  { 0,  0,  0,  0,  0,  0,   0 }, /* OBS_INFANTRY */
  { 2,  1,  1,  1,  1,  4,   0 }, /* OBS_LIGHT_HELO */
  { 4,  2,  2,  2,  2,  0,   4 }, /* OBS_HEAVY_HELO */
  { 6,  6,  6,  6,  6,  6,   6 }  /* OBS_FIRING */
};

int ObservationData::getObservationRange(UnitType unitType, int side, int optics, int terrain, int lowlevel, int defilade, int fired, int timeofday, int weather)
{
	// shift according to time of day, night values are after the day values
	int offset = (timeofday == TIME_NIGHT) ? TER_MAX : 0;

	// if unit has fired, this overrides everything
	int basicUnitType;
	if (fired)
	{
		basicUnitType = OBS_FIRING;
	}
	else
	{
		// get the generic unit type from the specific unit type
		switch (unitType)
		{
			case TANK:
			case TLAV:
			case WV:
				basicUnitType = OBS_VEHICLE;
				break;
			case INF:
				basicUnitType = OBS_INFANTRY;
				break;
			case LHX:
			case ARH:
			case UHM:
				basicUnitType = OBS_LIGHT_HELO;
				break;
			case UHH:
				basicUnitType = OBS_HEAVY_HELO;
				break;
			default:
				basicUnitType = OBS_INFANTRY;
				break;
		}
	}

	// if unit is a helo at low level, this overrides everything
	int basicTerrainType;
	if ( lowlevel && (unitType == UHM || unitType == ARH || unitType == LHX) )
	{
		basicTerrainType = TER_LIGHT_LOW;
	}
	else if ( lowlevel && unitType == UHH )
	{
		basicTerrainType = TER_HEAVY_LOW;
	}
	else if (defilade)
	{
		basicTerrainType = TER_DEFILADE;
	}
	else
	{
		// get specific terrain
		switch (terrain)
		{
			case CLEAR:
			case RIVER:
				basicTerrainType = TER_CLEAR;
				break;
			case WOODS:
				basicTerrainType = TER_WOODS;
				break;
			case TOWN:
				basicTerrainType = TER_TOWN;
				break;
		}
	}

	// determine the max range based on optics used
	switch (optics)
	{
		// standard optical sight - eyes, binoculars, gun sights, etc
		case OPTICS_OPTICAL_SIGHT:
		{
			switch (weather)
			{
				case WEATHER_CLEAR:
					return (Standard_Optics_Clear[basicUnitType][basicTerrainType + offset]);
					break;
				case WEATHER_RAIN:
				case WEATHER_SNOW:
				case WEATHER_LT_FOG:
					return (Standard_Optics_Rain_Snow_Lt_Fog[basicUnitType][basicTerrainType + offset]);
					break;
				case WEATHER_OVERCAST:
					return (Standard_Optics_Overcast[basicUnitType][basicTerrainType + offset]);
					break;
				case WEATHER_HVY_FOG:
					return (Standard_Optics_Heavy_Fog[basicUnitType][basicTerrainType + offset]);
					break;
			}
			return (Standard_Optics_Clear[basicUnitType][basicTerrainType + offset]);
		}

		// thermal imaging equipment - FLIR, TADS/PNVS, etc
		case OPTICS_THERMAL_IMAGER:
		{
			switch (weather)
			{
				case WEATHER_CLEAR:
					if (side == SIDE_BLUE)
						return (Blue_Thermal_Imager_Clear[basicUnitType][basicTerrainType]);
					else
						return (Red_Thermal_Imager_Clear[basicUnitType][basicTerrainType + offset]);
					break;
				case WEATHER_RAIN:
				case WEATHER_SNOW:
				case WEATHER_LT_FOG:
					if (side == SIDE_BLUE)
						return (Blue_Thermal_Imager_Rain_Snow_Lt_Fog[basicUnitType][basicTerrainType]);
					else
						return (Red_Thermal_Imager_Rain_Snow_Lt_Fog[basicUnitType][basicTerrainType + offset]);
					break;
				case WEATHER_OVERCAST:
					if (side == SIDE_BLUE)
						return (Blue_Thermal_Imager_Overcast[basicUnitType][basicTerrainType]);
					else
						return (Red_Thermal_Imager_Overcast[basicUnitType][basicTerrainType + offset]);
					break;
				case WEATHER_HVY_FOG:
					if (side == SIDE_BLUE)
						return (Blue_Thermal_Imager_Heavy_Fog[basicUnitType][basicTerrainType]);
					else
						return (Red_Thermal_Imager_Heavy_Fog[basicUnitType][basicTerrainType + offset]);
					break;
			}
			if (side == SIDE_BLUE)
				return (Blue_Thermal_Imager_Clear[basicUnitType][basicTerrainType]);
			else
				return (Red_Thermal_Imager_Clear[basicUnitType][basicTerrainType + offset]);
		}

		// Ambient Light Enhancer - NVG
		case OPTICS_AMBIENT_LIGHT_ENHANCER:
		{
			if (timeofday == TIME_DAY)
				return 0;

			switch (weather)
			{
				case WEATHER_CLEAR:
					return (Ambient_Light_Enhancer_Clear[basicUnitType][basicTerrainType]);
					break;
				case WEATHER_RAIN:
				case WEATHER_SNOW:
				case WEATHER_LT_FOG:
					return (Ambient_Light_Enhancer_Rain_Snow_Lt_Fog[basicUnitType][basicTerrainType]);
					break;
				case WEATHER_OVERCAST:
					return (Ambient_Light_Enhancer_Overcast[basicUnitType][basicTerrainType]);
					break;
				case WEATHER_HVY_FOG:
					return (Ambient_Light_Enhancer_Heavy_Fog[basicUnitType][basicTerrainType]);
					break;
			}
			return (Ambient_Light_Enhancer_Clear[basicUnitType][basicTerrainType]);
		}
		
		// infrared searchlight
		case OPTICS_INFRARED_SEARCHLIGHT:
		{
			if (timeofday == TIME_DAY)
				return 0;

			switch (weather)
			{
			case WEATHER_CLEAR:
				return (Infrared_Searchlight_Clear[basicUnitType][basicTerrainType]);
				break;
			case WEATHER_RAIN:
			case WEATHER_SNOW:
			case WEATHER_LT_FOG:
				return (Infrared_Searchlight_Rain_Snow_Lt_Fog[basicUnitType][basicTerrainType]);
				break;
			case WEATHER_OVERCAST:
				return (Infrared_Searchlight_Overcast[basicUnitType][basicTerrainType]);
				break;
			case WEATHER_HVY_FOG:
				return (Infrared_Searchlight_Heavy_Fog[basicUnitType][basicTerrainType]);
				break;
			}
			return (Infrared_Searchlight_Clear[basicUnitType][basicTerrainType]);
		}

		// whitelight searchlight
		case OPTICS_WHITELIGHT_SEARCHLIGHT:
		{
			if (timeofday == TIME_DAY)
				return 0;

			switch (weather)
			{
			case WEATHER_CLEAR:
				return (Whitelight_Searchlight_Clear[basicUnitType][basicTerrainType]);
				break;
			case WEATHER_RAIN:
			case WEATHER_SNOW:
			case WEATHER_LT_FOG:
				return (Whitelight_Searchlight_Rain_Snow_Lt_Fog[basicUnitType][basicTerrainType]);
				break;
			case WEATHER_OVERCAST:
				return (Whitelight_Searchlight_Overcast[basicUnitType][basicTerrainType]);
				break;
			case WEATHER_HVY_FOG:
				return (Whitelight_Searchlight_Heavy_Fog[basicUnitType][basicTerrainType]);
				break;
			}
			return (Whitelight_Searchlight_Clear[basicUnitType][basicTerrainType]);
		}
	}
	return (Standard_Optics_Clear[basicUnitType][basicTerrainType + offset]);
}
