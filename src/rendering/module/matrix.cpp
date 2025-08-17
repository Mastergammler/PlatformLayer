#include "../internal.h"

// FIXME: fighting the languge by wanting to give another parameter
//  but capture via lambda is not allawed for delegates as function pointers
template <typename T> void DefaultSwap(T& source, T& target, int length)
{
    T swapStore = target;
    target = source;
    source = swapStore;
}

template <typename T>
void matrix_row_swap(Matrix<T> m, SwapFunction<T> swap, int length)
{
    if (m.cell_count == 0 || m.rows < 1 || m.columns < 1) return;

    int targetPos = 0;
    int lockMin = 0;
    int lockMax = 0;

    // first row is special
    int col = 0;
    for (int r = 0; r < m.rows; r++)
    {
        int sourceValueLookup = r * m.columns + col;
        swap(m.data[targetPos], m.data[sourceValueLookup], length);
        /*T swapStore = m.data[targetPos];
        m.data[targetPos] = m.data[sourceValueLookup];
        m.data[sourceValueLookup] = swapStore;*/

        targetPos++;
    }

    lockMin = targetPos - 1;

    // now going back to front
    for (int c = 1; c < m.columns; c++)
    {
        targetPos += m.rows - 1;
        lockMax = targetPos;
        for (int r = m.rows - 1; r >= 0; r--)
        {
            int sourceValueLookup = r * m.columns + c;

            // source value is between lock & target OR
            // after target (because we're going back to front now)
            if ((sourceValueLookup > lockMin &&
                 sourceValueLookup <= targetPos) ||
                sourceValueLookup > lockMax)
            {
                // Mem pos = target pos
                // cur row pos = source pos
                swap(m.data[targetPos], m.data[sourceValueLookup], length);
                /*T swapValue = m.data[targetPos];
                m.data[targetPos] = m.data[sourceValueLookup];
                m.data[sourceValueLookup] = swapValue;*/
                /*logf("Swapping target %i with value from %i",
                     targetPos,
                     sourceValueLookup);*/
            }
            // this means memory is lock, means value was moved somewhere
            // so we need to backtrack where it went
            else
            {
                int sourceValuePrevPos = sourceValueLookup;

                int iterationCounter = 0;
                bool backtrack = true;
                while (backtrack && iterationCounter < m.cell_count)
                {
                    int tileCol = sourceValuePrevPos / m.rows;
                    int tileRow = sourceValuePrevPos % m.rows;

                    sourceValuePrevPos = tileRow * m.columns + tileCol;
                    /*logf("Searching target %i at position %i",
                         targetPos,
                         sourceValuePrevPos);*/

                    if ((sourceValuePrevPos > lockMin &&
                         sourceValuePrevPos <= targetPos) ||
                        sourceValuePrevPos > lockMax)
                    {
                        swap(m.data[targetPos],
                             m.data[sourceValuePrevPos],
                             length);
                        /*T swapValue = m.data[targetPos];
                        m.data[targetPos] = m.data[sourceValuePrevPos];
                        m.data[sourceValuePrevPos] = swapValue;*/
                        backtrack = false;

                        /*logf("Swapping target %i with value from %i",
                             targetPos,
                             sourceValuePrevPos);*/
                    }
                    else if (sourceValuePrevPos == targetPos)
                    {
                        // identity case, just continue
                        backtrack = false;
                    }

                    iterationCounter++;
                }
            }
            targetPos--;
        }

        targetPos = (c + 1) * m.rows;
        lockMin = lockMax;
    }
}
