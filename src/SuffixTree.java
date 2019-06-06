
public class SuffixTree {
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
	
	
}



