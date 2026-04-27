"""
Linux wrapper for serial communication through a device file

This module is a wrapper around termios API
Read about termios: https://docs.python.org/3/library/termios.html
"""

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
        attrs = termios.tcgetattr(self._fd) # [iflag, oflag, cflag, lflag, ispeed, ospeed, cc] 

        # Configure baud rate
        attrs[4] = BAUD_RATE  # output speed
        attrs[5] = BAUD_RATE  # input speed

        # Configure: 8N1 (8 data bits, no parity, 1 stop bit)
        attrs[2] &= ~termios.PARENB;                # PARENBN = Parity Enable (Disabled)
        attrs[2] &= ~termios.CSTOPB;                # CSTOPB = C Stop Bits (one stop bit), when enabled - 2 stop bits
        attrs[2] &= ~termios.CSIZE;                 # CSIZE = Character Size (Clear CS Flags for the next line).
        attrs[2] |= termios.CS8;                    # CS = Character Size (8 bits per chunk)
        attrs[2] |= termios.CREAD | termios.CLOCAL; # C = Channel (enable Read and Write)
    
        # Enable raw mode (non-canonical mode)
        # In short - let the data flow without terminal interruptions (like pressing the Backspace key).
        attrs[3] &= ~(termios.ICANON | termios.ECHO | termios.ECHOE | termios.ISIG)    # Enable NON CANONICAL Mode for Serial Port Comm
        attrs[0] &= ~(termios.IXON | termios.IXOFF | termios.IXANY)                    # Turn OFF software based flow control (XON/XOFF)
        attrs[0] &= ~(termios.ICRNL | termios.INLCR | termios.IGNCR)
        attrs[1] &= ~termios.OPOST

        attrs[6][termios.VMIN] = 1  # return from 'read' only after getting at least 1 byte
        attrs[6][termios.VTIME] = 0 # return from 'read' after VTIME*0.1 sec passed without input

        termios.tcsetattr(self._fd, termios.TCSANOW, attrs)
    
    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc, tb):
        self.close_connection()
        return False
    
    def read(self, size: int) -> bytes:
        """Read <num_bytes> """
        return os.read(self._fd, size)

    def read_line(self):
        raise NotImplementedError

    def write(self, data: bytes):
        os.write(self._fd, data)
    
    def get_fd(self):
        return self._fd
    
    def close_connection(self):
        os.close(self._fd)
