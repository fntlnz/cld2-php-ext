#include "php_cld2.h"
#include "internal/compact_lang_det_impl.h"
#include "encodings.h"
#include "public/encodings.h"

zend_class_entry *cld2_detector_ce;
zend_class_entry *cld2_language_ce;
zend_class_entry *cld2_encoding_ce;
zend_object_handlers cld2_object_handlers;

struct st_detected_language {
    CLD2::Language language;
    int probability;
    bool is_reliable;
    char* language_code;
    char* language_name;
};

st_detected_language detect_language(char *text, int text_len, bool is_plain_text, char *tld_hint, long language_hint, long encoding_hint)
{
    int flags = 0;
    bool is_reliable;

    // Detect language
    CLD2::Language language3[3];
    int percent3[3];
    double normalized_score3[3];
    int text_bytes;

    CLD2::CLDHints cld_hints = {NULL, tld_hint, (CLD2::Encoding) encoding_hint, (CLD2::Language) language_hint};
    CLD2::Language lang = CLD2::ExtDetectLanguageSummary(
                text,
                text_len,
                is_plain_text,
                &cld_hints,
                flags,
                language3,
                percent3,
                normalized_score3,
                NULL,
                &text_bytes,
                &is_reliable);

    st_detected_language dl;
    dl.language = lang;
    dl.probability = percent3[0];
    dl.is_reliable = is_reliable;
    dl.language_code = strdup(CLD2::LanguageCode(lang));
    dl.language_name = strdup(CLD2::LanguageName(lang));

    return dl;
}

bool check_language(CLD2::Language lang)
{
    return !(lang < 0 || lang > CLD2::NUM_LANGUAGES);
}

bool check_encoding(long encoding)
{
    return !(encoding < 0 || encoding > CLD2::NUM_ENCODINGS);
}

/* ============================================ */
/* Detector                                     */
/* ============================================ */

// CLD2Detector->detect(string $text)
PHP_METHOD(cld2_detector, detect)
{
    zval *detector, *is_plain, *tld_hint, *language_hint, *encoding_hint;
    char *text;
    int text_len;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &detector, cld2_detector_ce, &text, &text_len) == FAILURE) {
        RETURN_NULL();
    }

    is_plain = zend_read_property(cld2_detector_ce, detector, "isPlainText", sizeof("isPlainText") - 1, 1 TSRMLS_CC);
    tld_hint = zend_read_property(cld2_detector_ce, detector, "tldHint", sizeof("tldHint") - 1, 1 TSRMLS_CC);
    language_hint = zend_read_property(cld2_detector_ce, detector, "languageHint", sizeof("languageHint") - 1, 1 TSRMLS_CC);
    encoding_hint = zend_read_property(cld2_detector_ce, detector, "encodingHint", sizeof("encodingHint") - 1, 1 TSRMLS_CC);

    st_detected_language dl = detect_language(text, text_len, (bool) is_plain, (char *) tld_hint, (long) language_hint, (long) encoding_hint);


    // Prepare array
    array_init(return_value);
    add_assoc_long(return_value, "language_id", (int) (dl.language));
    add_assoc_string(return_value, "language_code", dl.language_code, 1);
    add_assoc_string(return_value, "language_name", dl.language_name, 1);
    add_assoc_long(return_value, "language_probability", dl.probability);
    add_assoc_bool(return_value, "is_reliable", dl.is_reliable);
}

// CLD2Detector->isPlainText()
PHP_METHOD(cld2_detector, isPlainText)
{
    zval *detector, *is_plain;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &detector, cld2_detector_ce) == FAILURE) {
        RETURN_NULL();
    }

    is_plain = zend_read_property(cld2_detector_ce, detector, "isPlainText", sizeof("isPlainText") -1, 1 TSRMLS_CC);

    RETVAL_ZVAL(is_plain, 1, 0);
}

