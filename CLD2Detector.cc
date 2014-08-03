#include "CLD2Detector.h"

CLD2Detector::CLD2Detector() {
    this->plainText = false;
    this->tldHint = "";
    this->langHint = CLD2::UNKNOWN_LANGUAGE;
    this->encHint = UNKNOWN_ENCODING;
}

DetectedLanguage CLD2Detector::detect(const char *&buffer) {
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

void CLD2Detector::setPlainText(bool pt) {
    this->plainText = pt;
}

void CLD2Detector::setTldHint(const char *tldHint) {
    this->tldHint = tldHint;
}

void CLD2Detector::setEncHint(int encHint) {
    this->encHint = encHint;
}

void CLD2Detector::setLangHint(CLD2::Language langHint) {
    this->langHint = langHint;
}

bool CLD2Detector::isPlainText() {
    return this->plainText;
}

char const *CLD2Detector::getTldHint() {
    return this->tldHint;
}

int CLD2Detector::getEncHint() {
    return this->encHint;
}

CLD2::Language const &CLD2Detector::getLangHint() {
    return this->langHint;
}
