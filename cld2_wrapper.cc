#include "cld2_wrapper.h"

CLD2Wrapper::CLD2Wrapper()
{
    this->mIsPlainText = false;
}

DetectedLanguage CLD2Wrapper::detect(const char* &buffer)
{
    const char* tldhint = "";
    int enchint = UNKNOWN_ENCODING;
    CLD2::Language langhint = CLD2::UNKNOWN_LANGUAGE;

    int flags = 0;
    bool is_reliable;

    // Full-blown flag-bit and hints interface
    bool allow_extended_lang = true;
    CLD2::Language plus_one = CLD2::UNKNOWN_LANGUAGE;

    int n = strlen(buffer);

    // Detect language
    CLD2::Language summary_lang = CLD2::UNKNOWN_LANGUAGE;

    CLD2::Language language3[3];
    int percent3[3];
    double normalized_score3[3];
    CLD2::ResultChunkVector resultchunkvector;
    int text_bytes;

    CLD2::CLDHints cldhints = { NULL, tldhint, enchint, langhint };

    summary_lang = CLD2::DetectLanguageSummaryV2(buffer, n, this->isPlainText(),
            &cldhints, allow_extended_lang, flags, plus_one, language3,
            percent3, normalized_score3,
            NULL, &text_bytes, &is_reliable);

    DetectedLanguage *dl = new DetectedLanguage();
    dl->languageCode = CLD2::LanguageCode(summary_lang);
    dl->languageName = CLD2::LanguageName(summary_lang);
    dl->languageProbability = percent3[0];
    dl->isReliable = is_reliable;

    return *dl;
}

bool CLD2Wrapper::isPlainText()
{
    return this->mIsPlainText;
}

void CLD2Wrapper::setPlainText(bool pt)
{
    this->mIsPlainText = pt;
}
