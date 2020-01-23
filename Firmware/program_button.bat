@echo Programming the Qwiic Button. If this looks incorrect, abort and retry.
@pause
:loop

@echo -
@echo Flashing bootloader...
@avrdude -C avrdude.conf -pattiny84 -cusbtiny -e -Uefuse:w:0xFF:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m

@timeout 1

@echo -
@echo Flashing firmware...

rem The -B1 option reduces the bitclock period (1us = 1MHz SPI), decreasing programming time
rem May increase verification errors

@avrdude -C avrdude.conf -pattiny84 -cusbtiny -e -Uflash:w:Qwiic_Button.ino.hex:i -B1

@echo Done programming! Move on to next board.
@pause

goto loop