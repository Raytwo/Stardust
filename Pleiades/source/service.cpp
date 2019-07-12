#include <switch.h>
#include "service.hpp"

Result Pleiades::GetArbitraryValue(Out<u32> number) {
	number.SetValue(0x69);
	
	return ResultSuccess;
}