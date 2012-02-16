using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeatMaker
{
    // Beat Structure, inherits from IComparable to make it comparable to
    // other beats so I can sort a list by time later.
    public class Beat : IComparable<Beat>
    {
        int nTimeOfBeat;
        string szDirection;
        char cKey;
        string szImage;
        string szDifficulty;
        int nWidth;
        int nHeight;

        public int CompareTo(Beat aBeat)
        {
            return nTimeOfBeat.CompareTo(aBeat.nTimeOfBeat);
        }

    }

}
