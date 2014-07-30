dnl config.m4 for extension cld2

PHP_ARG_WITH(cld2, whether to enable cld2 support,
[  --with-cld2[=DIR]         Include cld2 support])


if test "$PHP_CLD2" != "no"; then

    if test "$PHP_CLD2" = "yes"; then
        AC_MSG_ERROR([cld2 path not provided])
    fi

    if ! test -r $PHP_CLD2/internal/compact_lang_det_impl.h; then
        AC_MSG_ERROR([cld2 not found at provided path])
    fi


    PHP_REQUIRE_CXX()

    PHP_ADD_INCLUDE($PHP_CLD2/internal)
    PHP_ADD_LIBRARY(stdc++, 1, CLD2_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(cld2, $PHP_CLD2/internal, CLD2_SHARED_LIBADD)
    PHP_SUBST(CLD2_SHARED_LIBADD)
    
    PHP_NEW_EXTENSION(cld2, cld2_object.cc cld2_wrapper.cc, $ext_shared,,,1)
fi
