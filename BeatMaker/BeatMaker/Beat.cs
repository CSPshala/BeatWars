using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace BeatMaker
{
    // Beat Structure, inherits from IComparable to make it comparable to
    // other beats so I can sort a list by time later.
    public enum BEATIS { COMPLETE, ARROW, KEY, EMPTY };

    public class Beat : IComparable<Beat>
    {
        // Drawing rect
        Rectangle rDraw;

        public Rectangle DrawRect
        {
            get { return rDraw; }
            set { rDraw = value; }
        }

        private uint nTimeOfBeatS;

        public uint TimeOfBeatS
        {
            get { return nTimeOfBeatS; }
            set { nTimeOfBeatS = value; }
        }


        public uint nTimeOfBeat;

        public uint TimeOfBeat
        {
            get { return nTimeOfBeat; }
            set { nTimeOfBeat = value;}
        }

        string szDirection;

        public string Direction
        {
            get { return szDirection; }
            set { szDirection = value; }
        }

        char cKey;

        public char KeyPress
        {
            get { return cKey; }
            set { cKey = value; }
        }

        string szImage;

        public string Image
        {
            get { return szImage; }
            set { szImage = value; }
        }

        string szDifficulty;

        public string Difficulty
        {
            get { return szDifficulty; }
            set { szDifficulty = value; }
        }

        int nWidth;

        public int Width
        {
            get { return nWidth; }
            set { nWidth = value; }
        }

        int nHeight;

        public int Height
        {
            get { return nHeight; }
            set { nHeight = value; }
        }

        // Shows what has been completed with current note
        BEATIS eCompletion;

        public BEATIS Completion
        {
            get { return eCompletion; }
            set { eCompletion = value; }
        }

        int nTextureIndex;

        public int TextureIndex
        {
            get { return nTextureIndex; }
            set { nTextureIndex = value; }
        }



        // Constructor
        public Beat()
        {
            nTimeOfBeatS = 0;
            nTimeOfBeat = 0;
            szDirection = "x";
            cKey = 'x';
            szImage = "x";
            szDifficulty = "easy";
            nWidth = 0;
            nHeight = 0;
            eCompletion = BEATIS.EMPTY;
            nTextureIndex = -1;
        }

        public Beat(Beat aBeat)
        {
            nTimeOfBeatS = aBeat.nTimeOfBeatS;
            nTimeOfBeat = aBeat.nTimeOfBeat;
            szDirection = aBeat.szDirection;
            cKey = aBeat.cKey;
            szImage = aBeat.szImage;
            szDifficulty = aBeat.szDifficulty;
            nWidth = aBeat.nWidth;
            nHeight = aBeat.nHeight;
            eCompletion = aBeat.eCompletion;
            nTextureIndex = aBeat.nTextureIndex;
        }

        public int CompareTo(Beat aBeat)
        {
            return nTimeOfBeat.CompareTo(aBeat.nTimeOfBeat);
        }

    }

}
