/**
 * This class implements the FIFO page-replacement strategy.
 *
 */
package virtualmem;
public class FIFO extends ReplacementAlgorithm
{
	// FIFO list of page frames
	private FIFOList frameList;

	/**
	 * @param pageFrameCount - the number of physical page frames
	 */
	public FIFO(int pageFrameCount) {
		super(pageFrameCount);
		frameList = new FIFOList(pageFrameCount);
	}


	/**
	 * insert a page into a page frame.
	 * @param int pageNumber - the page number being inserted.
	 */
	public void insert(int pageNumber) {
		frameList.insert(pageNumber);
		frameList.dump();
    }
		
	class FIFOList
	{
		// the page frame list
		int[] pageFrameList;

		int first;

		// the number of elements in the page frame list
		int elementCount;

		FIFOList(int pageFrameCount) {
			pageFrameList = new int[pageFrameCount];

            // we initialize each entry to -1 to indicate initial value is invalid 
            java.util.Arrays.fill(pageFrameList,-1);
			elementCount = 0;
			first = 0;
		}

		/**
		 * @param pageNumber the number of the page to be 
		 *	inserted into the page frame list.
		 */
		void insert(int pageNumber) {
			// page exist
			if (search(pageNumber))
				return;
			++pageFaultCount;
			// the list is not full.
			if (elementCount < pageFrameCount) {
				pageFrameList[elementCount++] = pageNumber;
				return;
			}
			// page replacement
			pageFrameList[first++] = pageNumber;
			first %= pageFrameCount;
			return;
        }

		// dump the page frames
		void dump() {
			for (int i = 0; i < pageFrameList.length; i++)
				System.out.print("["+i+"]"+pageFrameList[i]+", ");
			System.out.println();
		}


		/**
		 * Searches for page pageNumber in the page frame list
		 * @return true if pageNumber was found
		 * @return false if pageNumber was not found
		 */
		boolean search(int pageNumber) {
			boolean returnVal = false;

			for (int i = 0; i < pageFrameList.length; i++) {
				if (pageNumber == pageFrameList[i]) {
					returnVal = true;
					break;
				}
			}
			return returnVal;
		}
	}
}
