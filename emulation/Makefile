ROOT=/vagrant

MODULE_DIR=/home/vagrant/cubic-modified

FILE_SIZES = $(shell seq 50000 50000 5000000)

clone_module_dir:
	@if [ ! -d $(MODULE_DIR) ]; then \
		git clone https://github.com/janev94/careful-resume $(MODULE_DIR); \
	fi

module: $(MODULE_DIR)/tcp_cubic_cr.c | clone_module_dir
	cd $(MODULE_DIR) && sudo make

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


install_module_LEO_IW_SS: module
	python3 /vagrant/scripts/analytics/load_cubic_module.py --module_dir ${MODULE_DIR} --IW 10 --USE_JUMP 0 --JUMP 0 --hystart 0

install_module_LEO_12BDP_SS: module
	python3 /vagrant/scripts/analytics/load_cubic_module.py --module_dir ${MODULE_DIR} --IW 10 --USE_JUMP 1 --JUMP 208 --hystart 0

install_module_LEO_23BDP_SS: module
	python3 /vagrant/scripts/analytics/load_cubic_module.py --module_dir ${MODULE_DIR} --IW 10 --USE_JUMP 1 --JUMP 278 --hystart 0

##############
# HyStart++ ##
##############

install_hpp_LEO_12BDP: module
	echo 208 | sudo tee /sys/module/tcp_cubic_hpp/parameters/JUMP

install_hpp_LEO_23BDP: module
	echo 278 | sudo tee /sys/module/tcp_cubic_hpp/parameters/JUMP

# GEO BDP = 1146
# 1/2 BDP = 573
# 2/3 BDP = 764

install_module_GEO_IW: module
ifneq ($(IW),)
	python3 /vagrant/scripts/analytics/load_cubic_module.py --module_dir ${MODULE_DIR} --IW $(IW) --USE_JUMP 0 --JUMP 0 --hystart 1
else
	python3 /vagrant/scripts/analytics/load_cubic_module.py --module_dir ${MODULE_DIR} --IW 10 --USE_JUMP 0 --JUMP 0 --hystart 1
endif

install_module_GEO_12BDP: module
ifneq ($(IW),)
	python3 /vagrant/scripts/analytics/load_cubic_module.py --module_dir ${MODULE_DIR} --IW $(IW) --USE_JUMP 1 --JUMP 573 --hystart 1
else
	python3 /vagrant/scripts/analytics/load_cubic_module.py --module_dir ${MODULE_DIR} --IW 10 --USE_JUMP 1 --JUMP 573 --hystart 1
endif

install_module_GEO_23BDP: module
ifneq ($(IW),)
	python3 /vagrant/scripts/analytics/load_cubic_module.py --module_dir ${MODULE_DIR} --IW $(IW) --USE_JUMP 1 --JUMP 764 --hystart 1
else
	python3 /vagrant/scripts/analytics/load_cubic_module.py --module_dir ${MODULE_DIR} --IW 10 --USE_JUMP 1 --JUMP 764 --hystart 1
endif


##############
# HyStart++ ##
##############

install_hpp_GEO_12BDP: module
	echo 573 | sudo tee /sys/module/tcp_cubic_hpp/parameters/JUMP

install_hpp_GEO_23BDP: module
	echo 764 | sudo tee /sys/module/tcp_cubic_hpp/parameters/JUMP


unload_module:
	sudo rmmod tcp_cubic_cr

run: ${ROOT}/scripts/mn_script.py install_module
	sudo python ${ROOT}/scripts/mn_script.py --cong_alg cubic_cr --transfer_size 50000 --log_root /vagrant/logs/test


LOGS_TEST = $(foreach size, ${FILE_SIZES}, /vagrant/logs/DSL/cubic_cr/${size}/experiment.info )
CWND_FIGS = $(foreach size, ${FILE_SIZES}, /vagrant/logs/DSL/cubic_cr/${size}/cwnd.png )

LOGS_IW_LEO = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_cr/IW/${size}/experiment.info )
LOGS_12BDP_LEO = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_cr/12BDP/${size}/experiment.info )
LOGS_23BDP_LEO = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_cr/23BDP/${size}/experiment.info )

