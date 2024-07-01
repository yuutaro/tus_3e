package kadai.AIClient;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class AIClient {
  final static int BLACK = 1;
  final static int WHITE = -1;

  private Socket socket;
  private PrintWriter out;
  private BufferedReader in;

  // コンストラクタ
  public AIClient(String serverAddress, int serverPort) {
    try {
      socket = new Socket(serverAddress, serverPort);
      out = new PrintWriter(socket.getOutputStream(), true);
      in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    } catch (IOException e) {
      e.printStackTrace();
      System.exit(1);
    }

    new Thread(new Runnable() {
      public void run() {
        try {
          String line;
          while ((line = in.readLine()) != null) {
            // handleServerMessage(line);
          }
        } catch (IOException e) {
          e.printStackTrace();
        }
      }
    }).start();
  }

  private void handleServerMessage() {

  }

}
