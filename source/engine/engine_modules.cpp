#include "engine_modules.h"
#include <stdio.h>
namespace { unsigned l=0,a=1; }
namespace aether::engine {
void init(){l=0;a=1;}
void tick(){l=l*1664525u+1013904223u;a=a*1103515245u+12345u;}
void saveProject(){FILE*f=fopen("REVF/PROJECTS/DEFAULT.APRJ","w");if(f){fprintf(f,"AETHEROS REV:F PROJECT\nLAB=%lu\nAI=%lu\n",l,a);fclose(f);}}
bool projectExists(){FILE*f=fopen("REVF/PROJECTS/DEFAULT.APRJ","r");if(!f)return false;fclose(f);return true;}
void resetProject(){remove("REVF/PROJECTS/DEFAULT.APRJ");}
unsigned labValue(){return l;}
unsigned aiValue(){return a;}
}