LOGS_IW_LEO_PACING = $(foreach size, ${FILE_SIZES}, $(foreach repeat, $(shell seq 1 5), /vagrant/logs/pacing/LEO/cubic_cr/IW/${size}/${repeat}/experiment.info ))
LOGS_12BDP_LEO_PACING = $(foreach size, ${FILE_SIZES}, $(foreach repeat, $(shell seq 1 5), /vagrant/logs/pacing/LEO/cubic_cr/12BDP/${size}/${repeat}/experiment.info ))
LOGS_23BDP_LEO_PACING = $(foreach size, ${FILE_SIZES}, $(foreach repeat, $(shell seq 1 5), /vagrant/logs/pacing/LEO/cubic_cr/23BDP/${size}/${repeat}/experiment.info ))

LOGS_IW_LEO_HPP = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_hpp/IW/${size}/experiment.info )
LOGS_12BDP_LEO_HPP = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_hpp/12BDP/${size}/experiment.info )
LOGS_23BDP_LEO_HPP = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_hpp/23BDP/${size}/experiment.info )

LOGS_IW_LEO_HPP_PACING = $(foreach size, ${FILE_SIZES}, $(foreach repeat, $(shell seq 1 5), /vagrant/logs/pacing/LEO/cubic_hpp/IW/${size}/${repeat}/experiment.info ))
LOGS_12BDP_LEO_HPP_PACING = $(foreach size, ${FILE_SIZES}, $(foreach repeat, $(shell seq 1 5), /vagrant/logs/pacing/LEO/cubic_hpp/12BDP/${size}/${repeat}/experiment.info ))
LOGS_23BDP_LEO_HPP_PACING = $(foreach size, ${FILE_SIZES}, $(foreach repeat, $(shell seq 1 5), /vagrant/logs/pacing/LEO/cubic_hpp/23BDP/${size}/${repeat}/experiment.info ))

LOGS_IW_LEO_SS = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_cr_ss/IW/${size}/experiment.info )
LOGS_12BDP_LEO_SS = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_cr_ss/12BDP/${size}/experiment.info )
LOGS_23BDP_LEO_SS = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_cr_ss/23BDP/${size}/experiment.info )


LOGS_IW_GEO = $(foreach size, ${FILE_SIZES}, /vagrant/logs/GEO/cubic_cr/IW/${size}/experiment.info )
LOGS_12BDP_GEO = $(foreach size, ${FILE_SIZES}, /vagrant/logs/GEO/cubic_cr/12BDP/${size}/experiment.info )
LOGS_23BDP_GEO = $(foreach size, ${FILE_SIZES}, /vagrant/logs/GEO/cubic_cr/23BDP/${size}/experiment.info )

LOGS_IW_GEO_PACING = $(foreach size, ${FILE_SIZES}, /vagrant/logs/pacing/GEO/cubic_cr/IW/${size}/experiment.info )
LOGS_12BDP_GEO_PACING = $(foreach size, ${FILE_SIZES}, /vagrant/logs/pacing/GEO/cubic_cr/12BDP/${size}/experiment.info )
LOGS_23BDP_GEO_PACING = $(foreach size, ${FILE_SIZES}, /vagrant/logs/pacing/GEO/cubic_cr/23BDP/${size}/experiment.info )

LOGS_IW_GEO_HPP = $(foreach size, ${FILE_SIZES}, /vagrant/logs/GEO/cubic_hpp/IW/${size}/experiment.info )
LOGS_12BDP_GEO_HPP = $(foreach size, ${FILE_SIZES}, /vagrant/logs/GEO/cubic_hpp/12BDP/${size}/experiment.info )
LOGS_23BDP_GEO_HPP = $(foreach size, ${FILE_SIZES}, /vagrant/logs/GEO/cubic_hpp/23BDP/${size}/experiment.info )

LOGS_IW_GEO_HPP_PACING = $(foreach size, ${FILE_SIZES}, /vagrant/logs/pacing/GEO/cubic_hpp/IW/${size}/experiment.info )
LOGS_12BDP_GEO_HPP_PACING = $(foreach size, ${FILE_SIZES}, /vagrant/logs/pacing/GEO/cubic_hpp/12BDP/${size}/experiment.info )
LOGS_23BDP_GEO_HPP_PACING = $(foreach size, ${FILE_SIZES}, /vagrant/logs/pacing/GEO/cubic_hpp/23BDP/${size}/experiment.info )

