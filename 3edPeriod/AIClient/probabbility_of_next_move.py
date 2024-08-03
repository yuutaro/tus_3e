import socket
import pickle
import numpy as np
import select

def calculate_next_move_probabilities(board):
    # 元の仮の処理をそのまま使用
    probabilities = np.random.rand(8, 8)
    return probabilities

def handle_client(conn):
    while True:
        # select を使用してノンブロッキングで受信を待つ
        ready = select.select([conn], [], [], 1.0)
        if ready[0]:
            try:
                data = conn.recv(4096)
                if not data:
                    print("Client disconnected")
                    break
                
                board = pickle.loads(data)
                print(f"Received board: {board}")

                probabilities = calculate_next_move_probabilities(board)
                print(f"Calculated probabilities: {probabilities}")

                response = pickle.dumps(probabilities)
                conn.sendall(response)
                print("Sent probabilities")
            except Exception as e:
                print(f"Error handling client: {e}")
                break
    conn.close()

def start_server(host='localhost', port=30001):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen(1)
        print(f"Server started on {host}:{port}, waiting for connection...")
        while True:
            conn, addr = s.accept()
            print(f"Connected by {addr}")
            handle_client(conn)
            print("Client disconnected, waiting for new connection...")

if __name__ == "__main__":
    start_server()