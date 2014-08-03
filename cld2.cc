#include "php_cld2.h"
#include "CLD2Detector.h"

zend_class_entry *cld2_detector_ce;
zend_object_handlers cld2_object_handlers;

struct cld2 {
    zend_object std;
    CLD2Detector *detector;
};

void cld2_free_storage(void *object TSRMLS_CC)
{
    cld2 *obj = (cld2 *)object;
    delete obj->detector;

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

zend_object_value cld2_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zend_object_value retval;

    cld2 *obj = (cld2 *)emalloc(sizeof(cld2));
    memset(obj, 0, sizeof(cld2));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    object_properties_init((zend_object*) &(obj->std.properties), type);
    retval.handle = zend_objects_store_put(obj, NULL,
        cld2_free_storage, NULL TSRMLS_CC);
    retval.handlers = &cld2_object_handlers;

    return retval;
}

PHP_METHOD(cld2_detector, __construct)
{
    CLD2Detector *detector = NULL;
    zval *object = getThis();
    detector = new CLD2Detector();
    cld2 *obj = (cld2 *)zend_object_store_get_object(object TSRMLS_CC);
    obj->detector = detector;
}

PHP_METHOD(cld2_detector, detect)
{
    CLD2Detector *detector;
    char *text;
    int text_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &text, &text_len) == FAILURE) {
        RETURN_NULL();
    }

    cld2 *obj = (cld2 *)zend_object_store_get_object(getThis() TSRMLS_CC);

    detector = obj->detector;

    if (detector == NULL) {
        RETURN_NULL();
    }

    // Prepare array
    const char* buffer = strdup(text);
    DetectedLanguage lang = detector->detect(buffer);
    char* language_code = strdup(lang.languageCode);
    char* language_name = strdup(lang.languageName);
    array_init(return_value);
    add_assoc_string(return_value, "language_code", language_code, 1);
    add_assoc_string(return_value, "language_name", language_name, 1);
    add_assoc_long(return_value, "language_probability", lang.languageProbability);
    add_assoc_bool(return_value, "is_reliable", lang.isReliable);
}

PHP_METHOD(cld2_detector, isPlainText)
{
    CLD2Detector *detector;
    cld2 *obj = (cld2 *)zend_object_store_get_object(getThis() TSRMLS_CC);
    detector = obj->detector;
    if (detector == NULL) {
        RETURN_NULL();
    }
    RETURN_BOOL(detector->isPlainText());
};

PHP_METHOD(cld2_detector, setPlainText)
{
    bool isPlainText;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &isPlainText) == FAILURE) {
        RETURN_NULL();
    }

    CLD2Detector *detector;
    cld2 *obj = (cld2 *)zend_object_store_get_object(
            getThis() TSRMLS_CC);
    detector = obj->detector;

    if (detector == NULL) {
        RETURN_NULL();
    }

    detector->setPlainText(isPlainText);
};

PHP_METHOD(cld2_detector, getTldHint)
{
    CLD2Detector *detector;
    cld2 *obj = (cld2 *)zend_object_store_get_object(getThis() TSRMLS_CC);
    detector = obj->detector;
    if (detector == NULL) {
        RETURN_NULL();
    }
    RETURN_STRING(detector->getTldHint(), 1);
};

PHP_METHOD(cld2_detector, setTldHint)
{
    char *hint;
    int hint_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hint, &hint_len) == FAILURE) {
        RETURN_NULL();
    }

    CLD2Detector *detector;
    cld2 *obj = (cld2 *)zend_object_store_get_object(
            getThis() TSRMLS_CC);
    detector = obj->detector;

    if (detector == NULL) {
        RETURN_NULL();
    }

    detector->setTldHint(hint);
};

// TODO: complete this method
PHP_METHOD(cld2_detector, getEncHint)
{
}

// TODO: complete this method
PHP_METHOD(cld2_detector, setEncHint)
{
}

// TODO: complete this method
PHP_METHOD(cld2_detector, getLangHint)
{
}

// TODO: complete this method
PHP_METHOD(cld2_detector, setLangHint)
{
}

zend_function_entry cld2_methods[] = {
    PHP_ME(cld2_detector, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(cld2_detector, detect, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, isPlainText, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, setPlainText, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, getTldHint, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, setTldHint, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, getEncHint, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, setEncHint, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, getLangHint, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(cld2_detector, setLangHint, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(cld2)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "CLD2", "Detector", cld2_methods);
    cld2_detector_ce = zend_register_internal_class(&ce TSRMLS_CC);
    cld2_detector_ce->create_object = cld2_create_handler;
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
