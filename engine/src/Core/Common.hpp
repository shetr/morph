
#ifndef MORPH_COMMON_HPP
#define MORPH_COMMON_HPP

#include <stdlib.h>
#include <stdint.h>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <optional>
#include <variant>
#include <tuple>
#include <any>
#include <initializer_list>
#include <thread>
#include <mutex>

#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <spdlog/spdlog.h>
#include "spdlog/fmt/ostr.h"

//#define NAMEOF_ENUM_RANGE_MIN (INT16_MIN+1)
//#define NAMEOF_ENUM_RANGE_MAX (INT16_MAX-1)
#include <Extern/nameof.hpp>
//#define MAGIC_ENUM_RANGE_MIN (INT16_MIN+1)
//#define MAGIC_ENUM_RANGE_MAX (INT16_MAX-1)
#include <Extern/magic_enum.hpp>

#endif // MORPH_COMMON_HPP