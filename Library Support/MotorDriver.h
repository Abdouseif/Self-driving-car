#include "pwmPF.h"
#include "PORTS_INIT.h"

void Init_Motors(void);
void FWD_Move(unsigned int, unsigned int);
void BWD_Move(unsigned int, unsigned int);
void FWD_BWD_Move(unsigned int, unsigned int);
void BWD_FWD_Move(unsigned int, unsigned int);
void Full_Brake(void);
void R_Half_Brake(void);
void L_Half_Brake(void);