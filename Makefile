LOC = $(HOME)/.local
CONF = $(HOME)/.config/yed

complete : install plugin userconf/yedrc

userconf :
	@test -d ./userconf || mkdir ./userconf
	@test -f ./userconf/yedrc || cp  ./mpyconf/yedrc ./userconf/yedrc
	@test -f ./userconf/ypm_list || cp  ./mpyconf/ypm_list ./userconf/ypm_list
	@test -f $(CONF)/yedrc || cp  ./userconf/yedrc $(CONF)/yedrc
	@test -f $(CONF)/ypm_list || cp  ./userconf/ypm_list $(CONF)/ypm_list

plugin : 
	@test -d $(CONF)/ypm || mkdir $(CONF)/ypm
	@test -d $(CONF)/ypm/ypm_plugins || cp -r ./mpy $(CONF)/ypm/ypm_plugins
	

install :
	@test -d $(LOC) || mkdir $(LOC)
	./install.sh -p $(LOC)

sysinstall :
	sudo ./install.sh
	
all : $(BUILDDIR)/$(BIN)

.PHONY : install sysinstall complete userconf plugin