// CLD2Detector->setPlainText(bool $isPlainText)
PHP_METHOD(cld2_detector, setPlainText)
{
    zval *detector;
    zend_bool is_plain;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ob", &detector, cld2_detector_ce, &is_plain) == FAILURE) {
        RETURN_NULL();
    }

    zend_update_property_bool(cld2_detector_ce, detector, "isPlainText", sizeof("isPlainText") -1, is_plain TSRMLS_CC);
}

// CLD2Detector->getTldHint()
PHP_METHOD(cld2_detector, getTldHint)
{
    zval *detector, *tld_hint;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &detector, cld2_detector_ce) == FAILURE) {
        RETURN_NULL();
    }

    tld_hint = zend_read_property(cld2_detector_ce, detector, "tldHint", sizeof("tldHint") - 1, 1 TSRMLS_CC);

    RETVAL_ZVAL(tld_hint, 1, 0);
}

// CLD2Detector->setTldHint(string $hint)
PHP_METHOD(cld2_detector, setTldHint)
{
    zval *detector;

    char *hint;
    int hint_len;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &detector, cld2_detector_ce, &hint, &hint_len) == FAILURE) {
        RETURN_NULL();
    }

    zend_update_property_string(cld2_detector_ce, detector, "tldHint", sizeof("tldHint") - 1, hint TSRMLS_CC);
}

// CLD2Detector->getLanguageHint()
PHP_METHOD(cld2_detector, getLanguageHint)
{
    zval *detector, *languageHint;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &detector, cld2_detector_ce) == FAILURE) {
        RETURN_NULL();
    }

    languageHint = zend_read_property(cld2_detector_ce, detector, "languageHint", sizeof("languageHint") - 1, 1 TSRMLS_CC);

    RETVAL_ZVAL(languageHint, 1, 0);
}


// CLD2Detector->setLanguageHint(int $hint)
PHP_METHOD(cld2_detector, setLanguageHint)
{
    zval *detector;
    long hint;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &detector, cld2_detector_ce, &hint) == FAILURE) {
        RETURN_NULL();
    }

    zend_update_property_long(cld2_detector_ce, detector, "languageHint", sizeof("languageHint") - 1, hint TSRMLS_CC);
}

// CLD2Detector->setEncodingHint(int $hint)
PHP_METHOD(cld2_detector, setEncodingHint)
{
    zval *detector;
    long hint;
    
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &detector, cld2_detector_ce, &hint) == FAILURE) {
        RETURN_NULL();
    }

    zend_update_property_long(cld2_detector_ce, detector, "encodingHint", sizeof("encodingHint") - 1, hint TSRMLS_CC);
}

// CLD2Detector->getEncodingHint()
PHP_METHOD(cld2_detector, getEncodingHint)
{
    zval *detector, *encodingHint;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &detector, cld2_detector_ce) == FAILURE) {
        RETURN_NULL();
    }

    encodingHint = zend_read_property(cld2_detector_ce, detector, "encodingHint", sizeof("encodingHint") -1, 1 TSRMLS_CC);

    RETVAL_ZVAL(encodingHint, 1, 0);
}

zend_function_entry cld2_methods[] = {
    PHP_ME(cld2_detector, detect, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, isPlainText, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, setPlainText, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, getTldHint, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, setTldHint, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, getLanguageHint, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, setLanguageHint, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, getEncodingHint, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, setEncodingHint, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};


/* ============================================ */
/* Language                                     */
/* ============================================ */

//CLD2Language::languageName(int $language)
PHP_METHOD(cld2_language, languageName)
{
    long language;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &language) == FAILURE) {
        RETURN_NULL();
    }

    CLD2::Language l = (CLD2::Language) (language);

    if (!check_language(l)) {
        //TODO: Handle this error
    };


    RETURN_STRING(CLD2::LanguageName(l), 1);
}

