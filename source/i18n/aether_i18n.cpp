#include "aether_i18n.h"
#include <stdio.h>
#include <string.h>
namespace {
aether::i18n::Language lang=aether::i18n::LANG_EN;
bool pack=false;
const aether::i18n::LanguageInfo langs[]={
{aether::i18n::LANG_AUTO,"auto","AUTO",true},{aether::i18n::LANG_EN,"en","English",true},{aether::i18n::LANG_ES,"es","Espanol",true},{aether::i18n::LANG_FR,"fr","Francais",true},{aether::i18n::LANG_DE,"de","Deutsch",true},{aether::i18n::LANG_IT,"it","Italiano",true},{aether::i18n::LANG_PT,"pt","Portugues",true},{aether::i18n::LANG_NL,"nl","Nederlands",true},{aether::i18n::LANG_DA,"da","Dansk",true},{aether::i18n::LANG_SV,"sv","Svenska",true},{aether::i18n::LANG_NO,"no","Norsk",true},{aether::i18n::LANG_FI,"fi","Suomi",true},{aether::i18n::LANG_IS,"is","Islenska",true},{aether::i18n::LANG_PL,"pl","Polski",true},{aether::i18n::LANG_CS,"cs","Cestina",true},{aether::i18n::LANG_SK,"sk","Slovencina",true},{aether::i18n::LANG_HU,"hu","Magyar",true},{aether::i18n::LANG_RO,"ro","Romana",true},{aether::i18n::LANG_TR,"tr","Turkce",true},{aether::i18n::LANG_EL,"el","Ellinika",true},{aether::i18n::LANG_RU,"ru","Russkiy",true},{aether::i18n::LANG_UK,"uk","Ukrainska",true},{aether::i18n::LANG_BG,"bg","Bulgarski",true},{aether::i18n::LANG_SR,"sr","Srpski",true},{aether::i18n::LANG_HR,"hr","Hrvatski",true},{aether::i18n::LANG_SL,"sl","Slovenscina",true},{aether::i18n::LANG_HE,"he","Ivrit",true},{aether::i18n::LANG_AR,"ar","Arabi",true},{aether::i18n::LANG_FA,"fa","Farsi",true},{aether::i18n::LANG_HI,"hi","Hindi",true},{aether::i18n::LANG_BN,"bn","Bangla",true},{aether::i18n::LANG_UR,"ur","Urdu",true},{aether::i18n::LANG_TA,"ta","Tamil",true},{aether::i18n::LANG_TE,"te","Telugu",true},{aether::i18n::LANG_MR,"mr","Marathi",true},{aether::i18n::LANG_GU,"gu","Gujarati",true},{aether::i18n::LANG_KN,"kn","Kannada",true},{aether::i18n::LANG_ML,"ml","Malayalam",true},{aether::i18n::LANG_TH,"th","Thai",true},{aether::i18n::LANG_VI,"vi","Tieng Viet",true},{aether::i18n::LANG_ID,"id","Bahasa Indonesia",true},{aether::i18n::LANG_MS,"ms","Bahasa Melayu",true},{aether::i18n::LANG_FIL,"fil","Filipino",true},{aether::i18n::LANG_SW,"sw","Kiswahili",true},{aether::i18n::LANG_AM,"am","Amharic",true},{aether::i18n::LANG_ZU,"zu","isiZulu",true},{aether::i18n::LANG_XH,"xh","isiXhosa",true},{aether::i18n::LANG_YO,"yo","Yoruba",true},{aether::i18n::LANG_JA,"ja","Nihongo",true},{aether::i18n::LANG_KO,"ko","Hanguk-eo",true},{aether::i18n::LANG_ZH,"zh","Zhongwen",true},{aether::i18n::LANG_SIGN_ASL,"ase","American Sign Language",true},{aether::i18n::LANG_SIGN_BSL,"bfi","British Sign Language",true},{aether::i18n::LANG_SIGN_ISL,"isg","International Sign",true},{aether::i18n::LANG_SIGN_JSL,"jsl","Japanese Sign Language",true}};
const char* en(const char*k){if(!strcmp(k,"HOME"))return"HOME";if(!strcmp(k,"SETTINGS"))return"SETTINGS";if(!strcmp(k,"READY"))return"READY";if(!strcmp(k,"SAFE"))return"SAFE";if(!strcmp(k,"OFFLINE"))return"OFFLINE";if(!strcmp(k,"OPEN"))return"OPEN";if(!strcmp(k,"BACK"))return"BACK";if(!strcmp(k,"RESET"))return"RESET";if(!strcmp(k,"LANGUAGE"))return"LANGUAGE";if(!strcmp(k,"CONTROL"))return"CONTROL";if(!strcmp(k,"SAVE"))return"SAVE";if(!strcmp(k,"SYSTEM"))return"SYSTEM";if(!strcmp(k,"PROJECT"))return"PROJECT";if(!strcmp(k,"TOUCH"))return"TOUCH";return k;}
}
namespace aether::i18n {
void init(){lang=LANG_EN;pack=false;}
void tick(){}
bool loadPack(const char*code){char path[64];snprintf(path,sizeof(path),"REVF/LANG/%s.LNG",code);FILE*f=fopen(path,"rb");if(!f)return false;fclose(f);pack=true;return true;}
bool save(){return true;}
void next(){lang=(Language)((lang+1)%LANG_COUNT);pack=false;loadPack(langs[lang].code);}
void previous(){lang=(Language)((lang+LANG_COUNT-1)%LANG_COUNT);pack=false;loadPack(langs[lang].code);}
void adjust(int d){if(d>0)next();else if(d<0)previous();}
Language current(){return lang;}
const LanguageInfo& info(Language x){return langs[x<LANG_COUNT?x:LANG_EN];}
const char* text(const char*k){return en(k);}
const char* languageName(){return info(lang).name;}
bool externalPackReady(){return pack;} unsigned languageCount(){return (unsigned)(sizeof(langs)/sizeof(langs[0]));} bool selectCode(const char*code){for(unsigned i=0;i<languageCount();++i)if(!strcmp(code,langs[i].code)){lang=langs[i].id;pack=false;loadPack(code);return true;}return false;}
}
