import java.net.*;
import java.io.*;

public class UdpReceive {
  UdpReceive() {
    byte buffer[] = new byte[40];
    try {
      DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
      DatagramSocket dsocket = new DatagramSocket(4321);
      while (true) {
        dsocket.receive(packet);
        String msg = new String(buffer, 0, packet.getLength());
        System.out.println(packet.getAddress().getHostName() + ": " + msg);
      }
    } catch (IOException e) {
      System.err.println("" + e);
    }
  }

  public static void main(String args[]) {
    new UdpReceive();
  }
}