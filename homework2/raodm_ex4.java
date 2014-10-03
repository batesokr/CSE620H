import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.charset.StandardCharsets;
import java.io.IOException;
import java.lang.Character;

class raodm_ex4 {
    static String toggleCase(String str, final char low) {
        final char up = Character.toUpperCase(low);
        char[] data   = str.toCharArray();
        for(int i = 0; (i < data.length); i++) {
            final char c = data[i];
            data[i] = (c == low ? up : c == up ? low : c);
        }
        return new String(data);
    }
    
    public static void main(String args[]) throws IOException {
        if (args.length < 2) {
            System.out.println("Specify <File> <toggle_char> ...");
            System.exit(1);
        }
        // Load all the bytes into a string using Java's non-blocking
        // I/O package (same as Python's read() method). However, it
        // is a bit of a cheating because it is much faster than the
        // iterator-based solution in C++.
        byte[] rawData = Files.readAllBytes(Paths.get(args[0]));
        String data    = new String(rawData, StandardCharsets.US_ASCII);
        // Replace specified charachters
        for(int curr = 1; (curr < args.length); curr++) {
            data = toggleCase(data, args[curr].charAt(0));
        }
        // Print the modifed data.
        System.out.print(data);
    }
}
