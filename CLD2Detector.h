#include <stdlib.h>
#include <stdio.h>

#include "cld2tablesummary.h"
#include "compact_lang_det_impl.h"
#include "debug.h"
#include "integral_types.h"
#include "lang_script.h"
#include "compact_lang_det_hint_code.h"
#include "utf8statetable.h"

#include "DetectedLanguage.h"

#ifndef __CLD2Detector_H_
#define __CLD2Detector_H_

static const int UNKNOWN_ENCODING = 0;

class CLD2Detector {
public:
    CLD2Detector();

    DetectedLanguage detect(const char *&buffer);

    void setPlainText(bool pt);

    void setTldHint(const char *tldHint);

    void setEncHint(int encHint);

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


#endif //__CLD2Detector_H_
