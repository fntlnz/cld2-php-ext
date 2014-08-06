#include "php_cld2.h"
#include "compact_lang_det_impl.h"

zend_class_entry *cld2_detector_ce;
zend_class_entry *cld2_language_ce;
zend_object_handlers cld2_object_handlers;

struct st_detected_language {
    CLD2::Language language;
    int probability;
    bool is_reliable;
    char* language_code;
    char* language_name;
};

st_detected_language detect_language(char *text, int text_len, bool is_plain_text, char *tld_hint)
{
    int flags = 0;
    bool is_reliable;

    // Detect language
    CLD2::Language language3[3];
    int percent3[3];
    double normalized_score3[3];
    int text_bytes;

    //CLD2::CLDHints cld_hints = {NULL, this->getTldHint(), this->getEncHint(), this->getLangHint()};

    CLD2::CLDHints cld_hints = {NULL, tld_hint, 0, CLD2::UNKNOWN_LANGUAGE};
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

PHP_METHOD(cld2_detector, detect)
{
    zval *detector, *is_plain, *tld_hint;
    char *text;
    int text_len;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &detector, cld2_detector_ce, &text, &text_len) == FAILURE) {
        RETURN_NULL();
    }

    is_plain = zend_read_property(cld2_detector_ce, detector, "isPlainText", sizeof("isPlainText") -1, 1 TSRMLS_CC);
    tld_hint = zend_read_property(cld2_detector_ce, detector, "tldHint", sizeof("tldHint") -1, 1 TSRMLS_CC);

    st_detected_language dl = detect_language(text, text_len, (bool) is_plain, (char *) tld_hint);

    // Prepare array
    array_init(return_value);
    add_assoc_string(return_value, "language_code", dl.language_code, 1);
    add_assoc_string(return_value, "language_name", dl.language_name, 1);
    add_assoc_long(return_value, "language_probability", dl.probability);
    add_assoc_bool(return_value, "is_reliable", dl.is_reliable);
}


PHP_METHOD(cld2_detector, isPlainText)
{
    zval *detector, *is_plain;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &detector, cld2_detector_ce) == FAILURE) {
        RETURN_NULL();
    }

    is_plain = zend_read_property(cld2_detector_ce, detector, "isPlainText", sizeof("isPlainText") -1, 1 TSRMLS_CC);

    RETVAL_ZVAL(is_plain, 1, 0);
}

PHP_METHOD(cld2_detector, setPlainText)
{
    zval *detector;
    zend_bool is_plain;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ob", &detector, cld2_detector_ce, &is_plain) == FAILURE) {
        RETURN_NULL();
    }

    zend_update_property_bool(cld2_detector_ce, detector, "isPlainText", sizeof("isPlainText") -1, is_plain TSRMLS_CC);
}

PHP_METHOD(cld2_detector, getTldHint)
{
    zval *detector, *tld_hint;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &detector, cld2_detector_ce) == FAILURE) {
        RETURN_NULL();
    }

    tld_hint = zend_read_property(cld2_detector_ce, detector, "tldHint", sizeof("tldHint") - 1, 1 TSRMLS_CC);

    RETVAL_ZVAL(tld_hint, 1, 0);
}

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

zend_function_entry cld2_methods[] = {
    PHP_ME(cld2_detector, detect, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, isPlainText, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, setPlainText, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, getTldHint, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, setTldHint, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(cld2)
{
    // Detector
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "CLD2Detector", cld2_methods);
    cld2_detector_ce = zend_register_internal_class(&ce TSRMLS_CC);
    zend_declare_property_bool(cld2_detector_ce, "isPlainText", sizeof("isPlainText") - 1 , 0, ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_string(cld2_detector_ce, "tldHint", sizeof("tldHint") - 1, "", ZEND_ACC_PUBLIC TSRMLS_CC);

    // Language
    zend_class_entry ce_Language;
    INIT_CLASS_ENTRY(ce_Language, "CLD2Language", NULL);
    cld2_language_ce = zend_register_internal_class(&ce_Language TSRMLS_CC);
 
    memcpy(&cld2_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    cld2_object_handlers.clone_obj = NULL;

    return SUCCESS;
}

zend_module_entry cld2_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_CLD2_EXTNAME,
    NULL,                  /* Functions */
    PHP_MINIT(cld2),
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
