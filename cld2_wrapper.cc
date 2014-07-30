#include "cld2_wrapper.h"

CLD2Wrapper::CLD2Wrapper() {
    this->plainText = false;
    this->tldHint = "";
    this->langHint = CLD2::UNKNOWN_LANGUAGE;
}

DetectedLanguage CLD2Wrapper::detect(const char *&buffer) {
    int flags = 0;
    bool isReliable;
    int n = strlen(buffer);

    // Detect language
    CLD2::Language language3[3];
    int percent3[3];
    double normalizedScore3[3];
    int textBytes;

    CLD2::CLDHints cldHints = {NULL, this->getTldHint(), this->getEncHint(), this->getLangHint()};

    CLD2::Language detectedLanguage = CLD2::ExtDetectLanguageSummary(
            buffer,
            n,
            this->isPlainText(),
            &cldHints,
            flags,
            language3,
            percent3,
            normalizedScore3,
            NULL,
            &textBytes,
            &isReliable);

    DetectedLanguage *dl = new DetectedLanguage();
    dl->languageCode = CLD2::LanguageCode(detectedLanguage);
    dl->languageName = CLD2::LanguageName(detectedLanguage);
    dl->languageProbability = percent3[0];
    dl->isReliable = isReliable;

    return *dl;
}

void CLD2Wrapper::setPlainText(bool pt) {
    this->plainText = pt;
}

void CLD2Wrapper::setTldHint(const char *tldHint) {
    this->tldHint = tldHint;
}

void CLD2Wrapper::setEncHint(int encHint) {
    this->encHint = encHint;
}

void CLD2Wrapper::setLangHintFromLanguageName(const char *languageName) {
    this->setLangHint(CLD2::GetLanguageFromName(languageName));
}

void CLD2Wrapper::setLangHint(CLD2::Language langHint) {
    this->langHint = langHint;
}

bool CLD2Wrapper::isPlainText() {
    return false;
}

char const *CLD2Wrapper::getTldHint() {
    return this->tldHint;
}

int CLD2Wrapper::getEncHint() {
    return this->encHint;
}

CLD2::Language const &CLD2Wrapper::getLangHint() {
    return this->langHint;
}
