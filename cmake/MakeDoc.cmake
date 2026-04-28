# -------------------------------------------------------------------------------------------------- #
# Copyright (c) 2014 - 2023, 2025 by Axel Kenzo, axelkenzo@mail.ru
#
# MakeDoc.cmake
# -------------------------------------------------------------------------------------------------- #
find_program( GZIP gzip )
if( GZIP )
  message( "-- gzip found: ${GZIP}" )
else()
  message( "-- gzip not found" )
endif()

find_program( SPHINX sphinx-build )
if( SPHINX )
  message( "-- sphinx-build found: ${SPHINX}" )
else()
  message( "-- sphinx-build not found" )
endif()

find_program( DOXYGEN doxygen )
if( DOXYGEN )
  message( "-- doxygen found: ${DOXYGEN}" )
else()
  message( "-- doxygen not found" )
endif()

find_program( LATEXMK latexmk )
if( LATEXMK )
  message( "-- latexmk found: ${LATEXMK}" )
else()
  message( "-- latexmk not found" )
endif()

find_program( QHELPGENERATOR qhelpgenerator )
if( QHELPGENERATOR )
  message( "-- qhelpgenerator found: ${QHELPGENERATOR}" )
else()
  message( "-- qhelpgenerator not found" )
endif()

# -------------------------------------------------------------------------------------------------- #
if( UNIX )

  # ------------------------------------------------------------------------------
  # короткий путь генерации документации с использованием doxygen
  # ------------------------------------------------------------------------------
  if( DOXYGEN )

    configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.akrypt.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.akrypt @ONLY )

    set( script ${CMAKE_CURRENT_BINARY_DIR}/make-doxygen-${FULL_VERSION}.sh )
    file( WRITE ${script} "#/bin/bash\n" )
    file( APPEND ${script} "# формируем документацию api библиотеки в форматах html и pdf\n\n" )
    file( APPEND ${script} "doxygen Doxyfile.akrypt\n" )

    execute_process( COMMAND chmod +x ${script} )
    add_custom_target( doxygen ${script} )
    message("-- Script for doxygen documentation is done (now \"make doxygen\" enabled)")

  endif()

endif()
# -------------------------------------------------------------------------------------------------- #
#                                                                                     MakeDoc.cmake  #
# -------------------------------------------------------------------------------------------------- #


