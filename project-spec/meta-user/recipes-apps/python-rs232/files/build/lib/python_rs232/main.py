from datetime import datetime
from pathlib import Path

from python_rs232.utils.serial import PosixSerial

UART_DEVICE = "/dev/ttyPS0"
UART_BUFFER_SIZE = 255

def _get_recording_path():
    """Determine recording file path"""
    rec_dir = Path("recordings")
    rec_dir.mkdir(exist_ok=True) # create folder if doesn't exists yet

    current_time_str = datetime.now().strftime("%d-%m-%Y_%H-%M-%S")
    rec_name = f"{current_time_str}.txt"

    rec_path = rec_dir.joinpath(rec_name)

    return rec_path

def main():
    print(f"Listening for serial connection at: {UART_DEVICE}")

    with PosixSerial(UART_DEVICE) as ser:
        print("Serial device is connected")

        rec_path = _get_recording_path()
        with open(rec_path, 'a', encoding='utf-8') as f:
            while True:
                buffer = ser.read(UART_BUFFER_SIZE)

                if not buffer: # End of file
                    break

                print("> " + buffer)
                f.write(buffer)
            
            print("\nFinished reading, closing resources...")
    
    print("Finish")


if __name__ == "__main__":
    main()
