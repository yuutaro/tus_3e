import socket
import pickle
import numpy as np

def calculate_next_move_probabilities(board):
  # 仮の処理関数。実際にはオセロの次の手の確率を計算する。
  probabilities = np.random.rand(8, 8)
  return probabilities

def start_server(host='localhost', port=65432):
  with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((host, port))
    s.listen()
    print("Server started, waiting for connection...")
    while True:
      try:
        conn, addr = s.accept()
        with conn:
          print(f"Connected by {addr}")
          data = b''
          while True:
            packet = conn.recv(4096)
            if not packet:
              break
            data += packet
            board = pickle.loads(data)
            print(f"Received board: {board}")

            # 次の手の確率を計算
            probabilities = calculate_next_move_probabilities(board)
            print(f"Calculated probabilities: {probabilities}")

            # 結果を送信
            response = pickle.dumps(probabilities)
            conn.sendall(response)
            print("Sent probabilities")
      except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
  start_server()