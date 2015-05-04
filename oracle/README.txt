Following files are in the package:
1) README: instruction to install and run the service
2) testServer shell script: This is a start up script for the testServer service.
3) testServer.c: TCP/IP server program written in C.
4) client.py: TCP/IP client written in python.

Steps to Install:
Install RHEL with default configuration.
Place testServer, testServer.c and client.py under /home directory in RHEL system.
copy testServer script file under /etc/init.d/

Configuration:
Configure the testServer process as service
	chkconfig --add testServer --level 3
Compile testServer.c which is under /home directory with below command:
	gcc testServer.c -lpthread -o testServer
Run the below configuration:
	chkconfig testServer on
	service testServer start


