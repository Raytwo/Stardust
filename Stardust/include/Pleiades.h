#pragma once

#include "types.h"

Result pleiadesInitialize(void);

Result pleiadesWriteBranch(u64 dest, u64 addr);
Result pleiadesWriteBranchLink(u64 dest, u64 func_addr);