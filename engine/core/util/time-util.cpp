#include "time-util.h"

namespace Boo {

long long TimeUtil::nowTime()
{
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return ms.count();  
};

} // namespace Boo