#ifndef __DIK2STR__
#define __DIK2STR__

#include "inputengine.h"

extern char						DikName[MaxKeys][17];
extern char						DikCharName[MaxKeys];

char * DIK2STR(int CharCode);
int CharNameAvailable(int CharCode);
char DIK2ASCII(int CharCode);

void SetDikNames();
#endif