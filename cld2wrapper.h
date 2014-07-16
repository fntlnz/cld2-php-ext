#include <stdlib.h>
#include <stdio.h>

#include "libcld2/internal/cld2tablesummary.h"
#include "libcld2/internal/compact_lang_det_impl.h"
#include "libcld2/internal/debug.h"
#include "libcld2/internal/integral_types.h"
#include "libcld2/internal/lang_script.h"
#include "libcld2/internal/utf8statetable.h"



#ifndef CLD2_CLASS_H
#define CLD2_CLASS_H


// Scaffolding
static const int UNKNOWN_ENCODING = 0;

#ifndef CLD2_DYNAMIC_MODE
extern const CLD2::UTF8PropObj cld_generated_CjkUni_obj;
extern const CLD2::CLD2TableSummary kCjkDeltaBi_obj;
extern const CLD2::CLD2TableSummary kDistinctBiTable_obj;
extern const CLD2::CLD2TableSummary kQuad_obj;
extern const CLD2::CLD2TableSummary kDeltaOcta_obj;
extern const CLD2::CLD2TableSummary kDistinctOcta_obj;
extern const CLD2::CLD2TableSummary kOcta2_obj;
extern const short kAvgDeltaOctaScore[];
#endif 



class CLD2Wrapper
{
    public:
        CLD2Wrapper();
        const char* detect(const char* &buffer);
};

#endif
