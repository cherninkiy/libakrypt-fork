# -------------------------------------------------------------------------------------------------- #
# Copyright (c) 2014 - 2025 by Axel Kenzo, axelkenzo@mail.ru
#
# CMakeLists.txt
# -------------------------------------------------------------------------------------------------- #
# Минимальная версия cmake для сборки
 cmake_minimum_required( VERSION 3.20 )

# -------------------------------------------------------------------------------------------------- #
 set( HEAD_VERSION 0 )
 set( MAIN_VERSION 10 )
 set( MINOR_VERSION 1 )
 set( MAJOR_VERSION ${HEAD_VERSION}.${MAIN_VERSION} )
 set( FULL_VERSION ${MAJOR_VERSION}.${MINOR_VERSION} )

# -------------------------------------------------------------------------------------------------- #
# Формируем название проекта
 project( libakrypt C )

# -------------------------------------------------------------------------------------------------- #
# Перечень доступных опций для сборки библиотеки
 option( AK_SHARED_LIB "Build the shared library" ON )
 option( AK_STATIC_LIB "Build the static library" ON )
 option( AK_EXAMPLES "Build examples for libakrypt" OFF )

# -------------------------------------------------------------------------------------------------- #
# Проверка архитектуры
 message( STATUS "Using endianness: ${CMAKE_C_BYTE_ORDER}" )

# -------------------------------------------------------------------------------------------------- #
# Проверка разрядности системы
 if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
    message( STATUS "Using architecture: 32-bit ")
 endif()
 if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    message( STATUS "Using architecture: 64-bit ")
 endif()

# -------------------------------------------------------------------------------------------------- #
# Подключаем модули CMake для тонкой настройки параметров сборки
 set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH} )
 include( DetectHeaders )
 include( DetectFlags )
 include( DetectIntrinsics )

# -------------------------------------------------------------------------------------------------- #
# Вырабатываем заголовочный файл (в каталоге сборки)
 configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/source/libakrypt-config.h.in
   ${CMAKE_CURRENT_BINARY_DIR}/libakrypt-config.h
   @ONLY
 )
 message( STATUS "Generation of libakrypt-config.h is done")

# -------------------------------------------------------------------------------------------------- #
# Тип сборки библиотеки
 if( NOT CMAKE_BUILD_TYPE )
   set(
     CMAKE_BUILD_TYPE
     "Release"
     CACHE STRING
     "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
     FORCE
   )
 endif()
 message( STATUS "Build type: ${CMAKE_BUILD_TYPE}" )

# -------------------------------------------------------------------------------------------------- #
 message( STATUS "Used compile flags: ${CMAKE_C_FLAGS}")
 if( CMAKE_BUILD_TYPE STREQUAL Release )
    message( STATUS "Used release build type compile flags: ${CMAKE_C_FLAGS_RELEASE}")
 endif()

# -------------------------------------------------------------------------------------------------- #
# Определяем используемые каталоги
 include_directories( "source" )
 include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

# -------------------------------------------------------------------------------------------------- #
# Определяем базовое множество исходных текстов библиотеки
 set( MAIN_HEADERS
  ${CMAKE_CURRENT_BINARY_DIR}/libakrypt-config.h
  source/libakrypt.h
 )

 set( AKRYPT_SOURCES
   source/ak_tools.c
   source/ak_libakrypt.c
 )

# -------------------------------------------------------------------------------------------------- #
# формируем команды для сборки библиотеки
 if( AK_SHARED_LIB )
    add_library( akrypt-shared SHARED ${MAIN_HEADERS} ${AKRYPT_SOURCES} )
    set_target_properties( akrypt-shared PROPERTIES
                           VERSION ${MAJOR_VERSION}
                           SOVERSION ${FULL_VERSION}
                           OUTPUT_NAME akrypt
                           CLEAN_DIRECT_CUSTOM 1
    )
    target_link_libraries( akrypt-shared ${LIBAKRYPT_LIBS} )
    message( STATUS "Building libakrypt: shared library" )
 endif()

 if( AK_STATIC_LIB )
    add_library( akrypt-static STATIC ${MAIN_HEADERS} ${AKRYPT_SOURCES} )
    set_target_properties( akrypt-static PROPERTIES
                           OUTPUT_NAME akrypt
                           CLEAN_DIRECT_CUSTOM 1
    )
    target_link_libraries( akrypt-static ${LIBAKRYPT_LIBS} )
    message( STATUS "Building libakrypt: static library" )
 endif()

# -------------------------------------------------------------------------------------------------- #
# Сборка различных примеров и иллюстраций
# -------------------------------------------------------------------------------------------------- #
# определяем перечень примеров из FAQ
 set( EXAMPLES_LIST
      audit-01
      hello-01
    )

 if( AK_EXAMPLES )
   message( STATUS "libakrypt examples:  (now \"make test\" enabled):" )
   enable_testing()
   foreach( programm ${EXAMPLES_LIST} )
    if( AK_STATIC_LIB )
      add_executable( example-${programm} examples/faq/example-${programm}.c )
      target_link_libraries( example-${programm} akrypt-static ${LIBAKRYPT_LIBS} )
      message( NOTICE "      example-${programm}" )
      add_test( NAME example-${programm} COMMAND example-${programm} )
    endif()
    if( AK_SHARED_LIB )
      add_executable( example-shared-${programm} examples/faq/example-${programm}.c )
      target_link_libraries( example-shared-${programm} akrypt-shared ${LIBAKRYPT_LIBS} )
      message( NOTICE "      example-shared-${programm}" )
      add_test( NAME example-shared-${programm} COMMAND example-shared-${programm} )
    endif()
  endforeach()
 endif()

# -------------------------------------------------------------------------------------------------- #
# Сборка документации и завершающие настройки
 include( MakeDoc )
 set( AK_DOCUMENTATION
     ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
     ${CMAKE_CURRENT_SOURCE_DIR}/Changelog.md
)

# -------------------------------------------------------------------------------------------------- #
#                                                                                    CMakeLists.txt
# -------------------------------------------------------------------------------------------------- #