LOGS_LEO = ${LOGS_IW_LEO} ${LOGS_12BDP_LEO} ${LOGS_23BDP_LEO}
LOGS_LEO_PACING = ${LOGS_IW_LEO_PACING} ${LOGS_12BDP_LEO_PACING} ${LOGS_23BDP_LEO_PACING}
LOGS_LEO_SS = ${LOGS_IW_LEO_SS} ${LOGS_12BDP_LEO_SS} ${LOGS_23BDP_LEO_SS}
LOGS_LEO_HPP = ${LOGS_IW_LEO_HPP} ${LOGS_12BDP_LEO_HPP} ${LOGS_23BDP_LEO_HPP}

LOGS_GEO = ${LOGS_IW_GEO} ${LOGS_12BDP_GEO} ${LOGS_23BDP_GEO}
LOGS_GEO_PACING = ${LOGS_IW_GEO_PACING} ${LOGS_12BDP_GEO_PACING} ${LOGS_23BDP_GEO_PACING}
LOGS_GEO_HPP = ${LOGS_IW_GEO_HPP} ${LOGS_12BDP_GEO_HPP} ${LOGS_23BDP_GEO_HPP}
LOGS_ALL = ${LOGS_IW_LEO} ${LOGS_12BDP_LEO} ${LOGS_23BDP_LEO} ${LOGS_GEO}

LOGS_IW_CWND = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_cr/IW/${size}/cwnd.png )
LOGS_12BDP_CWND = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_cr/12BDP/${size}/cwnd.png )
LOGS_23BDP_CWND = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_cr/23BDP/${size}/cwnd.png )

LOGS_IW_SS_CWND = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_cr_ss/IW/${size}/cwnd.png )
LOGS_12BDP_SS_CWND = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_cr_ss/12BDP/${size}/cwnd.png )
LOGS_23BDP_SS_CWND = $(foreach size, ${FILE_SIZES}, /vagrant/logs/LEO/cubic_cr_ss/23BDP/${size}/cwnd.png )

PLOTS_CWND = ${LOGS_IW_CWND} ${LOGS_12BDP_CWND} ${LOGS_23BDP_CWND}
PLOTS_CWND_SS = ${LOGS_IW_SS_CWND} ${LOGS_12BDP_SS_CWND} ${LOGS_23BDP_SS_CWND}

# /vagrant/logs/test/experiment.info: install_module_LEO_12BDP_SS ${ROOT}/scripts/mn_script.py
/vagrant/logs/test/_experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval RES_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${RES_DIR} --cong_alg cubic_hpp --transfer_size 5000000 --network_model_file /vagrant/network_models/links/LEO.json

test: /vagrant/logs/test/_experiment.info



/vagrant/logs/pacing/_NON_EXISTANT_experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval RES_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${RES_DIR} --cong_alg cubic_cr --transfer_size 5000000 --network_model_file /vagrant/network_models/links/LEO.json


##############
# LOGS LEO
##############

# LOGS_IW
/vagrant/logs/LEO/cubic_cr/IW/%/experiment.info /vagrant/logs/LEO/cubic_cr_ss/IW/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json


# LOGS_12BDP
/vagrant/logs/LEO/cubic_cr/12BDP/%/experiment.info /vagrant/logs/LEO/cubic_cr_ss/12BDP/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json


# LOGS_23BDP
/vagrant/logs/LEO/cubic_cr/23BDP/%/experiment.info /vagrant/logs/LEO/cubic_cr_ss/23BDP/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json


# LOGS_IW
/vagrant/logs/pacing/LEO/cubic_cr/IW/%/experiment.info /vagrant/logs/pacing/LEO/cubic_cr_ss/IW/%/experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(shell dirname $(@D)))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json


# LOGS_12BDP
/vagrant/logs/pacing/LEO/cubic_cr/12BDP/%/experiment.info /vagrant/logs/pacing/LEO/cubic_cr_ss/12BDP/%/experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(shell dirname $(@D)))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json


# LOGS_23BDP
/vagrant/logs/pacing/LEO/cubic_cr/23BDP/%/experiment.info /vagrant/logs/pacing/LEO/cubic_cr_ss/23BDP/%/experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(shell dirname $(@D)))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json


