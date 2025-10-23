#define PlayerBlitWidth	(4*2*32)
#define PlayerBlitHeight (TickFrames*32)
#define PlayerBlitStartX	(0)
#define PlayerBlitStartY	(Team*PlayerBlitHeight)


#define PLSETBLIT SetBlit(2*32 , 4*32 , 32, 32, DIR_8, TT_PINGPONG, 5, &(TheGame->g_pDDSPlayer))

#define WeaponBlitX PlayerBlitWidth
#define WeaponBlitY ((Team*PlayerBlitHeight)+ 128)

#define RotatedX PlayerBlitWidth
#define RotatedY (Team*PlayerBlitHeight)

