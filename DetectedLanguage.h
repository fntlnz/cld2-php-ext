#ifndef __DetectedLanguage_H_
#define __DetectedLanguage_H_

class DetectedLanguage {
public:
    const char *languageName;
    const char *languageCode;
    int languageProbability;
    bool isReliable;
};

#endif //__DetectedLanguage_H_
