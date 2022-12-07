MyServer.java:
import java.io.*;  
import java.net.*;  
public class MyServer {  
public static void main(String[] args) throws Exception{  

// Socket(), ServerSocket() used -- TCP connection
// DatagramSocket(), DatagramPacket() -- UDP connection
ServerSocket ss=new ServerSocket(6666);  
Socket s=ss.accept();//establishes connection   
DataInputStream din=new DataInputStream(s.getInputStream());  
DataOutputStream dout=new DataOutputStream(s.getOutputStream());  
BufferedReader br=new BufferedReader(new InputStreamReader(System.in));  
  
String str="",str2="";  
while(!str.equals("stop"))
{  
str=din.readUTF();  
System.out.println("client says: "+str);  
str2=br.readLine();  
dout.writeUTF(str2);  
dout.flush();  
}  
din.close();
s.close(); 
ss.close();
}
} 

MyClient.java:
import java.io.*;  
import java.net.*;  
public class MyClient {  
public static void main(String[] args) throws Exception {  
     
Socket s=new Socket("localhost",6666);
DataInputStream din = new DataInputStream(s.getInputStream());
DataOutputStream dout=new DataOutputStream(s.getOutputStream());
BufferedReader br=new BufferedReader(new InputStreamReader(System.in));

String str="",str2="";
while(!str.equals("stop"))
{
	str=br.readLine();
	dout.writeUTF(str);  
	dout.flush();  
str2=din.readUTF(); 
System.out.println("Server says: "+str2);
}

dout.close();
s.close();
}
}