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
	python3 /vagrant/scripts/analytics/load_cubic_module.py --module_dir ${MODULE_DIR} --IW 10 --USE_JUMP 0 --JUMP 0 --hystart 1

install_module_LEO_12BDP: module
	python3 /vagrant/scripts/analytics/load_cubic_module.py --module_dir ${MODULE_DIR} --IW 10 --USE_JUMP 1 --JUMP 208 --hystart 1

install_module_LEO_23BDP: module
	python3 /vagrant/scripts/analytics/load_cubic_module.py --module_dir ${MODULE_DIR} --IW 10 --USE_JUMP 1 --JUMP 278 --hystart 1


# GEO BDP = 1146
# 1/2 BDP = 573
# 2/3 BDP = 764

install_module_GEO_IW: module
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
	sudo python ${ROOT}/scripts/mn_script.py --cong_alg cubic_cr --transfer_size 50000 --log_root /vagrant/logs/test


LOGS_TEST = $(foreach size, $(shell seq 50000 50000 5000000), /vagrant/logs/DSL/cubic_cr/${size}/experiment.info )
CWND_FIGS = $(foreach size, $(shell seq 50000 50000 5000000), /vagrant/logs/DSL/cubic_cr/${size}/cwnd.png )

LOGS_IW = $(foreach size, $(shell seq 50000 50000 5000000), /vagrant/logs/LEO/cubic_cr/IW/${size}/experiment.info )
LOGS_12BDP = $(foreach size, $(shell seq 50000 50000 5000000), /vagrant/logs/LEO/cubic_cr/12BDP/${size}/experiment.info )
LOGS_23BDP = $(foreach size, $(shell seq 50000 50000 5000000), /vagrant/logs/LEO/cubic_cr/23BDP/${size}/experiment.info )

LOGS_IW_CWND = $(foreach size, $(shell seq 50000 50000 5000000), /vagrant/logs/LEO/cubic_cr/IW/${size}/cwnd.png )
LOGS_12BDP_CWND = $(foreach size, $(shell seq 50000 50000 5000000), /vagrant/logs/LEO/cubic_cr/12BDP/${size}/cwnd.png )
LOGS_23BDP_CWND = $(foreach size, $(shell seq 50000 50000 5000000), /vagrant/logs/LEO/cubic_cr/23BDP/${size}/cwnd.png )

LOGS_ALL = ${LOGS_IW} ${LOGS_12BDP} ${LOGS_23BDP} 

PLOTS_CWND = ${LOGS_IW_CWND} ${LOGS_12BDP_CWND} ${LOGS_23BDP_CWND}

%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval RES_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE}


# LOGS_IW
/vagrant/logs/LEO/cubic_cr/IW/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json


# LOGS_12BDP
/vagrant/logs/LEO/cubic_cr/12BDP/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json


# LOGS_23BDP
/vagrant/logs/LEO/cubic_cr/23BDP/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json

##############
# CWND Plots
##############

# LOGS_IW
/vagrant/logs/LEO/cubic_cr/IW/%/cwnd.png: /vagrant/logs/LEO/cubic_cr/IW/%/kern.log /vagrant/scripts/analytics/plot_cwnd.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	python3 /vagrant/scripts/analytics/plot_cwnd.py --kern_log_path $< --save_path $(@D) --file_type png


# LOGS_12BDP 
/vagrant/logs/LEO/cubic_cr/12BDP/%/cwnd.png: /vagrant/logs/LEO/cubic_cr/12BDP/%/kern.log /vagrant/scripts/analytics/plot_cwnd.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	python3 /vagrant/scripts/analytics/plot_cwnd.py --kern_log_path $< --save_path $(@D) --file_type png


# LOGS_23BDP
/vagrant/logs/LEO/cubic_cr/23BDP/%/cwnd.png: /vagrant/logs/LEO/cubic_cr/23BDP/%/kern.log /vagrant/scripts/analytics/plot_cwnd.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	python3 /vagrant/scripts/analytics/plot_cwnd.py --kern_log_path $< --save_path $(@D) --file_type png


experiments_all: ${LOGS_ALL}


experiments_test: ${LOGS_TEST}


experiments_plots: ${PLOTS_CWND}


/vagrant/logs/DSL/cubic_cr/%/cwnd.png: /vagrant/logs/DSL/cubic_cr/%/kern.log /vagrant/scripts/analytics/plot_cwnd.py
	python3 /vagrant/scripts/analytics/plot_cwnd.py --kern_log_path $< --save_path $@ --file_type png

plot_cwnds: ${CWND_FIGS}

	
