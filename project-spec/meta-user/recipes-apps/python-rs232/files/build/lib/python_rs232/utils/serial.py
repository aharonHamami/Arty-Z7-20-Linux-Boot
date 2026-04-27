"""Linux wrapper for serial communication through a device file"""

import os
import termios

BAUD_RATE = termios.B9600

class PosixSerial:
    device: str
    #baud_rate: int
    #timeout: int
    _fd: int

    def __init__(self, device: str):
        """
        Create a serial communication

        May raise FileNotFoundError error
        """
        self.device = device
        #self.baud_rate = baud_rate
        #self.timeout = timeout
        self._fd = -1

        self._open_connection()
    
    def _open_connection(self):
        """
        Open a file for the connection
        
        May raise FileNotFoundError error 
        """
        self._fd = os.open(self.device, os.O_RDWR | os.O_NOCTTY | os.O_SYNC)

        # Get the tty attributes for file descriptor fd.
        attrs = termios.tcgetattr(self._fd)

        # Configure baud rate (e.g., 9600)
        attrs[4] = BAUD_RATE  # output speed
        attrs[5] = BAUD_RATE  # input speed

        # Configure: 8N1 (8 data bits, no parity, 1 stop bit)
        attrs[2] &= ~termios.PARENB;                # PARENBN = Parity Enable (Disabled)
        attrs[2] &= ~termios.CSTOPB;                # CSTOPB = C Stop Bits (one stop bit), when enabled - 2 stop bits
        attrs[2] &= ~termios.CSIZE;                 # CSIZE = Character Size (Clear CS Flags for the next line).
        attrs[2] |= termios.CS8;                    # CS = Character Size (8 bits per chunk)
        attrs[2] |= termios.CREAD | termios.CLOCAL; # C = Channel (enable Read and Write)
    
    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc, tb):
        self.close_connection()
        return False
    
    def read(self, size: int):
        """Read <num_bytes> """
        os.read(self._fd, size)

    def read_line(self):
        raise NotImplementedError

    def write(self, data: str):
        os.write(self._fd, data)
    
    def get_fd(self):
        return self._fd
    
    def close_connection(self):
        os.close(self._fd)
