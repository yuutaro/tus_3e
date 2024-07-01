import java.net.*;
import java.io.*;

class TcpClient {
  public static void main(String args[]) {
    int c;
    Socket s;
    InputStream sIn;

    if (args.length != 1) {
      System.out.println("No hostname given");
      System.exit(1);
    }

    try {
      s = new Socket(args[0], 4321);

      sIn = s.getInputStream();
      while ((c = sIn.read()) != -1) {
        System.out.print((char) c);
      }
      sIn.close();
      s.close();
    } catch (IOException e) {
      System.err.println("Caught IOException");
      System.exit(1);
    }
  }
}