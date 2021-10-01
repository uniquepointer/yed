LOC = $(HOME)/.local
CONF = $(HOME)/.config/yed

complete : install plugin userconf

ypm :
	@test -d $(CONF)/mpy || rm -r $(CONF)/mpy
	@test -d $(CONF)/ypm || mkdir $(CONF)/ypm

plugin : userconf
	./updateplugs.sh 2> ./updatelog
	@test -d $(CONF)/mpy || mkdir $(CONF)/mpy
	@test -d $(CONF)/mpy/mpy_plugins || cp -r ./mpy $(CONF)/mpy/mpy_plugins
	@test -d $(CONF)/mpy/plugins || mkdir $(CONF)/mpy/plugins
	@test -d $(CONF)/mpy/man || mkdir $(CONF)/mpy/man
	cp -r ./userconf/plugins/* $(CONF)/mpy/plugins/.
	@test -d ./userconf/man/man7 || mkdir -p ./userconf/man/ && cp -r ./userconf/man/man7 $(CONF)/mpy/man/.

userconf :
	@test -d ./userconf || mkdir ./userconf
	@test -d ./userconf/plugins || mkdir -p ./userconf/plugins/lang/syntax
	@test -f ./userconf/yedrc || cp  ./mpyconf/yedrc ./userconf/yedrc
	@test -f ./userconf/ypm_list || cp  ./mpyconf/ypm_list ./userconf/ypm_list
	@test -f ./userconf/init.c || cp ./mpyconf/init.c ./userconf/init.c
	@test -d ./userconf/man/man7 || mkdir -p ./userconf/man/man7
	@test -f $(CONF)/yedrc || cp  ./userconf/yedrc $(CONF)/yedrc
	@test -f $(CONF)/ypm_list || cp  ./userconf/ypm_list $(CONF)/ypm_listp
	@test -f $(CONF)init.c || cp ./userconf/init.c $(CONF)/init.c

install :
	@test -d $(LOC) || mkdir $(LOC)
	./install.sh -p $(LOC)

sysinstall :
	sudo ./install.sh
	
all : $(BUILDDIR)/$(BIN)

.PHONY : install sysinstall complete userconf plugin
