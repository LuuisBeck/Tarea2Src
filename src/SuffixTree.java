
public class SuffixTree {
	private int MAX_CHAR=256;
	private String text;
	private Suffix_Node root= null;
	private Suffix_Node lastNewNode = null;
	private Suffix_Node activeNode = null;
	private int activeEdge= -1;
	private int activeLength =0;
	
	private int remainingSuffixCount = 0; 
	private int leafEnd = -1; 
	private int rootEnd[] = null; 
	private int splitEnd[] = null; 
	private int size = -1; //Length of input string 
	

	public int edgeLength(Suffix_Node n) {
		if (n.equals(root)) {
			return 0;
		}
		return n.getEnd() - n.getStart() + 1;
	}
	
	public boolean walkDown(Suffix_Node currentNode) {
		if (activeLength >= edgeLength(currentNode)) {
			activeEdge += edgeLength(currentNode);
			activeLength -= edgeLength(currentNode);
			activeNode = currentNode;
			return true;
		}
		return false;
	}
	
	public void extendSuffixTree(int pos) {
		leafEnd = pos;
		remainingSuffixCount++;
		
		while (remainingSuffixCount >0) {
			if(activeLength == 0) {
				activeEdge = pos;
			}
			if(activeNode.getChildren(text.charAt(activeEdge))==null) {
				Suffix_Node n= new Suffix_Node(256, pos, leafEnd, root );
				activeNode.setChildren(text.charAt(activeEdge), n);
				if (lastNewNode != null) {
					lastNewNode.setSuffixLink(activeNode);
					lastNewNode=null;
				}
			}
			else {
				Suffix_Node next = activeNode.getChildren(text.charAt(activeEdge));
				
				if(walkDown(next)) {
					continue;
				}
				if (text.charAt(next.getStart() + activeLength)==text.charAt(pos)) {
					if(lastNewNode!=null && activeNode!=root) {
						lastNewNode.setSuffixLink(activeNode);
						lastNewNode=null;
					}
					activeLength++;
					break;
				}
				int splitEnd = next.getStart()+activeLength -1;
				
				Suffix_Node split = new Suffix_Node(256, next.getStart(), splitEnd, root );
				activeNode.setChildren(text.charAt(activeEdge), split);
				Suffix_Node n_2 = new Suffix_Node(256, pos, leafEnd, root );
				split.setChildren(text.charAt(pos), n_2);
				next.setStart(next.getStart()+activeLength);
				split.setChildren(text.charAt(next.getStart()), next);
				
				if(lastNewNode!=null) {
					lastNewNode.setSuffixLink(split);
				}
				
				lastNewNode=split;
			}
			
			remainingSuffixCount--;
			
			if(activeNode==root && activeLength>0) {
				activeLength--;
				activeEdge=pos-remainingSuffixCount+1;
			}
			
			else if(activeNode!=root){
				activeNode = activeNode.getSuffixLink();
			}
			
			
		}
	}
	
	public void print(int i, int j) {
		int k;
		for(k=i; k<=j; k++ ) {
			System.out.print(text.charAt(k));
		}
	}
	
	public void setSuffixIndexByDFS(Suffix_Node n, int labelHeight)
	{
		if (n == null) return;

		if (n.getStart() != -1) 
		{
			// print(n->start, *(n->end));
		}
		int leaf = 1;
		int i;
		for (i = 0; i < MAX_CHAR; i++)
		{
			if (n.getChildren(i) != null)
			{
				//Uncomment below two lines to print suffix index
			// if (leaf == 1 && n->start != -1)
				// printf(" [%d]\n", n->suffixIndex);

				//Current node is not a leaf as it has outgoing
				//edges from it.
				leaf = 0;
				setSuffixIndexByDFS(n.getChildren(i), labelHeight +
									edgeLength(n.getChildren(i)));
			}
		}
		if (leaf == 1)
		{
			n.setSuffixIndex(size - labelHeight);
			//Uncomment below line to print suffix index
			//printf(" [%d]\n", n->suffixIndex);
		} 
	}
	
	public void freeSuffixTreeByPostOrder(Suffix_Node n)
	{
		if (n == null)
			return;
		int i;
		for (i = 0; i < MAX_CHAR; i++)
		{
			if (n.getChildren(i) != null)
			{
				freeSuffixTreeByPostOrder(n.getChildren(i));
			}
		}
		/*if (n.getSuffixIndex() == -1) {
			free(n->end);
		}
		free(n);*/
	}

	/*Build the suffix tree and print the edge labels along with
	suffixIndex. suffixIndex for leaf edges will be >= 0 and
	for non-leaf edges will be -1*/
	public void buildSuffixTree()
	{
		size = text.length();
		int i;
		int rootEnd = -1;

		/*Root is a special node with start and end indices as -1,
		as it has no parent from where an edge comes to root*/
		root = new Suffix_Node(256, -1, rootEnd, null);

		activeNode = root; //First activeNode will be root
		for (i=0; i<size; i++)
			extendSuffixTree(i);
		int labelHeight = 0;
		setSuffixIndexByDFS(root, labelHeight);
	}
	
	int traverseEdge(String str, int idx, int start, int end)
	{
		int k = 0;
		//Traverse the edge with character by character matching
		for(k=start; k<=end && str.charAt(idx) != '\0'; k++, idx++)
		{
			if(text.charAt(k) != str.charAt(idx))
				return -1; // mo match
		}
		if(str.charAt(idx) == '\0')
			return 1; // match
		return 0; // more characters yet to match
	}
	
	int doTraversalToCountLeaf(Suffix_Node n)
	{
		if(n == null)
			return 0;
		if(n.getSuffixIndex() > -1)
		{
			System.out.println("\nFound at position: " + n.getSuffixIndex());
			return 1;
		}
		int count = 0;
		int i = 0;
		for (i = 0; i < MAX_CHAR; i++)
		{
			if(n.getChildren(i) != null)
			{
				count += doTraversalToCountLeaf(n.getChildren(i));
			}
		}
		return count;
	}
	
	int countLeaf(Suffix_Node n)
	{
		if(n == null)
			return 0;
		return doTraversalToCountLeaf(n);
	}
	
	int doTraversal(Suffix_Node n, String str, int idx)
	{
		if(n == null)
		{
			return -1; // no match
		}
		int res = -1;
		//If node n is not root node, then traverse edge
		//from node n's parent to node n.
		if(n.getStart() != -1)
		{
			res = traverseEdge(str, idx, n.getStart(), n.getEnd());
			if(res == -1) //no match
				return -1;
			if(res == 1) //match
			{
				if(n.getSuffixIndex() > -1)
					System.out.print("\nsubstring count: 1 and position: " + 
								n.getSuffixIndex());
				else
					System.out.print("\nsubstring count: " + countLeaf(n));
				return 1;
			}
		}
		//Get the character index to search
		idx = idx + edgeLength(n);
		//If there is an edge from node n going out
		//with current character str[idx], traverse that edge
		if(n.getChildren(str.charAt(idx)) != null)
			return doTraversal(n.getChildren(str.charAt(idx)), str, idx);
		else
			return -1; // no match
	}
	
	void checkForSubString(String str)
	{
		int res = doTraversal(root, str, 0);
		if(res == 1)
			System.out.print("\nPattern " + str + " is a Substring\n");
		else
			System.out.print("\nPattern " + str + " is NOT a Substring\n");
	}
}



