#!/usr/bin/bash

cd /vagrant/hystartpp
sudo make
sudo make install_hystartpp

sudo sysctl net.ipv4.tcp_congestion_control=cubic_hystartpp