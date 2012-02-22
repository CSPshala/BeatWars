using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Animation_Tool
{
    public class Animations
    {
        string name;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        List<Frame> lframes;

        internal List<Frame> lFrames
        {
            get { return lframes; }
            set { lframes = value; }
        }

        public override string ToString()
        {
            return name;
        }
    }
}
