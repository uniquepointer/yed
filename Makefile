LOC = $(HOME)/.local
CONF = $(HOME)/.config/yed

complete : install plugin userconf

ypm :
	@test -d $(CONF)/mpy || rm -r $(CONF)/mpy
	@test -d $(CONF)/ypm || mkdir $(CONF)/ypm

plugin : userconf
	./installplugs.sh 2> ./updatelog
	@test -d $(CONF)/mpy || mkdir $(CONF)/mpy
	@test -d $(CONF)/mpy/mpy_plugins || mkdir -p $(CONF)/mpy/mpy_plugins
	cp -r ./mpy/* $(CONF)/mpy/mpy_plugins
	@test -d $(CONF)/mpy/plugins || mkdir $(CONF)/mpy/plugins
	@test -d $(CONF)/mpy/man || mkdir $(CONF)/mpy/man
	cp -r ./userconf/plugins/* $(CONF)/mpy/plugins/.
	@test -d ./userconf/man/man7 || mkdir -p ./userconf/man/ && cp -r ./userconf/man/man7 $(CONF)/mpy/man/.

userconf :
	@test -d ./userconf || mkdir ./userconf
	@test -d ./userconf/plugins/lang/syntax || mkdir -p ./userconf/plugins/lang/syntax
	@test -d ./userconf/plugins/lang/tools || mkdir -p ./userconf/plugins/lang/tools
	@test -d ./userconf/plugins/styles || mkdir -p ./userconf/plugins/styles
	@test -f ./userconf/yedrc || cp  ./mpyconf/yedrc ./userconf/yedrc
	@test -f ./userconf/mpy_tally || cp  ./mpyconf/mpy_tally ./userconf/mpy_tally
	@test -f ./userconf/init.c || cp ./mpyconf/init.c ./userconf/init.c
	@test -d ./userconf/man/man7 || mkdir -p ./userconf/man/man7
	@test -d $(CONF)/ || mkdir -p $(CONF)/
	@test -f $(CONF)/yedrc || cp  ./userconf/yedrc $(CONF)/yedrc
	@test -f $(CONF)/mpy_tally || cp  ./userconf/mpy_tally $(CONF)/mpy_tally
	@test -f $(CONF)/init.c || cp ./userconf/init.c $(CONF)/init.c
	@test -f $(CONF)/build_init.sh || cp ./userconf/build_init.sh $(CONF)/build_init.sh

update :
	./updateplugs.sh 2> ./updatelog
	@test -d ./userconf/man/man7 || mkdir -p ./userconf/man/ && cp -r ./userconf/man/man7 $(CONF)/mpy/man/.

install :
	@test -d $(LOC) || mkdir $(LOC)
	./install.sh -p $(LOC)

sysinstall :
	sudo ./install.sh
	
all : $(BUILDDIR)/$(BIN)

.PHONY : install sysinstall complete userconf plugin ypm update
