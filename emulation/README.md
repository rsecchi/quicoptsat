
# Prerequisites

1. Vagrant
2. Hypervisor (we recommend libvirt but others can be adapted too)



# Running the emulator

In the root directory run `vagrant up`.
This creates a virtual machine (VM) and installs dependencies that can then be used to run the kernel modules.
Once the VM is up and running login using `vagrant ssh`.
From within the VM, navigate to /vagrant.
From there, use the provided Makefile to run experiments.

