#include "unionfs.h"

#include <vector>
#include <assert.h>

namespace UnionFS {

static std::vector<LPVOID> services;

static BOOL RegisterService(Service svc, LPVOID obj) 
{
	if (services[svc] != NULL)
		return FALSE;
	services[svc] = obj;
	return TRUE;
}

static BOOL UnregisterService(Service svc, LPVOID obj)
{
	if (services[svc] == NULL)
		return FALSE;
	services[svc] = NULL;
	return TRUE;
}

LPVOID GetService(Service svc)
{
	return services[svc];
}

ServiceRegister::ServiceRegister(Service svc, LPVOID obj)
{
	assert(RegisterService(svc, obj));
	_svc = svc;
	_obj = obj;
}

ServiceRegister::~ServiceRegister()
{
	assert(UnregisterService(_svc, _obj));
}

}