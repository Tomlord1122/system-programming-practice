cmd_/home/tomlord/sys/hw2/Module.symvers := sed 's/\.ko$$/\.o/' /home/tomlord/sys/hw2/modules.order | scripts/mod/modpost -m -a  -o /home/tomlord/sys/hw2/Module.symvers -e -i Module.symvers   -T -
