// SPDX-FileCopyrightText: 2022 Eric Barndollar
//
// SPDX-License-Identifier: Apache-2.0

#ifndef SIMPLE_TR8N_STRING_VIEW_HPP
#define SIMPLE_TR8N_STRING_VIEW_HPP

// Include necessary header for the implementation being aliased.
#ifdef SIMPLE_TR8N_STRING_VIEW_INCLUDE
#include SIMPLE_TR8N_STRING_VIEW_INCLUDE
#endif

namespace simple_tr8n {

#ifndef SIMPLE_TR8N_BASIC_STRING_VIEW_TYPE

#error "SIMPLE_TR8N_BASIC_STRING_VIEW_TYPE must be defined"

#else

/** Type alias for std::basic_string_view or an equivalent implementation. */
template<typename CharT>
using basic_string_view = SIMPLE_TR8N_BASIC_STRING_VIEW_TYPE<CharT>;

#endif  // SIMPLE_TR8N_BASIC_STRING_VIEW_TYPE

}  // namespace simple_tr8n

#endif  // SIMPLE_TR8N_STRING_VIEW_HPP
