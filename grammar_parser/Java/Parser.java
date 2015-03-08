import java.io.InputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.FileNotFoundException;

class Parser {
  private int la; /* One character lookahead */
  InputStream input;

  /**
    Read next input character into lookahead. la = -1 indicates end of file.
  */
  private void next() {
    try {
      la = input.read();
    }
    catch (IOException e) {
      la = -1;
    }
  }

  private void match(String matches) throws SyntaxErrorException {
    if (matches.indexOf(la)>=0) {
      next();
    }
    else {
      throw new SyntaxErrorException("Unexpected character");
    }
  }

  /**
    Skips whitespace characters until a non-whitespace character or end of file is reached.
  */
  private void skipSpaces() {
    while (Character.isWhitespace(la)) {
      next();
    }
  }

  public Parser(InputStream input) {
    this.input = input;
    next();
    skipSpaces();
  }

  /* ******************************************************************** */
  /*                     PUT YOUR PARSER RULES HERE                       */



  /* ******************************************************************** */

  public void parse_grammar() throws SyntaxErrorException {
    /* Calls your start rule here */
  }

  public static void main(String[] args) {
    if (args.length==1) {
      try {
        FileInputStream input = new FileInputStream(args[0]);
        Parser parser = new Parser(input);
        parser.parse_grammar();
      }
      catch (FileNotFoundException e) {
        System.err.println("Cannot open file "+args[0]);
      }
      catch (SyntaxErrorException e) {
        System.err.println("Syntax Error: "+e.getMessage());
      }
    }
    else {
      System.err.println("Usage: Parser FILENAME");
    }
  }
}
