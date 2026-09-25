#pragma once
#include <nds.h>
namespace aether::i18n {
enum Language : u8 {
 LANG_AUTO=0, LANG_EN, LANG_ES, LANG_FR, LANG_DE, LANG_IT, LANG_PT, LANG_NL, LANG_DA, LANG_SV,
 LANG_NO, LANG_FI, LANG_IS, LANG_PL, LANG_CS, LANG_SK, LANG_HU, LANG_RO, LANG_TR, LANG_EL,
 LANG_RU, LANG_UK, LANG_BG, LANG_SR, LANG_HR, LANG_SL, LANG_HE, LANG_AR, LANG_FA, LANG_HI,
 LANG_BN, LANG_UR, LANG_TA, LANG_TE, LANG_MR, LANG_GU, LANG_KN, LANG_ML, LANG_TH, LANG_VI,
 LANG_ID, LANG_MS, LANG_FIL, LANG_SW, LANG_AM, LANG_ZU, LANG_XH, LANG_YO, LANG_JA, LANG_KO,
 LANG_ZH, LANG_COUNT
};
struct LanguageInfo { Language id; const char* code; const char* name; bool builtIn; };
void init(); void tick(); bool loadPack(const char* code); bool save();
void next(); void previous(); void adjust(int delta);
Language current(); const LanguageInfo& info(Language);
const char* text(const char* key); const char* languageName();
bool externalPackReady();
}
