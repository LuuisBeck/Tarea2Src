
public class Suffix_Node {
	private int MAX_CHAR=256;
	private Suffix_Node[] children;
	private Suffix_Node suffixLink;
	private int start;
	private int end;
	private int suffixIndex;
	
	public Suffix_Node(int max_char, int start, int end, Suffix_Node root) {
		this.MAX_CHAR = max_char;
		this.children = new Suffix_Node[MAX_CHAR];
		for (int i=0; i<this.MAX_CHAR; i++) {
			this.children[i] = null;
		}
		
		this.setSuffixLink(root);
		this.setStart(start);
		this.setEnd(end);
		this.setSuffixIndex(-1);
	}

	public int getStart() {
		return start;
	}

	public Suffix_Node getChildren(int i) {
		return children[i];
	}

	public void setChildren(int i, Suffix_Node children) {
		this.children[i] = children;
	}

	public void setStart(int start) {
		this.start = start;
	}

	public int getEnd() {
		return end;
	}

	public void setEnd(int end) {
		this.end = end;
	}

	public int getSuffixIndex() {
		return suffixIndex;
	}

	public void setSuffixIndex(int suffixIndex) {
		this.suffixIndex = suffixIndex;
	}

	public Suffix_Node getSuffixLink() {
		return suffixLink;
	}

	public void setSuffixLink(Suffix_Node suffixLink) {
		this.suffixLink = suffixLink;
	}
	
	
}
