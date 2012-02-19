// System References
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ParticleFX
{
    public class PerformanceCounter
    {
        // Data Members
        int fps;
        int fpsTick;
        int LastTick;
        float gameTime;
        float elapsed;

        // Member Properties
        public int FPS
        {
            get { return fps; }
            protected set { fps = value; }
        }
        protected int FpsTick
        {
            get { return fpsTick; }
            set { fpsTick = value; }
        }
        public float GameTime
        {
            get { return gameTime; }
            protected set { gameTime = value; }
        }
        public float ElapsedTime
        {
            get { return elapsed; }
            protected set { elapsed = value; }
        }

        // Constructor
        public PerformanceCounter()
        {
            Reset();
        }

        // Methods
        public void Reset()
        {
            GameTime = 0.0f;
            FPS = 0;
            LastTick = Environment.TickCount;
            FpsTick = Environment.TickCount;
        }

        // Used for updating
        int CurrentTick;
        int DeltaTick;
        int DeltaElapsedTick;
        public void Update(System.Windows.Forms.ToolStripStatusLabel ToUpdate)
        {
            CurrentTick = Environment.TickCount;
            DeltaTick = CurrentTick - FpsTick;
            DeltaElapsedTick = CurrentTick - LastTick;

            ElapsedTime = (float)DeltaElapsedTick / 1000.0f;
            GameTime += ElapsedTime;

            if (DeltaTick > 1000)
            {
                ToUpdate.Text = "Frames: " + FPS.ToString();
                FPS = 0;
                FpsTick = CurrentTick;
            }

            LastTick = CurrentTick;
        }
        public void AddFrame()
        {
            ++FPS;
        }
    }
}
