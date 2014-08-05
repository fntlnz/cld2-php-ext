#include "php_cld2.h"
#include "compact_lang_det_impl.h"

zend_class_entry *cld2_detector_ce;
zend_object_handlers cld2_object_handlers;


PHP_METHOD(cld2_detector, __construct)
{

}


PHP_METHOD(cld2_detector, detect)
{
    zval *detector, *is_plain;
    char *text;
    int text_len;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &detector, cld2_detector_ce, &text, &text_len) == FAILURE) {
        RETURN_NULL();
    }

    is_plain = zend_read_property(cld2_detector_ce, detector, "isPlainText", sizeof("isPlainText") -1, 1 TSRMLS_CC);

    int flags = 0;
    bool is_reliable;

    // Detect language
    CLD2::Language language3[3];
    int percent3[3];
    double normalized_score3[3];
    int text_bytes;

    //CLD2::CLDHints cld_hints = {NULL, this->getTldHint(), this->getEncHint(), this->getLangHint()};

    CLD2::CLDHints cld_hints = {NULL, "", 0, CLD2::UNKNOWN_LANGUAGE};
    CLD2::Language detectedLanguage = CLD2::ExtDetectLanguageSummary(
            text,
            text_len,
            (bool) is_plain,
            &cld_hints,
            flags,
            language3,
            percent3,
            normalized_score3,
            NULL,
            &text_bytes,
            &is_reliable);


    // Prepare array
    char* language_code = strdup(CLD2::LanguageCode(detectedLanguage));
    char* language_name = strdup(CLD2::LanguageName(detectedLanguage));
    array_init(return_value);
    add_assoc_string(return_value, "language_code", language_code, 1);
    add_assoc_string(return_value, "language_name", language_name, 1);
    add_assoc_long(return_value, "language_probability", percent3[0]);
    add_assoc_bool(return_value, "is_reliable", is_reliable);
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

//PHP_METHOD(cld2_detector, setTldHint)
//{
//    char *hint;
//    int hint_len;
//
//    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hint, &hint_len) == FAILURE) {
//        RETURN_NULL();
//    }
//
//    CLD2Detector *detector;
//    cld2 *obj = (cld2 *)zend_object_store_get_object(
//            getThis() TSRMLS_CC);
//    detector = obj->detector;
//
//    if (detector == NULL) {
//        RETURN_NULL();
//    }
//
//    detector->setTldHint(hint);
//}
//
//PHP_METHOD(cld2_detector, getEncHint)
//{
//    CLD2Detector *detector;
//    cld2 *obj = (cld2 *)zend_object_store_get_object(getThis() TSRMLS_CC);
//    detector = obj->detector;
//    if (detector == NULL) {
//        RETURN_NULL();
//    }
//
//    RETURN_LONG(detector->getEncHint());
//}
//
//PHP_METHOD(cld2_detector, setEncHint)
//{
//    long hint;
//
//    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &hint) == FAILURE) {
//        RETURN_NULL();
//    }
//
//    CLD2Detector *detector;
//    cld2 *obj = (cld2 *) zend_object_store_get_object(getThis() TSRMLS_CC);
//    detector = obj->detector;
//
//    if (detector == NULL) {
//        RETURN_NULL();
//    }
//
//    detector->setEncHint(hint);
//
//}
//
//// TODO: complete this method
//PHP_METHOD(cld2_detector, getLangHint)
//{
//}
//
//// TODO: complete this method
//PHP_METHOD(cld2_detector, setLangHint)
//{
//}

zend_function_entry cld2_methods[] = {
    PHP_ME(cld2_detector, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(cld2_detector, detect, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, isPlainText, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, setPlainText, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, getTldHint, NULL, ZEND_ACC_PUBLIC)
//    PHP_ME(cld2_detector, setTldHint, NULL, ZEND_ACC_PUBLIC)
//    PHP_ME(cld2_detector, getEncHint, NULL, ZEND_ACC_PUBLIC)
//    PHP_ME(cld2_detector, setEncHint, NULL, ZEND_ACC_PUBLIC)
//    PHP_ME(cld2_detector, getLangHint, NULL, ZEND_ACC_PUBLIC)
//    PHP_ME(cld2_detector, setLangHint, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(cld2)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "CLD2Detector", cld2_methods);
    cld2_detector_ce = zend_register_internal_class(&ce TSRMLS_CC);
    zend_declare_property_bool(cld2_detector_ce, "isPlainText", sizeof("isPlainText") - 1 , 0, ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_string(cld2_detector_ce, "tldHint", sizeof("tldHint") - 1, "", ZEND_ACC_PUBLIC TSRMLS_CC);
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
