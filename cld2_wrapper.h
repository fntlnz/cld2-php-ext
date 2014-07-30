#include <stdlib.h>
#include <stdio.h>

#include "cld2tablesummary.h"
#include "compact_lang_det_impl.h"
#include "debug.h"
#include "integral_types.h"
#include "lang_script.h"
#include "utf8statetable.h"

#include "detected_language.h"

#ifndef CLD2_CLASS_H
#define CLD2_CLASS_H


// Scaffolding
static const int UNKNOWN_ENCODING = 0;

#ifndef CLD2_DYNAMIC_MODE
extern const CLD2::UTF8PropObj cld_generated_CjkUni_obj;
extern const CLD2::CLD2TableSummary kCjkDeltaBi_obj;
extern const CLD2::CLD2TableSummary kDistinctBiTable_obj;
extern const CLD2::CLD2TableSummary kQuad_obj;
extern const CLD2::CLD2TableSummary kDeltaOcta_obj;
extern const CLD2::CLD2TableSummary kDistinctOcta_obj;
extern const CLD2::CLD2TableSummary kOcta2_obj;
extern const short kAvgDeltaOctaScore[];
#endif

class CLD2Wrapper {
public:
    CLD2Wrapper();

    DetectedLanguage detect(const char *&buffer);

    void setPlainText(bool pt);

    void setTldHint(const char *tldHint);

    void setEncHint(int encHint);

    void setLangHintFromLanguageName(const char *languageName);

    void setLangHint(CLD2::Language langHint);

    bool isPlainText();

    char const *getTldHint();

    int getEncHint();

    CLD2::Language const &getLangHint();

protected:
    bool plainText;
    const char *tldHint;
    int encHint;
    CLD2::Language langHint;
};

#endif