# LOGS_IW
/vagrant/logs/LEO/cubic_hpp/IW/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_hpp --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json


# LOGS_12BDP
/vagrant/logs/LEO/cubic_hpp/12BDP/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_hpp --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json


# LOGS_23BDP
/vagrant/logs/LEO/cubic_hpp/23BDP/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_hpp --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json


# LOGS_IW
/vagrant/logs/pacing/LEO/cubic_hpp/IW/%/experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${OUTPUT_DIR} --cong_alg cubic_hpp --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json

# LOGS 12 BDP
/vagrant/logs/pacing/LEO/cubic_hpp/12BDP/%/experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${OUTPUT_DIR} --cong_alg cubic_hpp --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json

# LOGS 23 BDP
/vagrant/logs/pacing/LEO/cubic_hpp/23BDP/%/experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${OUTPUT_DIR} --cong_alg cubic_hpp --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/LEO.json

##############
# LOGS GEO
##############

# LOGS_IW
/vagrant/logs/GEO/cubic_cr/IW/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/GEO.json


# LOGS_12BDP
/vagrant/logs/GEO/cubic_cr/12BDP/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/GEO.json


# LOGS_23BDP
/vagrant/logs/GEO/cubic_cr/23BDP/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/GEO.json


# LOGS_IW
/vagrant/logs/pacing/GEO/cubic_cr/IW/%/experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/GEO.json


# LOGS_12BDP
/vagrant/logs/pacing/GEO/cubic_cr/12BDP/%/experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/GEO.json


# LOGS_23BDP
/vagrant/logs/pacing/GEO/cubic_cr/23BDP/%/experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${OUTPUT_DIR} --cong_alg cubic_cr --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/GEO.json


# LOGS_IW
/vagrant/logs/GEO/cubic_hpp/IW/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_hpp --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/GEO.json


# LOGS_12BDP
/vagrant/logs/GEO/cubic_hpp/12BDP/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_hpp --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/GEO.json


# LOGS_23BDP
/vagrant/logs/GEO/cubic_hpp/23BDP/%/experiment.info: ${ROOT}/scripts/mn_script.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script.py --log_root ${OUTPUT_DIR} --cong_alg cubic_hpp --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/GEO.json


# LOGS_IW
/vagrant/logs/pacing/GEO/cubic_hpp/IW/%/experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${OUTPUT_DIR} --cong_alg cubic_hpp --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/GEO.json


# LOGS_12BDP
/vagrant/logs/pacing/GEO/cubic_hpp/12BDP/%/experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${OUTPUT_DIR} --cong_alg cubic_hpp --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/GEO.json


# LOGS_23BDP
/vagrant/logs/pacing/GEO/cubic_hpp/23BDP/%/experiment.info: ${ROOT}/scripts/mn_script_pacing.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	sudo python ${ROOT}/scripts/mn_script_pacing.py --log_root ${OUTPUT_DIR} --cong_alg cubic_hpp --transfer_size ${TRANSFER_SIZE} --network_model_file /vagrant/network_models/links/GEO.json


##############
# CWND Plots
##############

# LOGS_IW
/vagrant/logs/LEO/cubic_cr/IW/%/cwnd.png /vagrant/logs/LEO/cubic_cr_ss/IW/%/cwnd.png: /vagrant/logs/LEO/cubic_cr/IW/%/kern.log /vagrant/scripts/analytics/plot_cwnd.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	python3 /vagrant/scripts/analytics/plot_cwnd.py --kern_log_path $< --save_path $(@D) --file_type png


# LOGS_12BDP 
/vagrant/logs/LEO/cubic_cr/12BDP/%/cwnd.png /vagrant/logs/LEO/cubic_cr_ss/12BDP/%/cwnd.png: /vagrant/logs/LEO/cubic_cr/12BDP/%/kern.log /vagrant/scripts/analytics/plot_cwnd.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	python3 /vagrant/scripts/analytics/plot_cwnd.py --kern_log_path $< --save_path $(@D) --file_type png


