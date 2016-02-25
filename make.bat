@echo off
@echo Compiling open at user source codes...
if not exist C:\ARM\RVCT31\Licenses\rvds.dat (
@echo Please install third party compile entironment firstly!
@echo done
pause
exit
)

set LM_LICENSE_FILE=C:\ARM\RVCT31\Licenses\rvds.dat
set ARMLIB=C:\ARM\RVCT31\RVCT\Data\3.1\569\lib
set armpath=C:\ARM\RVCT31\RVCT\Programs\3.1\569\win_32-pentium

set ARMCONF=C:\ARM\RVCT31\RVCT\Programs\3.1\569\win_32-pentium
set ARMDLL=C:\ARM\RVCT31\RVCT\Programs\3.1\569\win_32-pentium
set ARMHOME=C:\ARM\RVCT31\RVCT
set ARMINC=C:\ARM\RVCT31\RVCT\Data\3.1\569\include\windows
set PATH="C:\ARM\RVCT31\RVCT\Programs\3.1\569\win_32-pentium";%PATH%
set RVCT31INC=C:\ARM\RVCT31\RVCT\Data\3.1\569\include\windows
set RVCT31BIN=C:\ARM\RVCT31\RVCT\Programs\3.1\569\win_32-pentium
set RVCT31LIB=C:\ARM\RVCT31\RVCT\Data\3.1\569\lib

attrib -h -s -r .\*.* /s 
del obj\*.* /F /Q
del out\dll\*.* /F /Q
del log\*.* /F /Q
del out\*.* /F /Q
del lib\openat.lib /F /Q
del mak\~linkdll.via
del lib\openatdll.o
del log\openatdll.c

rmdir /s /q out\backup
rmdir /s /q out\DYNAMIC_COMP_BIN

del lib\oa_audcoeff.o /F


@echo generating ~linkdll.via during making ALL
mak\make.exe oa_audcoeff -f mak\makefileADS
mak\make.exe all -f mak\makefileADS

@echo openatdll Linking...
%armpath%\armlink --via mak\~linkdll.via

@echo make openatdll.dll
%armpath%\fromelf out\dll\openatdll.elf --bin --output out\dll\openatdll.dll
perl mak\AddCRCCheck.pl out\dll\openatdll.dll

@echo make new openatdll.o for BIN linking
perl mak\dllbinToTxtString.pl 256 out\dll\openatdll.dll log\openatdll.c
mak\make.exe openatdll -f mak\makefileADS

@echo linking BIG BIN
%armpath%\armlink.exe --via mak\~link.via
%armpath%\fromelf.exe out\OPENAT_BIN.elf -bin out

perl mak\get_gfh_files.pl out GFH out\~gfh_files.tmp

perl mak\vivaProcess.pl out mak\OPENAT_gprs.mak 
perl mak\gfh_process.pl mak\~gfh_files.tmp mak\~gfh_cfg.tmp mak\OPENAT_GPRS.mak


copy /Y lib\OPENAT_BTLD.bin out\BOOTLOADER
copy /Y lib\EXT_BOOTLOADER out\EXT_BOOTLOADER
copy /Y mak\OPENAT_FLASHTOOL.cfg out\OPENAT_FLASHTOOL.cfg

@echo done!
pause
