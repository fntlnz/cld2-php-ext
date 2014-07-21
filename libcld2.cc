#include "php_cld2.h"
#include "cld2wrapper.h"

zend_class_entry *cld2_ce;
zend_object_handlers cld2_object_handlers;

struct cld2_object {
    zend_object std;
    CLD2Wrapper *cld2;
};

void cld2_free_storage(void *object TSRMLS_CC)
{
    cld2_object *obj = (cld2_object *)object;
    delete obj->cld2;

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

zend_object_value cld2_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zval *tmp;
    zend_object_value retval;

    cld2_object *obj = (cld2_object *)emalloc(sizeof(cld2_object));
    memset(obj, 0, sizeof(cld2_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    object_properties_init((zend_object*) &(obj->std.properties), type);
    retval.handle = zend_objects_store_put(obj, NULL,
        cld2_free_storage, NULL TSRMLS_CC);
    retval.handlers = &cld2_object_handlers;

    return retval;
}

/* {{{ CLD2 Methods */
PHP_METHOD(CLD2, __construct)
{
    CLD2Wrapper *cld2 = NULL;
    zval *object = getThis();
    cld2 = new CLD2Wrapper();
    cld2_object *obj = (cld2_object *)zend_object_store_get_object(object TSRMLS_CC);

    // Properties
    add_property_bool(getThis(), "isPlainText", cld2->isPlainText);

    obj->cld2 = cld2;
}

PHP_METHOD(CLD2, detect)
{
    CLD2Wrapper *cld2;
    char *text;
    int text_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &text, &text_len) == FAILURE) {
        RETURN_NULL();
    }

    cld2_object *obj = (cld2_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    cld2 = obj->cld2;
    if (cld2 == NULL) {
        RETURN_NULL();
    }

    // Prepare array
    const char* buffer = strdup(text);
    DetectedLanguage lang = cld2->detect(buffer);
    char* language_code = strdup(lang.language_code);
    char* language_name = strdup(lang.language_name);
    array_init(return_value);
    add_assoc_string(return_value, "language_code", language_code, 1);
    add_assoc_string(return_value, "language_name", language_name, 1);
    add_assoc_long(return_value, "language_accuracy", lang.language_accuracy);
    add_assoc_bool(return_value, "is_reliable", lang.is_reliable);
}

zend_function_entry cld2_methods[] = {
    PHP_ME(CLD2, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(CLD2, detect, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

/* }}} */

PHP_MINIT_FUNCTION(cld2)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "CLD2", cld2_methods);
    cld2_ce = zend_register_internal_class(&ce TSRMLS_CC);
    cld2_ce->create_object = cld2_create_handler;
    memcpy(&cld2_object_handlers,
            zend_get_std_object_handlers(), sizeof(zend_object_handlers));
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
