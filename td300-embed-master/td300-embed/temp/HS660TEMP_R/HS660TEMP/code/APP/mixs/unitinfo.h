#ifndef __UNIT_INFO_H__
#define __UNIT_INFO_H__

#include "app_downmsg.h"

#define UnitCount   (2)

#define Unit_Mix (0)
#define Unit_Smp (1)

#define UNITID(canId) (((canId) == CanId_Mix) ? Unit_Mix : ((canId) ==CanId_SDisdk) ? Unit_Smp : 0xf)
#define NAMEID(uId) (((uId) == Unit_Mix) ? DName_Mix : ((uId) == Unit_Smp) ? DName_SDisdk : 0xf)
#define CANID(uId) (((uId) == Unit_Mix) ? CanId_Mix : ((uId) == Unit_Smp) ? CanId_SDisdk : 0xf)
#endif
