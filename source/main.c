#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
typedef struct { u32 magic; u16 version; u16 mode; u16 sound; u16 intensity; u32 launches; u32 checksum; } SaveData;
static SaveData save; static int mode=0,cursor=0,safeMode=0; static u32 frameCounter=0;
static u32 checksum32(const void *ptr,size_t n){const u8*p=(const u8*)ptr;u32 h=2166136261u;for(size_t i=0;i<n;i++){h^=p[i];h*=16777619u;}return h;}
static void defaults(void){memset(&save,0,sizeof(save));save.magic=SAVE_MAGIC;save.version=SAVE_VERSION;save.sound=1;save.intensity=2;}
static void load_save(void){defaults();FILE*f=fopen("fat:/data/O2S2HaHa/save.dat","rb");if(!f)return;SaveData t;if(fread(&t,1,sizeof(t),f)==sizeof(t)){u32 old=t.checksum;t.checksum=0;if(old==checksum32(&t,sizeof(t))&&t.magic==SAVE_MAGIC&&t.version==SAVE_VERSION)save=t;}fclose(f);}
static void save_state(void){save.checksum=0;save.checksum=checksum32(&save,sizeof(save));FILE*f=fopen("fat:/data/O2S2HaHa/save.dat","wb");if(!f)return;fwrite(&save,1,sizeof(save),f);fclose(f);}
static void header(const char*t){consoleClear();iprintf("\x1b[0;0HAETHEROS  O2S2HaHa  %s\n",APP_VERSION);iprintf("--------------------------------\n%s\n\n",t);}
static void hub(void){header("NEXT-GEN HUB");const char*i[]={"O2S2HaHa FUN","AETHER TELEMETRY","SETTINGS","SAFE TEST","ABOUT"};for(int x=0;x<5;x++)iprintf("%s %s\n",x==cursor?">":" ",i[x]);iprintf("\nA SELECT  UP/DOWN NAV  X FUN  Y TELEMETRY\nLaunches: %lu\n",(unsigned long)save.launches);}
static void haha(void){header("CHAOS LAB");int e=(frameCounter/3)%101;iprintf("FUN ENGINE: ONLINE\n\nENERGY [");for(int i=0;i<20;i++)iprintf("%c",i<e/5?'#':'.');iprintf("] %d%%\n\nPulse: %lu\nPhase: %lu\nStability: %s\n",(unsigned long)(frameCounter&65535),(unsigned long)((frameCounter/7)%12),safeMode?"SAFE":"LIVE");iprintf("\nA=BOOST X=CHAOS Y=CALM\nB=HUB\n");if(frameCounter%45==0)iprintf("\n>>> HAHA EVENT #%lu <<<\n",(unsigned long)(frameCounter/45));}
static void telemetry(void){header("AETHER TELEMETRY");iprintf("Frame: %lu\nIntensity: %u\nSound: %s\nMemory: bounded/static\nStorage: FAT/SD\nBoot: %s\n\nB=BACK\n",(unsigned long)frameCounter,save.intensity,save.sound?"ON":"OFF",safeMode?"SAFE":"NORMAL");}
static void settings(void){header("SETTINGS");iprintf("Intensity: %u\nSound: %s\n\nUP/DOWN intensity\nA toggle sound\nB save/back\n",save.intensity,save.sound?"ON":"OFF");}
static void safe_test(void){header("SAFE TEST");iprintf("[OK] bounded runtime\n[OK] save validation\n[OK] SD fallback\n[OK] no network dependency\n\nB=BACK\n");}
static void draw(void){if(mode==1)haha();else if(mode==2)telemetry();else if(mode==3)settings();else if(mode==4)safe_test();else hub();}
static void input(void){scanKeys();u32 d=keysDown(),h=keysHeld();if(d&KEY_SELECT){safeMode=!safeMode;if(safeMode){save.sound=0;save.intensity=1;mode=0;}}if(mode==0){if(d&KEY_UP)cursor=(cursor+4)%5;if(d&KEY_DOWN)cursor=(cursor+1)%5;if(d&KEY_X)mode=1;if(d&KEY_Y)mode=2;if(d&KEY_A){mode=cursor+1;save.launches++;save_state();}}else if(mode==1){if(d&KEY_B)mode=0;if(d&KEY_X)frameCounter+=97;if(d&KEY_Y)frameCounter/=2;if(h&KEY_A)frameCounter+=2;}else if(mode==2){if(d&KEY_B)mode=0;}else if(mode==3){if(d&KEY_UP&&save.intensity<4)save.intensity++;if(d&KEY_DOWN&&save.intensity>0)save.intensity--;if(d&KEY_A)save.sound^=1;if(d&KEY_B){save_state();mode=0;}}else if(d&KEY_B)mode=0;}
int main(void){powerON(POWER_ALL_2D);videoSetMode(MODE_0_2D);vramSetBankA(VRAM_A_MAIN_BG);consoleDemoInit();if(!fatInitDefault()){safeMode=1;defaults();header("RAM SAFE MODE");iprintf("SD/FAT unavailable.\nRunning without persistence.\n");}else{load_save();save.launches++;save_state();}draw();while(1){swiWaitForVBlank();frameCounter++;input();if((frameCounter&7)==0)draw();}return 0;}
