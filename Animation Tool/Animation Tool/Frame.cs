using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Animation_Tool
{
    class Frame
    {
        int anchorx;

        public int AnchorX
        {
            get { return anchorx; }
            set { anchorx = value; }
        }

        int anchory;

        public int AnchorY
        {
            get { return anchory; }
            set { anchory = value; }
        }

        int drawx;

        public int DrawX
        {
            get { return drawx; }
            set { drawx = value; }
        }

        int drawy;

        public int DrawY
        {
            get { return drawy; }
            set { drawy = value; }
        }

        int collisionx;

        public int CollisionX
        {
            get { return collisionx; }
            set { collisionx = value; }
        }

        int collisiony;

        public int CollisionY
        {
            get { return collisiony; }
            set { collisiony = value; }
        }

        int drawheight;
       
        public int DrawHeight
        {
            get { return drawheight; }
            set { drawheight = value; }
        }

        int drawwidth;

        public int DrawWidth
        {
            get { return drawwidth; }
            set { drawwidth = value; }
        }

        int collisionheight;

        public int CollisionHeight
        {
            get { return collisionheight; }
            set { collisionheight = value; }
        }

        int collisionwidth;

        public int CollisionWidth
        {
            get { return collisionwidth; }
            set { collisionwidth = value; }
        }


        string triggerevent;

        public string TriggerEvent
        {
            get { return triggerevent; }
            set { triggerevent = value; }
        }

        float duration;

        public float Duration
        {
            get { return duration; }
            set { duration = value; }
        }

        int framenumber;

        public int FrameNumber
        {
            get { return framenumber; }
            set { framenumber = value; }
        }


        public override string ToString()
        {
            string nNum;

            nNum = "Frame #";

            nNum += FrameNumber.ToString();

            return nNum;

        }

    }
}
