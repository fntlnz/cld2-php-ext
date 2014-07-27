dnl config.m4 for extension cld2

PHP_ARG_WITH(cld2, whether to enable cld2 support,
[  --enable-cld2             Include cld2 support])

if test "$PHP_CLD2" != "no"; then
    
    LIBCLD2_DIR=libcld2/internal
    
    if test -z "$LIBCLD2_DIR"; then
        AC_MSG_RESULT([not found])
        AC_MSG_ERROR([Please add libcld2])
    fi
    
    PHP_REQUIRE_CXX()
    
    PHP_ADD_LIBRARY(stdc++, 1, CLD2_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(cld2, $LIBCLD2_DIR, CLD2_SHARED_LIBADD)
    PHP_SUBST(CLD2_SHARED_LIBADD)
    
    PHP_NEW_EXTENSION(cld2, cld2_object.cc cld2_wrapper.cc, $ext_shared,,,1)
fi
