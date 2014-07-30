#ifndef PHP_CLD2_H
#define PHP_CLD2_H

#define PHP_CLD2_EXTNAME  "cld2"
#define PHP_CLD2_EXTVER   "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

extern zend_module_entry cld2_module_entry;
#define phpext_cld2_ptr &cld2_module_entry;

#endif /* PHP_CLD2_H */
