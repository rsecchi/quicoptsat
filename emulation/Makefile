ROOT=/vagrant

MODULE_DIR=/home/vagrant/cubic-modified

module:
	cd $(MODULE_DIR) && make

install_module:
ifneq ($(IW),)
	cd $(MODULE_DIR) && sudo make install_cubic_cr IW=$(IW)
else
	cd $(MODULE_DIR) && sudo make install_cubic_cr IW=10
endif

unload_module:
	sudo rmmod tcp_cubic_cr

run:
	sudo python ${ROOT}/scripts/mn_script.py
