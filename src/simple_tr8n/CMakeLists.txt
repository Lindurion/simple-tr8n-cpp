include(SimpleTr8nTargets)

# StringView: alias for std::string_view or an equivalent type.
simple_tr8n_header_library(StringView string_view.hpp)

target_compile_definitions(SimpleTr8n_StringView INTERFACE
    "SIMPLE_TR8N_STRING_VIEW_TYPE=${SIMPLE_TR8N_STRING_VIEW_TYPE}")
if(SIMPLE_TR8N_STRING_VIEW_INCLUDES)
  target_compile_definitions(SimpleTr8n_StringView INTERFACE
      "SIMPLE_TR8N_STRING_VIEW_INCLUDES=${SIMPLE_TR8N_STRING_VIEW_INCLUDES}")
endif()
if(SIMPLE_TR8N_STRING_VIEW_TARGET)
  target_link_libraries(SimpleTr8n_StringView INTERFACE "${SIMPLE_TR8N_STRING_VIEW_TARGET}")
endif()

# API: interface for SimpleTr8n project.
simple_tr8n_header_library(API translator.hpp)
target_link_libraries(SimpleTr8n_API INTERFACE SimpleTr8n::StringView Microsoft.GSL::GSL)

# SimpleTranslator: simple implementation of the API.
simple_tr8n_header_library(SimpleTranslator simple_translator.hpp)
if(SIMPLE_TR8N_ENABLE_EXCEPTIONS)
  target_sources(SimpleTr8n_SimpleTranslator INTERFACE exceptions.hpp)
  target_compile_definitions(SimpleTr8n_SimpleTranslator INTERFACE "SIMPLE_TR8N_ENABLE_EXCEPTIONS")
endif()
target_link_libraries(SimpleTranslator INTERFACE SimpleTr8n::API SimpleTr8n::StringView)

# TODO: Add test.
