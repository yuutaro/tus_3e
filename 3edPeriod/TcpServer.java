
import java.net.*;
import java.io.*;

class TcpServer {
  public static void main(String args[]) {
    ServerSocket ss = null;
    Socket s;
    String sendString = "Hello Net World! via TCP\n";
    int slength;
    OutputStream sout;

    try {
      ss = new ServerSocket(4321, 300);
      while (true) {
        s = ss.accept();
        sout = s.getOutputStream();
        slength = sendString.length();
        for (int i = 0; i < slength; i++) {
          sout.write((int) sendString.charAt(i));
        }
        sout.close();
        s.close();
      }
    } catch (IOException e) {
      System.err.println("Caught IOException");
      System.exit(1);
    }
  }
}