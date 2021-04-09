SET PATH=D:\Keil_v5\ARM\ARMCC\Bin;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\IDM Computer Solutions\UltraEdit;C:\Program Files\TortoiseGit\bin
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F103CB
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\main.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\delay.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\sys.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\usart.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\led.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\switch.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\tempcollecte.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\pid.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\temp_sample.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\tempcontrol.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stmflash.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\obj\startup_stm32f10x_hd._ia"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_it.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\system_stm32f10x.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\misc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_gpio.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_rcc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_tim.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_usart.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_adc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_flash.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_core.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_flag.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_mem.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_q.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_sem.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_task.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_time.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\obj\os_cpu_a._ia"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_cpu_c.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_dbg.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\product.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\task.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\act.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\obj\Hs660TransportCtrl.lnp"
"D:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\obj\Hs660TransportCtrl.axf" --i32combined --output ".\obj\Hs660TransportCtrl.hex"
