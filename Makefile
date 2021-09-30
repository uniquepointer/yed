LOC = $(HOME)/.local
complete : install
	

install :
	@test -d $(LOC) || mkdir $(LOC)
	./install.sh -p $(LOC)

sysinstall :
	sudo ./install.sh
	
all : $(BUILDDIR)/$(BIN)

.PHONY : install sysinstall
