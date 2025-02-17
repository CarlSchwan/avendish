find_path(PD_HEADER NAMES m_pd.h)

function(avnd_make_pd)
  if(NOT PD_HEADER)
    return()
  endif()

  cmake_parse_arguments(AVND "" "TARGET;MAIN_FILE;MAIN_CLASS;C_NAME" "" ${ARGN})

  string(MAKE_C_IDENTIFIER "${AVND_MAIN_CLASS}" MAIN_OUT_FILE)

  configure_file(
    src/pd/prototype.cpp.in
    "${CMAKE_BINARY_DIR}/${MAIN_OUT_FILE}_pd.cpp"
    @ONLY
    NEWLINE_STYLE LF
  )

  set(AVND_FX_TARGET "${AVND_TARGET}_pd")
  add_library(${AVND_FX_TARGET} SHARED)

  set_target_properties(
    ${AVND_FX_TARGET}
    PROPERTIES
      OUTPUT_NAME "${AVND_C_NAME}"
      LIBRARY_OUTPUT_DIRECTORY pd
      RUNTIME_OUTPUT_DIRECTORY pd
  )

  target_sources(
    ${AVND_FX_TARGET}
    PRIVATE
      "${CMAKE_BINARY_DIR}/${MAIN_OUT_FILE}_pd.cpp"
  )

  target_compile_definitions(
    ${AVND_FX_TARGET}
    PRIVATE
      AVND_PUREDATA=1
  )

  target_link_libraries(
    ${AVND_FX_TARGET}
    PRIVATE
      Avendish::Avendish_pd
      DisableExceptions
  )

  if(APPLE)
    set_target_properties(${AVND_FX_TARGET} PROPERTIES SUFFIX ".pd_darwin")
    target_link_libraries(${AVND_FX_TARGET} PRIVATE -Wl,-undefined,dynamic_lookup)
  elseif(UNIX)
    if("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm")
      set_target_properties(${AVND_FX_TARGET} PROPERTIES SUFFIX ".l_arm")
    elseif("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "x86_64")
      set_target_properties(${AVND_FX_TARGET} PROPERTIES SUFFIX ".l_ia64")
    elseif("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "x86")
      set_target_properties(${AVND_FX_TARGET} PROPERTIES SUFFIX ".l_i386")
    else()
      set_target_properties(${AVND_FX_TARGET} PROPERTIES SUFFIX ".pd_linux")
    endif()
  endif()

  avnd_common_setup("${AVND_TARGET}" "${AVND_FX_TARGET}")
endfunction()


add_library(Avendish_pd INTERFACE)
target_link_libraries(Avendish_pd INTERFACE Avendish)
add_library(Avendish::Avendish_pd ALIAS Avendish_pd)
