import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.net.*;

class OthelloClient2 extends JFrame {
  final static int BLACK = 1;
  final static int WHITE = -1;

  private JTextField tf;
  private JTextArea ta;
  private JLabel label;
  private OthelloCanvas canvas;
  private Socket socket;
  private PrintWriter out;
  private BufferedReader in;

  public OthelloClient2(String serverAddress, int serverPort) {
    try {
      socket = new Socket(serverAddress, serverPort);
      out = new PrintWriter(socket.getOutputStream(), true);
      in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    } catch (IOException e) {
      e.printStackTrace();
      System.exit(1);
    }

    this.setSize(640, 320);
    this.addWindowListener(new WindowAdapter() {
      public void windowClosing(WindowEvent e) {
        /* ウインドウが閉じられた時の処理 */
        out.println("CLOSE");
        System.exit(0);
      }
    });

    tf = new JTextField(40);
    tf.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        /* テキストフィールドに文字が入力された時の処理 */
        String message = tf.getText();
        if (message.equals("quit")) {
          out.println("CLOSE");
          System.exit(0);
        } else if (message.startsWith("NICK ")) {
          out.println(message);
        } else if (message.startsWith("SAY ")) {
          out.println(message);
        } else {
          ta.append("Unknown command.\n");
        }
        tf.setText(""); // テキストフィールドの文字を初期化
      }
    });

    ta = new JTextArea(18, 40);
    ta.setLineWrap(true);
    ta.setEditable(false);
    label = new JLabel();

    JPanel mainp = (JPanel) getContentPane();
    JPanel ep = new JPanel();
    JPanel wp = new JPanel();
    canvas = new OthelloCanvas(out);
    GridLayout gl = new GridLayout(1, 2);
    gl.setHgap(5);
    mainp.setLayout(gl);
    ep.setLayout(new BorderLayout());
    ep.add(new JScrollPane(ta), BorderLayout.CENTER);
    ep.add(tf, BorderLayout.SOUTH);
    wp.setLayout(new BorderLayout());
    wp.add(label, BorderLayout.SOUTH);
    wp.add(canvas, BorderLayout.CENTER);
    mainp.add(wp);
    mainp.add(ep);
    this.setVisible(true);

    // 受信部分追加
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
    if (message.startsWith("BOARD ")) {
      String[] tokens = message.split(" ");
      byte[][] board = new byte[8][8];
      for (int i = 1; i < tokens.length; i++) {
        board[(i - 1) / 8][(i - 1) % 8] = Byte.parseByte(tokens[i]);
      }
      canvas.updateBoard(board);
    } else if (message.startsWith("TURN ")) {
      int turn = Integer.parseInt(message.split(" ")[1]);
      label.setText("Turn: " + (turn == BLACK ? "BLACK" : "WHITE"));
    } else if (message.startsWith("SAY ")) {
      ta.append(message.substring(4) + "\n");
    } else if (message.startsWith("END ")) {
      ta.append("Game Over: " + message.substring(4) + "\n");
    }
  }

  public static void main(String args[]) {
    if (args.length != 2) {
      System.out.println("Usage: java OthelloClient <server address> <server port>");
      System.exit(1);
    }
    new OthelloClient2(args[0], Integer.parseInt(args[1]));
  }
}

class OthelloCanvas extends JPanel {
  private final static int startx = 20;
  private final static int starty = 10;
  private final static int gap = 30;
  private byte[][] board = {
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 1, -1, 0, 0, 0 },
      { 0, 0, 0, -1, 1, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 }
  }; // サンプルデータ
  private PrintWriter out;

  public OthelloCanvas(PrintWriter out) {
    this.out = out;
    this.addMouseListener(new MouseAdapter() {
      public void mousePressed(MouseEvent e) {
        /* 盤目上でマウスがクリックされた時の処理 */
        Point p = e.getPoint();
        int x = (p.x - startx) / gap;
        int y = (p.y - starty) / gap;
        if (x >= 0 && x < 8 && y >= 0 && y < 8) {
          out.println("PUT " + x + " " + y);
        }
      }
    });
  }

  public void updateBoard(byte[][] board) {
    this.board = board;
    repaint();
  }

  public void paintComponent(Graphics g) {
    super.paintComponent(g);
    g.setColor(new Color(0, 180, 0));
    g.fillRect(startx, starty, gap * 8, gap * 8);

    g.setColor(Color.BLACK);
    for (int i = 0; i < 9; i++) {
      g.drawLine(startx, starty + i * gap, startx + 8 * gap, starty + i * gap);
      g.drawLine(startx + i * gap, starty, startx + i * gap, starty + 8 * gap);
    }
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (board[i][j] == OthelloClient2.BLACK) {
          g.setColor(Color.BLACK);
          g.fillOval(startx + gap * i, starty + gap * j, gap, gap);
        } else if (board[i][j] == OthelloClient2.WHITE) {
          g.setColor(Color.WHITE);
          g.fillOval(startx + gap * i, starty + gap * j, gap, gap);
        }
      }
    }
  }
}
