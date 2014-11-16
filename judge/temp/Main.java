import java.util.Scanner;
public class Main {    //注：用Java提交时类名必须为"Main"
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        while (in.hasNextInt()) {
            int a = in.nextInt();
            int b = in.nextInt();
            System.out.println(a + b);
        }
        while (true) {}
    }
}