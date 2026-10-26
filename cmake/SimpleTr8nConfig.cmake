# SPDX-FileCopyrightText: 2022 Eric Barndollar
#
# SPDX-License-Identifier: Apache-2.0

# Configure build based on defaults or client specified options.

# C++ exceptions support:
option(SIMPLE_TR8N_ENABLE_EXCEPTIONS "Enables C++ exceptions for SimpleTr8n" ON)
mark_as_advanced(SIMPLE_TR8N_ENABLE_EXCEPTIONS)
