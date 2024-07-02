import java.net.Socket;
import java.util.ArrayList;
import java.util.Random;
import java.io.PrintWriter;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectOutputStream;
import java.io.ObjectInputStream;

public class AIClient {
  final static int BLACK = 1;
  final static int WHITE = -1;

  private Socket socket;
  private PrintWriter out;
  private BufferedReader in;

  private Process pythonProcess;
  private Socket aiSocket;
  private ObjectOutputStream aiOut;
  private ObjectInputStream aiIn;

  private int myColor;
  private int currentTurn;
  private int[][] board = new int[8][8];

  Random random = new Random();

  public AIClient(String serverAddress, int serverPort) {
    try {
      // オセロゲームサーバーとの通信用ソケットとIO
      socket = new Socket(serverAddress, serverPort);
      out = new PrintWriter(socket.getOutputStream(), true);
      in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      out.println("NICK 6322087_Mikasa");
    } catch (Exception e) {
      System.out.println("オセロサーバーに接続できませんでした。");
      System.exit(1);
    }

    try {
      ProcessBuilder pb = new ProcessBuilder("python3", "probabbility_of_next_move.py");
      pythonProcess = pb.start();
    } catch (Exception e) {
      System.out.println("Pythonプロセスを起動できませんでした。");
      System.exit(1);
    }

    try {
      aiSocket = new Socket("localhost", 65432);
      aiOut = new ObjectOutputStream(aiSocket.getOutputStream());
      aiIn = new ObjectInputStream(aiSocket.getInputStream());
    } catch (Exception e) {
      System.out.println("AIサーバーに接続できませんでした。");
      System.exit(1);
    }

    // サーバーからのオセロ対戦通信受信部分追加
    new Thread(new Runnable() {
      public void run() {
        try {
          String line;
          while ((line = in.readLine()) != null) {
            handleServerMessage(line);
          }
        } catch (IOException e) {
          e.printStackTrace();
        }
      }
    }).start();
  }

  private void handleServerMessage(String message) {
    String command = message.split(" ")[0];

    switch (command) {
      case "START":
        String[] startTokens = message.split(" ");
        myColor = Integer.parseInt(startTokens[1]);
        System.out.println("自分の色: " + (myColor == BLACK ? "BLACK" : "WHITE") + "\n");
        break;

      case "BOARD":
        String[] boardTokens = message.split(" ");
        for (int i = 1; i < boardTokens.length; i++) {
          board[(i - 1) / 8][(i - 1) % 8] = Byte.parseByte(boardTokens[i]);
        }
        System.out.println("現在の盤面\n");
        for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
            if (board[i][j] == BLACK) {
              System.out.print("○ ");
            } else if (board[i][j] == WHITE) {
              System.out.print("● ");
            } else {
              System.out.print("- ");
            }
          }
          System.out.print("\n");
        }
        System.out.println();
        break;

      case "TURN":
        int turn = Integer.parseInt(message.split(" ")[1]);
        currentTurn = turn;
        System.out.println("現在のターン: " + (currentTurn == BLACK ? "BLACK" : "WHITE") + "\n");

        // 自動で石を置く処理
        if (currentTurn == myColor) {
          try {
            int[] pos = getMoveFromAI(board);
            if (pos != null) {
              out.println("PUT " + pos[0] + " " + pos[1]);
            } else {
              System.out.println("No valid move available.");
            }
          } catch (Exception e) {
            e.printStackTrace();
          }
        }
        break;

      case "SAY":
        System.out.println("メッセージ" + message.substring(4) + "\n");
        break;

      case "ERROR":
        String errorCode = message.split(" ")[1];
        switch (errorCode) {
          case "1":
            System.out.println("書式が間違っています。\n");
            break;
          case "2":
            System.out.println("選択した番目には自身の色の石は置けません。\n");
            break;
          case "3":
            System.out.println("相手のターンです。\n");
            break;
          case "4":
            System.out.println("処理できない命令です。\n");
            break;
          default:
            System.out.println("未知のエラーが発生しました。\n");
            break;
        }
        break;