# LOGS_23BDP
/vagrant/logs/LEO/cubic_cr/23BDP/%/cwnd.png /vagrant/logs/LEO/cubic_cr_ss/23BDP/%/cwnd.png: /vagrant/logs/LEO/cubic_cr/23BDP/%/kern.log /vagrant/scripts/analytics/plot_cwnd.py
	$(eval OUTPUT_DIR = $(@D))
	$(eval TRANSFER_SIZE = $(shell echo $(shell basename $(@D))))
	python3 /vagrant/scripts/analytics/plot_cwnd.py --kern_log_path $< --save_path $(@D) --file_type png


experiments_all: ${LOGS_ALL}

experiments_LEO_IW: install_module_LEO_IW ${LOGS_IW_LEO}

experiments_LEO_12: install_module_LEO_12BDP ${LOGS_12BDP_LEO}

experiments_LEO_23: install_module_LEO_23BDP ${LOGS_23BDP_LEO}

experiments_LEO_IW_PACING: install_module_LEO_IW ${LOGS_IW_LEO_PACING}

experiments_LEO_12_PACING: install_module_LEO_12BDP ${LOGS_12BDP_LEO_PACING}

experiments_LEO_23_PACING: install_module_LEO_23BDP ${LOGS_23BDP_LEO_PACING}

experiments_LEO: experiments_LEO_IW experiments_LEO_12 experiments_LEO_23 ${LOGS_LEO}

experiments_LEO_PACING: experiments_LEO_IW_PACING experiments_LEO_12_PACING experiments_LEO_23_PACING ${LOGS_LEO_PACING}

experiments_LEO_IW_HPP: ${LOGS_IW_LEO_HPP}

experiments_LEO_12_HPP: ${LOGS_12BDP_LEO_HPP}

experiments_LEO_23_HPP: ${LOGS_23BDP_LEO_HPP}

experiments_GEO_IW_HPP: ${LOGS_IW_GEO_HPP}

experiments_GEO_12_HPP: ${LOGS_12BDP_GEO_HPP}

experiments_GEO_23_HPP: ${LOGS_23BDP_GEO_HPP}


##########
# Pacing #
##########

experiments_LEO_IW_HPP_pacing: ${LOGS_IW_LEO_HPP_PACING}

experiments_LEO_12_HPP_pacing: install_hpp_LEO_12BDP ${LOGS_12BDP_LEO_HPP_PACING}

experiments_LEO_23_HPP_pacing: install_hpp_LEO_23BDP ${LOGS_23BDP_LEO_HPP_PACING}

experiments_GEO_IW_HPP_pacing: ${LOGS_IW_GEO_HPP_PACING}

experiments_GEO_12_HPP_pacing: install_hpp_GEO_12BDP ${LOGS_12BDP_GEO_HPP_PACING}

experiments_GEO_23_HPP_pacing: install_hpp_GEO_23BDP ${LOGS_23BDP_GEO_HPP_PACING}



experiments_LEO_IW_SS: install_module_LEO_IW_SS ${LOGS_IW_LEO_SS}

experiments_LEO_12_SS: install_module_LEO_12BDP_SS ${LOGS_12BDP_LEO_SS}

experiments_LEO_23_SS: install_module_LEO_23BDP_SS ${LOGS_23BDP_LEO_SS}

experiments_LEO_SS: experiments_LEO_IW_SS experiments_LEO_12_SS experiments_LEO_23_SS ${LOGS_LEO_SS}

experiments_GEO_IW: ${LOGS_IW_GEO}

experiments_GEO_12: ${LOGS_12BDP_GEO}

experiments_GEO_23: ${LOGS_23BDP_GEO}

experiments_GEO_IW_PACING: install_module_GEO_IW ${LOGS_IW_GEO_PACING}

experiments_GEO_12_PACING: install_module_GEO_12BDP ${LOGS_12BDP_GEO_PACING}

experiments_GEO_23_PACING: install_module_GEO_23BDP ${LOGS_23BDP_GEO_PACING}

experiments_GEO: ${LOGS_GEO}

experiments_GEO_PACING: experiments_GEO_IW_PACING experiments_GEO_12_PACING experiments_GEO_23_PACING ${LOGS_GEO_PACING}

experiments_test: ${LOGS_TEST}

pacing: install_module_LEO_12BDP /vagrant/logs/pacing/_NON_EXISTANT_experiment.info 

experiments_plots: ${PLOTS_CWND}

plots_LEO_SS: ${PLOTS_CWND_SS}

