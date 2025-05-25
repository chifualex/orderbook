import socket
import os

HOST = '127.0.0.1'  # Replace with your server's IP
PORT = 5000

current_dir = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.join(current_dir, '../inputFile.csv')

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
    client_socket.connect((HOST, PORT))
    with open(file_path, 'rb') as f:
        while True:
            data = f.read(1024)
            if not data:
                break
            client_socket.sendall(data)
    print("File sent successfully.")
