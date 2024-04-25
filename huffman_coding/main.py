import socket
import tempfile

import huffman as hm
import file_utils as fu

if __name__ == '__main__':
    while True:
        choice = str(input('Choose an action:' +
                           '\n' + '1 – encode message' +
                           '\n' + '2 – decode message' +
                           '\n' + '3 – exit the program' +
                           '\n'))
        if choice == '1':
            filename = str(input('Enter filename: '))
            with open(filename, 'r') as input_file:
                message = str(input_file.read())

            min_heap = hm.get_huffman_tree(message)
            encoded_message = hm.encode_message(message)
            file_to_send = tempfile.NamedTemporaryFile(delete=False)
            fu.write_to_file(file_to_send.name, min_heap, encoded_message)
            server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            print('Socket successfully created')
            host = ''
            port = 8080
            server_socket.bind((host, port))
            server_socket.listen(1)
            client_socket, addr = server_socket.accept()
            with open(file_to_send.name, mode='rb') as input_file:
                data = input_file.read()
                if not data:
                    break
                while data:
                    client_socket.send(data)
                    data = input_file.read()

            client_socket.close()
            server_socket.close()
            file_to_send.close()

        elif choice == '2':
            client_socket = socket.socket()
            print('Socket successfully created')
            host = str(input('Enter host to connect to: '))
            port = 8080
            client_socket.connect((host, port))
            received_file = tempfile.NamedTemporaryFile(delete=False)
            data = client_socket.recv(1024)
            with open(received_file.name, 'wb') as output_file:
                while data:
                    if not data:
                        break
                    else:
                        output_file.write(data)
                        data = client_socket.recv(1024)

            min_heap, encoded_message = fu.read_from_file(received_file.name)
            decoded_message = hm.decode_message(min_heap, encoded_message)
            filename = str(input('Enter filename: '))
            with open(filename, 'w') as output_file:
                output_file.write(decoded_message)

        elif choice == '3':
            break

        else:
            print('\n' + 'Invalid choice' + '\n')
