@cd ./compiler
@echo updating cc ...
@call make cc
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
