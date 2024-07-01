package others;

import java.net.*;
import java.io.*;

public class UdpSend {

  UdpSend(String args[]) {
    if (args.length != 1) {
      System.exit(1);
    }
    String sendString = "Hello !!!";
    byte message[] = sendString.getBytes();
    try {
      InetAddress address = InetAddress.getByName(args[0]);
      DatagramPacket packet = new DatagramPacket(message, message.length, address, 4321);
      DatagramSocket s = new DatagramSocket();
      s.send(packet);
      s.close();
    } catch (IOException e) {
      System.err.println("" + e);
    }
  }

  public static void main(String args[]) {
    new UdpSend(args);
  }
}