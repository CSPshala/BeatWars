using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Animation_Tool
{
    class Timer
    {
        // Starting Date Time
        System.DateTime startingTime;

        // Delta Date Time
        System.TimeSpan deltaTime;

        // Return time delta
        public System.TimeSpan DeltaTime
        {
            get { return deltaTime; }
        }

        // Previous Date Time
        System.DateTime previousTime;

        // Time since timer started
        System.TimeSpan runningTime;

        // Returns total time Timer has been running
        public System.TimeSpan RunningTime
        {
            get { return runningTime; }
        }


        public void InitTimer()
        {
            // Setup on date time stuff
            startingTime = System.DateTime.Now;

            previousTime = System.DateTime.Now;
        }

        public void Update()
        {
            // Getting delta time
            deltaTime = System.DateTime.Now - previousTime;

            previousTime = System.DateTime.Now;

            // Getting total runtime
            runningTime = System.DateTime.Now - startingTime;
        }
    }
}
