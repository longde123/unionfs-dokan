#include "windows.h"
#include "dokan.h"

namespace UnionFS {

struct Operations : public DOKAN_OPERATIONS {
	Operations();
};

}