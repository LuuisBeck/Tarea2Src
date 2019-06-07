
public class TestSuffix {

	public static void main(String[] args) {
		String text;
		SuffixTree t=new SuffixTree();
		
		text= "GEEKSFORGEEKS$";  
		t.setText(text);
	    t.buildSuffixTree();     
	    System.out.println("Text: GEEKSFORGEEKS, Pattern to search: GEEKS"); 
	    t.checkForSubString("GEEKS"); 
	    System.out.println("\n\nText: GEEKSFORGEEKS, Pattern to search: GEEK1"); 
	    t.checkForSubString("GEEK1"); 
	    System.out.println("\n\nText: GEEKSFORGEEKS, Pattern to search: FOR"); 
	    t.checkForSubString("FOR"); 
	    //Free the dynamically allocated memory 
	    t.freeSuffixTreeByPostOrder(t.getRoot()); 
	}
}
