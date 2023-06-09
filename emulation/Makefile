ROOT=/vagrant

MODULE_DIR=/home/vagrant/cubic-modified

clone_module_dir:
	git clone https://github.com/janev94/careful-resume $(MODULE_DIR)

module: $(MODULE_DIR)/tcp_cubic_cr.c | clone_module_dir
	cd $(MODULE_DIR) && make

install_module: module
ifneq ($(IW),)
	cd $(MODULE_DIR) && sudo make install_cubic_cr IW=$(IW)
else
	cd $(MODULE_DIR) && sudo make install_cubic_cr IW=10
endif

unload_module:
	sudo rmmod tcp_cubic_cr

run: ${ROOT}/scripts/mn_script.py install_module
	sudo python ${ROOT}/scripts/mn_script.py
