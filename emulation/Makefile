ROOT=/vagrant

MODULE_DIR=/home/vagrant/cubic-modified

clone_module_dir:
	@if [ ! -d $(MODULE_DIR) ]; then \
		git clone https://github.com/janev94/careful-resume $(MODULE_DIR); \
	fi

module: $(MODULE_DIR)/tcp_cubic_cr.c | clone_module_dir
	cd $(MODULE_DIR) && make

install_module: module
ifneq ($(IW),)
	cd $(MODULE_DIR) && sudo make install_cubic_cr IW=$(IW) USE_JUMP=0 JUMP=60
else
	cd $(MODULE_DIR) && sudo make install_cubic_cr IW=10 USE_JUMP=1 JUMP=60
endif

# LEO BDP = 417
# 1/2 BDP = 208
# 2/3 BDP = 278

install_module_LEO_IW: module
ifneq ($(IW),)
	cd $(MODULE_DIR) && sudo make install_cubic_cr IW=$(IW)
else
	cd $(MODULE_DIR) && sudo make install_cubic_cr IW=10 USE_JUMP=0 JUMP=0
endif

install_module_LEO_12BDP: module
ifneq ($(IW),)
	cd $(MODULE_DIR) && sudo make install_cubic_cr IW=$(IW)
else
	cd $(MODULE_DIR) && sudo make install_cubic_cr IW=10 USE_JUMP=1 JUMP=208
endif

install_module_LEO_23BDP: module
ifneq ($(IW),)
	cd $(MODULE_DIR) && sudo make install_cubic_cr IW=$(IW)
else
	cd $(MODULE_DIR) && sudo make install_cubic_cr IW=10 USE_JUMP=1 JUMP=278
endif

unload_module:
	sudo rmmod tcp_cubic_cr

run: ${ROOT}/scripts/mn_script.py install_module
	sudo python ${ROOT}/scripts/mn_script.py
