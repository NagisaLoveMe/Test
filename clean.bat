@echo off
@echo Clean open at generated files...
del out\lnkfeedback.txt
del obj\*.* /F /Q
del log\*.* /F /Q
del out\*.* /F /Q
del out\dll\*.* /F /Q
del lib\openat.lib /F /Q
del mak\~linkdll.via
del lib\openatdll.o
del log\openatdll.c

rmdir /s /q out\backup
rmdir /s /q out\DYNAMIC_COMP_BIN
del lib\oa_audcoeff.o /F
@echo Clean finished!
pause