//CLD2Language::languageCode(int $language)
PHP_METHOD(cld2_language, languageCode)
{
    long language;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &language) == FAILURE) {
        RETURN_NULL();
    }

    CLD2::Language l = (CLD2::Language) (language);

    if (!check_language(l)) {
        // TODO: handle this case
    };

    RETURN_STRING(CLD2::LanguageCode(l), 1);
}

// CLD2Language::getLanguageFromName(string name)
PHP_METHOD(cld2_language, getLanguageFromName)
{
    char *name;
    int name_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
        RETURN_NULL();
    }

    RETURN_LONG(CLD2::GetLanguageFromName(name));
}

zend_function_entry cld2_language_methods[] = {
        PHP_ME(cld2_language, languageName, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        PHP_ME(cld2_language, languageCode, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        PHP_ME(cld2_language, getLanguageFromName, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        {NULL, NULL, NULL}
};


/* ============================================ */
/* Encoding                                     */
/* ============================================ */

// CLD2Encoding::encodingName(int $encoding)
PHP_METHOD(cld2_encoding, encodingName)
{
    long encoding;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &encoding) == FAILURE) {
        RETURN_NULL();
    }

    if (!check_encoding(encoding)) {
       RETURN_STRING(encodingStrings[CLD2::UNKNOWN_ENCODING], 1);
    }

    RETURN_STRING(encodingStrings[encoding], 1);
    
}

zend_function_entry cld2_encoding_methods[] = {
        PHP_ME(cld2_encoding, encodingName, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        {NULL, NULL, NULL}
};

// MINIT
PHP_MINIT_FUNCTION(cld2)
{
    // Detector
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "CLD2Detector", cld2_methods);
    cld2_detector_ce = zend_register_internal_class(&ce TSRMLS_CC);
    zend_declare_property_bool(cld2_detector_ce, "isPlainText", sizeof("isPlainText") - 1 , 0, ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_string(cld2_detector_ce, "tldHint", sizeof("tldHint") - 1, "", ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_long(cld2_detector_ce, "languageHint", sizeof("languageHint") - 1, CLD2::UNKNOWN_LANGUAGE, ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_long(cld2_detector_ce, "encodingHint", sizeof("encodingHint") - 1, CLD2::UNKNOWN_ENCODING, ZEND_ACC_PUBLIC TSRMLS_CC);

    // Language
    zend_class_entry ce_Language;
    INIT_CLASS_ENTRY(ce_Language, "CLD2Language", cld2_language_methods);
    cld2_language_ce = zend_register_internal_class(&ce_Language TSRMLS_CC);

    for (int i = 0; i < CLD2::NUM_LANGUAGES; i++) {
        CLD2::Language lan = (CLD2::Language)(i);
        zend_declare_class_constant_long(cld2_language_ce, CLD2::LanguageName(lan),  strlen(CLD2::LanguageName(lan)), i TSRMLS_CC);
    }

    // Encoding
    zend_class_entry ce_Encoding;
    INIT_CLASS_ENTRY(ce_Encoding, "CLD2Encoding", cld2_encoding_methods);
    cld2_encoding_ce = zend_register_internal_class(&ce_Encoding TSRMLS_CC);

    for (int i = 0; i < CLD2::NUM_ENCODINGS; i++) {
        CLD2::Encoding enc = (CLD2::Encoding) (i);
        zend_declare_class_constant_long(cld2_encoding_ce, encodingStrings[i], strlen(encodingStrings[i]), i TSRMLS_CC);
    }

    memcpy(&cld2_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    cld2_object_handlers.clone_obj = NULL;

    return SUCCESS;
}

zend_module_entry cld2_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_CLD2_EXTNAME,
    NULL,                  /* Functions */
    PHP_MINIT(cld2),       /* MINIT */
    NULL,                  /* MSHUTDOWN */
    NULL,                  /* RINIT */
    NULL,                  /* RSHUTDOWN */
    NULL,                  /* MINFO */
    PHP_CLD2_EXTVER,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_CLD2
extern "C" {
ZEND_GET_MODULE(cld2)
}
#endif
