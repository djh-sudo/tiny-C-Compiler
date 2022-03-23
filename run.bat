@cd ./compiler
@echo updating cc ...
@call make
@echo update cc succefully!
@cd ..
@cd ./Assembler
@echo updating ass
@call make
@echo update ass successfully!
@cd ..
@cd ./Linker
@echo updating linker
@call make
@echo update linker successfully!
@cd ..
@echo updating main-control
@call make
@echo update over!