#ifndef __APP_COM_H__
#define __APP_COM_H__


#define ChansFre(fre) 1000000/(fre)   //32Ï¸·Ö
#define XF (32)
#define FREE_MEMORY(POINT) if (NULL != POINT){ free(POINT); POINT = NULL; }

#define MUTEX_PEND_WITH_RET(funcname, mutexobj, err) \
	OSMutexPend(mutexobj, 5000, &err); \
	if (0 != err){return err;}

#endif
