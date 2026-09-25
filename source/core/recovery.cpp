#include "recovery.h"
#include <stdio.h>
namespace { bool safe=false; unsigned hb=0; char faultText[96]=""; }
namespace aether::recovery {
void init(){safe=false;hb=0;faultText[0]=0;}
void heartbeat(){hb++;}
void fault(const char*m){safe=true;unsigned i=0;if(m)while(m[i]&&i<95){faultText[i]=m[i];i++;}faultText[i]=0;FILE*f=fopen("REVF/RECOVERY/LAST.FLT","w");if(f){fprintf(f,"%s\n",faultText);fclose(f);}}
bool safeMode(){return safe;} const char*lastFault(){return faultText;}
}