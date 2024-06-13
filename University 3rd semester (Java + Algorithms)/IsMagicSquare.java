public class IsMagicSquare 
{
    public static int[][] createMagicSquare(final int size)
    {
        int[][] magicSquare = new int[size][size];

        for(int row = 0; row < size; row++) 
        {
            for(int index : magicSquare[row])
            {
                magicSquare[row][index] = -1;
            }
        }
         
        // Indices for element to be inserted
        int x = size / 2;
        int y = size - 1;
         
        // Loop till size ^ 2 times
        for(int i = 1; i <= size * size; i++) 
        {
            // Put the current element at (x, y)
            magicSquare[x][y] = i;
             
            if(i % size == 0) 
            {
                y--; // If we get multiple of size, move left
            } 
            else 
            {
                x--; y++; // Else, move top-right
            }

            // Take modulo to avoid out of bounds
            x = (x + size) % size;
            y = (y + size) % size;
        }

        return magicSquare;
    }

    // Returns true if arr[][] is magic
    // square, else returns false.
    public static boolean isMagicSquare(int arr[][])
    {
        final int SIZE = arr.length;
         
        int rightDiagonalSum = 0; // top-left -> bottom-right
        int leftDiagonalSum = 0; // top-right -> bottom-left

        for(int i = 0; i < SIZE; i++)
        {
            rightDiagonalSum += arr[i][i];
            leftDiagonalSum += arr[i][SIZE - 1 - i];
        }
        
        if(rightDiagonalSum != leftDiagonalSum)
            return false;
    
        // Calculating sums of rows and columns and checking if they are equal to each other,
        // as well as equal to diagonal sum or not
        for(int row = 0; row < SIZE; row++) 
        {
            int rowSum = 0;
            int colSum = 0;

            for(int column = 0; column < SIZE; column++)
            {
                rowSum += arr[row][column];
                colSum += arr[column][row];
            }

            if(rowSum != colSum || rowSum != rightDiagonalSum)
                return false;
        }
    
        return true;
    }

    public static void main(String[] args) 
    {
        System.out.println(isMagicSquare(createMagicSquare(7)));

        System.out.println(isMagicSquare(createMagicSquare(5)));
    }  
}
