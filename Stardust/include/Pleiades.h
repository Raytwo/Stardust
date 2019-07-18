#pragma once

#include "types.h"

Result pleiadesInitialize(void);

Result pleiadesWriteBranch(u64 write_address, u64 dest);
Result pleiadesWriteBranchLink(u64 write_address, u64 dest);