IETF_plots: /vagrant/logs/LEO/cubic_cr/12BDP/5000000/kern.log /vagrant/logs/pacing/kern.log /vagrant/scripts/analytics/plot_cwnd.py
	python3 /vagrant/scripts/analytics/plot_cwnd.py --kern_log_path /vagrant/logs/LEO/cubic_cr/12BDP/5000000/kern.log --save_path /vagrant/plots --file_type png

/vagrant/logs/DSL/cubic_cr/%/cwnd.png: /vagrant/logs/DSL/cubic_cr/%/kern.log /vagrant/scripts/analytics/plot_cwnd.py
	python3 /vagrant/scripts/analytics/plot_cwnd.py --kern_log_path $< --save_path $@ --file_type png

/vagrant/logs/DSL/cubic_cr_ss/%/cwnd.png: /vagrant/logs/DSL/cubic_cr_ss/%/kern.log /vagrant/scripts/analytics/plot_cwnd.py
	python3 /vagrant/scripts/analytics/plot_cwnd.py --kern_log_path $< --save_path $@ --file_type png

/vagrant/logs/test/cwnd.png: /vagrant/logs/test/kern.log /vagrant/scripts/analytics/plot_cwnd.py
	python3 /vagrant/scripts/analytics/plot_cwnd.py --kern_log_path $< --save_path $(@D) --file_type png

plot_cwnds: ${CWND_FIGS}
 
_COMPLETION_TIME_FIG_NAMES = completion_times_RTT_LEO_cubic_cr_pacing.png completion_times_RTT_GEO_cubic_cr_pacing.png\
								completion_times_RTT_LEO_cubic_hpp_pacing.png completion_times_RTT_GEO_cubic_hpp_pacing.png \
								completion_times_RTT_LEO_cubic_cr.png completion_times_RTT_GEO_cubic_cr.png \
								completion_times_RTT_LEO_cubic_hpp.png completion_times_RTT_GEO_cubic_hpp.png
COMPLETION_TIME_FIGS = $(foreach figname, ${_COMPLETION_TIME_FIG_NAMES}, /vagrant/plots/${figname})

completion_time_plots: ${COMPLETION_TIME_FIGS}
	@echo "Done" 


PARSED_LOGS = /vagrant/logs/parsed/pacing/LEO/cubic_cr/completion_times.json /vagrant/logs/parsed/pacing/GEO/cubic_cr/completion_times.json


parsed_logs: ${PARSED_LOGS}

/vagrant/logs/parsed/pacing/LEO/cubic_cr/completion_times.json: /vagrant/scripts/analytics/parse_completion_time.py
	/bin/python3 /vagrant/scripts/analytics/parse_completion_time.py --root /vagrant/logs/pacing/LEO/cubic_cr --subdirs IW 12BDP 23BDP --experiment_paths ${FILE_SIZES}

/vagrant/logs/parsed/pacing/GEO/cubic_cr/completion_times.json: /vagrant/scripts/analytics/parse_completion_time.py
	/bin/python3 /vagrant/scripts/analytics/parse_completion_time.py --root /vagrant/logs/pacing/GEO/cubic_cr --subdirs IW 12BDP 23BDP --experiment_paths ${FILE_SIZES}
	
/vagrant/plots/completion_times_RTT_%.png: /vagrant/scripts/analytics/plot_completion_times.py
	$(eval LINK_TYPE = ${shell echo $(*) | cut -d '_' -f 1})
	$(eval ALG = ${shell echo $(*) | cut -d '_' -f 2,3})
	python3 /vagrant/scripts/analytics/plot_completion_times.py --figname ${@} --plot_data_file /vagrant/logs/parsed/${LINK_TYPE}/${ALG}/completion_times.json

/vagrant/plots/completion_times_RTT_%_pacing.png: /vagrant/scripts/analytics/plot_completion_times.py
	$(eval LINK_TYPE = ${shell echo $(*) | cut -d '_' -f 1})
	$(eval ALG = ${shell echo $(*) | cut -d '_' -f 2,3})
	python3 /vagrant/scripts/analytics/plot_completion_times.py --figname ${@} --plot_data_file /vagrant/logs/parsed/pacing/${LINK_TYPE}/${ALG}/completion_times.json
