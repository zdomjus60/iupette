import socket
import json

HOST = '127.0.0.1'  # L'hostname o l'indirizzo IP del server
PORT = 8080         # La porta utilizzata dal server

def send_json_message(message):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(json.dumps(message).encode('utf-8'))
        data = s.recv(1024)
        return data.decode('utf-8')

if __name__ == "__main__":
    test_message = {"command": "get_status", "widget_id": "combobox_1"}
    print(f"Invio: {json.dumps(test_message)}")
    response = send_json_message(test_message)
    print(f"Ricevuto: {response}")

    test_message_2 = {"command": "set_text", "widget_id": "textbox_2", "value": "New Text"}
    print(f"Invio: {json.dumps(test_message_2)}")
    response_2 = send_json_message(test_message_2)
    print(f"Ricevuto: {response_2}")
