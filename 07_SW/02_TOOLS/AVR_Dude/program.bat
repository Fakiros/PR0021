REM CD ../../01_BIN/01_HEX


"avrdude" -p m8 -c usbasp -P com1 -U flash:w:..\..\01_BIN\01_HEX\main.hex:i -U flash:v:..\..\01_BIN\01_HEX\main.eep:i -e
REM E:\cyfrowka\Programy\czekoladowy_terminal\terminal.exe
@pause