cmake_minimum_required(VERSION 3.15)
project(F103Btld C)

set(CMAKE_C_STANDARD 11)

include_directories(HardWare/inc)
include_directories(Inc)
include_directories(Startup)
include_directories(Tools/inc)

add_executable(F103Btld
        App/main.c
        HardWare/inc/Led.h
        HardWare/inc/Timer.h
        HardWare/inc/Uart.h
        HardWare/Led.c
        HardWare/Timer.c
        HardWare/Uart.c
        Inc/cmsis_gcc.h
        Inc/core_cm3.h
        Inc/core_cmFunc.h
        Inc/core_cmInstr.h
        Inc/stm32f103xe.h
        Startup/Public.h
        Startup/sys.c
        Startup/sys.h
        Tools/inc/CrcUtil.h
        Tools/inc/IapHwOpts.h
        Tools/inc/IapUtil.h
        Tools/inc/malloc.h
        Tools/inc/TaskList.h
        Tools/inc/TaskManager.h
        Tools/CrcUtil.c
        Tools/IapHwOpts.c
        Tools/IapUtil.c
        Tools/malloc.c
        Tools/TaskList.c
        Tools/TaskManager.c)
