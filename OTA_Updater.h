

#ifndef updaterheadergaurd
#define updaterheadergaurd

namespace test{
extern char * SSid;
extern char * PAss; 
}
//The reset function
extern void(* resetFunc) (void);

extern void update_firmware(void * p);


#endif
