/**
 * This class implements the LRU page-replacement strategy.
 *
 */
package virtualmem;
public class LRU extends ReplacementAlgorithm
{
	// LRU list of page frames
	private LRUList frameList;

	/**
	 * @param pageFrameCount - the number of physical page frames
	 */
	public LRU(int pageFrameCount) {
		super(pageFrameCount);
		frameList = new LRUList(pageFrameCount);
	}

	/**
	 * Insert a page into a page frame.
	 */
	public void insert(int pageNumber) {
		frameList.insert(pageNumber);
		frameList.dump();
    }
		
	class LRUList
	{
		// the page frame list
		int[] pageFrameList;

		// the number of elements in the page frame list
		int elementCount;

		// the last page inserted
		int lastInserted = -1;

		LRUList(int pageFrameCount) {
			pageFrameList = new int[pageFrameCount];

            // we initialize each entry to -1 to indicate initial value is invalid.
            java.util.Arrays.fill(pageFrameList,-1);
			elementCount = 0;
		}

		/**
		 * @param pageNumber the number of the page to be 
		 *	inserted into the page frame list.
		 */
		void insert(int pageNumber) {
			int pagePos = search(pageNumber);
			if (pagePos == -1) {
				++pageFaultCount;
				if (elementCount < pageFrameCount)
					++elementCount;
				pagePos = elementCount - 1;				
			}
			for (int i = pagePos; i > 0; --i)
				pageFrameList[i] = pageFrameList[i - 1];
			pageFrameList[0] = pageNumber;
			
			return;
        }

		void dump() {
			for (int i = 0; i < pageFrameList.length; i++)
				System.out.print("["+i+"]"+pageFrameList[i]+", ");
			System.out.println();
			System.out.println(" element count = " + elementCount);
		}


		/**
		 * Searches for page pageNumber in the page frame list
		 * @return non-negative integer if pageNumber was found
		 * @return -1 if pageNumber was not found
		 */
		int search(int pageNumber) {
			int returnVal = -1;

			for (int i = 0; i < pageFrameList.length; i++) {
				if (pageNumber == pageFrameList[i]) {
					returnVal = i;
					break;
				}
			}
			return returnVal;
		}
	}
}
