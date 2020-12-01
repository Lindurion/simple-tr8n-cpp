#ifndef SIMPLE_TR8N_STRING_VIEW_HPP
#define SIMPLE_TR8N_STRING_VIEW_HPP

#ifdef SIMPLE_TR8N_STRING_VIEW_INCLUDES
SIMPLE_TR8N_STRING_VIEW_INCLUDES
#endif

namespace simple_tr8n {

#ifndef SIMPLE_TR8N_STRING_VIEW_TYPE

#error "SIMPLE_TR8N_STRING_VIEW_TYPE must be defined"

#else

/** Type alias for std::string_view or an equivalent implementation. */
using string_view = SIMPLE_TR8N_STRING_VIEW_TYPE;

#endif  // SIMPLE_TR8N_STRING_VIEW_TYPE

}  // namespace simple_tr8n

#endif  // SIMPLE_TR8N_STRING_VIEW_HPP