      case "NICK":
        String nickname = message.substring(5);
        System.out.println("ニックネームが" + nickname + "に設定されました。\n");
        break;

      case "END":
        System.out.println("ゲーム終了: " + message.substring(4) + "\n");
        break;

      case "CLOSE":
        System.out.println("対戦相手とサーバーとの接続が切断されました。\n");
        break;

      default:
        System.out.println("不明なメッセージが送られてきました。\n");
        System.out.println(message + "\n");
        break;
    }
  }

  // Pythonサーバーにボードの状態を送信して次の手の確率を受信
  private int[] getMoveFromAI(int[][] board) throws IOException, ClassNotFoundException {
    aiOut.writeObject(board);
    aiOut.flush();

    double[][] probabilities = (double[][]) aiIn.readObject();
    System.out.println("Received probabilities from AI server:");

    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        System.out.print(String.format("%.2f ", probabilities[i][j]));
      }
      System.out.println();
    }

    // 確率の高い場所を選択
    int[] bestMove = null;
    double bestProbability = -1;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (probabilities[i][j] > bestProbability) {
          bestProbability = probabilities[i][j];
          bestMove = new int[]{i, j};
        }
      }
    }

    return bestMove;
  }

    // 石が置けるかどうかを判定する関数
    private boolean isValidMove(int[][] board, int x, int y, int color) {
      // すでに石が置かれている場所には置けない
      if (board[x][y] != 0) {
        return false;
      }
  
      int opponent = -color;
  
      // 8方向をチェック
      int[][] directions = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };
  
      for (int[] dir : directions) {
        int dx = dir[0];
        int dy = dir[1];
        int nx = x + dx;
        int ny = y + dy;
  
        // 隣が相手の石かチェック
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && board[nx][ny] == opponent) {
          nx += dx;
          ny += dy;
          // その方向にさらに進む
          while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            if (board[nx][ny] == color) {
              // 自分の石で挟めていればtrue
              return true;
            } else if (board[nx][ny] == 0) {
              // 空白マスに到達したらこの方向は無効
              break;
            }
            nx += dx;
            ny += dy;
          }
        }
      }
  
      // どの方向でも挟めなかった
      return false;
    }
  
    // 全マスをチェックして置ける場所を二次元配列で返す関数
    private int[][] getValidMoves(int[][] board, int color) {
      int[][] validMoves = new int[8][8];
  
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          if (isValidMove(board, i, j, color)) {
            validMoves[i][j] = 1;
          }
        }
      }
  
      return validMoves;
    }
  
    // とりあえずランダムに石を置く関数
    private int[] randomPut(int[][] board, int color) {
      int[][] validMoves = getValidMoves(board, color);
      ArrayList<int[]> movesList = new ArrayList<>();
  
      // デバッグ用の表示と有効な手のリスト作成
      System.out.println("置ける位置:");
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          System.out.print(validMoves[i][j] + " ");
          if (validMoves[i][j] == 1) {
            movesList.add(new int[] { i, j });
          }
        }
        System.out.println();
      }
      System.out.println();
  
      // 有効な手がない場合
      if (movesList.isEmpty()) {
        System.out.println("置ける位置がありません。");
        return null;
      }
  
      // ランダムに選択
      Random random = new Random();
      int randomIndex = random.nextInt(movesList.size());
      int[] selectedMove = movesList.get(randomIndex);
  
      System.out.println("選択された位置: (" + selectedMove[0] + ", " + selectedMove[1] + ")");
  
      return selectedMove;
    }

  public static void main(String args[]) {
    if (args.length != 2) {
      System.out.println("Usage: java AIClient <server address> <server port>");
      System.exit(1);
    }
    new AIClient(args[0], Integer.parseInt(args[1]));
  }
}
