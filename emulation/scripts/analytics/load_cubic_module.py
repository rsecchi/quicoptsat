import argparse
import subprocess
import json
import time


if __name__ == '__main__':
    is_loaded_output = subprocess.run('sudo lsmod | grep tcp_cubic_cr', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if is_loaded_output.stderr:
        print(is_loaded_output.stderr.decode())
        exit(1)
    elif is_loaded_output.stdout:
        print("Unloading existing module...")
        retries = 3
        MAX_RETRIES = retries
        while retries > 0:
            rm_process = subprocess.run('sudo rmmod tcp_cubic_cr', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            if rm_process.returncode != 0 or rm_process.stderr:
                print(rm_process.stderr.decode())
                if retries > 0:
                    retries -= 1
                    print(f"Retrying to unload module... {MAX_RETRIES - retries}")
                    time.sleep(3)
                    continue
                print(f"Gave up after {MAX_RETRIES} attempts.")
                exit(1)
            break
        subprocess.run('sudo rm -rf /vagrant/module_params.info', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print("Done")

    parser = argparse.ArgumentParser()
    parser.add_argument('--module_dir', help='Directory where source for the new module and Makefile to build it are present', required=True)
    parser.add_argument('--IW', help='Initial window value. Optional. Default is 10', default=10)
    parser.add_argument('--USE_JUMP', help='Instructs whether module should Jump to a specified value after the first IW ACKs are received. Default is 0', default=0)
    parser.add_argument('--JUMP', help='The CWND value to jump to after the first IW ACKs are received if USE_JUMP is not 0', default=0)
    parser.add_argument('--hystart', help='Instructs whether the Hybrid Start (HyStart) algorithm should be used', default=1)

    arguments = parser.parse_args()
    module_load_output = subprocess.run(
        f'cd {arguments.module_dir} && sudo make install_cubic_cr IW={arguments.IW} USE_JUMP={arguments.USE_JUMP} JUMP={arguments.JUMP} hystart={arguments.hystart}',
        shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if module_load_output.returncode != 0 or module_load_output.stderr:
        print(module_load_output.stderr.decode())
        exit(1)
    with open('/vagrant/module_params.info', 'w') as f:
        json.dump(vars(arguments), f)
    print(module_load_output.stdout.decode())