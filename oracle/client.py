# TCP client which sends GETMESSAGE requests
# and gets random strings from the server
# @author Ashwini

import socket
import sys
import logging
import logging.handlers
LOG_FILENAME = 'log_client.log'

# create logger
logger = logging.getLogger("client")
logger.setLevel(logging.DEBUG)

# create console handler and set level to debug
ch = logging.handlers.RotatingFileHandler(LOG_FILENAME, maxBytes=20000, backupCount=5)
ch.setLevel(logging.DEBUG)

# create formatter
formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")

# add formatter to ch
ch.setFormatter(formatter)

# add ch to logger
logger.addHandler(ch)

#old_stdout = sys.stdout

#main function
if __name__ == "__main__":

    if(len(sys.argv) < 4) :
        print 'Usage : python client.py hostname port number_of_requests'
        sys.exit()

    host = sys.argv[1]
    port = int(sys.argv[2])
    number_of_requests = int(sys.argv[3])

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = (host, port)
print >>sys.stderr, 'connecting to %s port %s' % server_address
sock.connect(server_address)

try:
# Send data
    i = 0
    result = []
    while i < number_of_requests: 
    	message = 'GETMESSAGE\n'
    	print >>sys.stderr, 'sending "%s"' % message
    	sock.sendall(message)

    	data = sock.recv(100)
	result.append(data)
    	print >>sys.stderr, 'received "%s"' % data
    	i += 1
    
    for s in result:
	logger.info(s)
finally:
    message = 'BYE\n'
    print >>sys.stderr, 'sending "%s"' % message
    sock.sendall(message)
    print >>sys.stderr, 'closing socket'
    sock.close()
