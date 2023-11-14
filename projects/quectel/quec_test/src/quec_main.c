
#include "quec_apimf.h"

#define CFG_ENABLE_QUECTEL_ST 	0

void ql_apimf_main(){
#if CFG_ENABLE_QUECTEL_ST 	
	quecst_apimf_start();
#endif
	return;
}


