#pragma once

#include "types.h"

Result pleiadesInitialize(u64 base_address);

Result pleiadesWriteBranch(u64 write_address, u64 dest);
Result pleiadesWriteBranchLink(u64 write_address, u64 dest);